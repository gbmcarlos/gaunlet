#include "Scene.h"

#include "entity/Entity.h"
#include "entity/ScriptComponents.h"
#include "renderer/Renderer.h"

#include <iostream>

namespace engine {

    Scene::~Scene() {

        if (m_physicsWorld) {
            destroyPhysics();
        }

    }

    Entity Scene::createEntity() {
        entt::entity entityHandle = m_registry.create();
        return {entityHandle, this};
    }

    void Scene::start(glm::vec2 gravity) {

        initPhysics(gravity);
        initScripts();

    }

    void Scene::stop() {

        destroyScripts();
        destroyPhysics();

    }

    void Scene::initPhysics(glm::vec2 gravity) {

        m_physicsWorld = new b2World({gravity.x, gravity.y});

        auto group = m_registry.group<RigidBodyComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};

            auto [rigidBody, transform] = group.get<RigidBodyComponent, TransformComponent>(e);
            createRigidBody(rigidBody, transform);

            if (entity.hasComponent<BoxColliderComponent>()) {
                auto& boxCollider = entity.getComponent<BoxColliderComponent>();
                createBoxFixture(rigidBody.m_runtimeBody, boxCollider, transform);
            }

            if (entity.hasComponent<CircleColliderComponent>()) {
                auto& circleCollider = entity.getComponent<CircleColliderComponent>();
                createCircleFixture(rigidBody.m_runtimeBody, circleCollider, transform);
            }

        }

    }

    void Scene::createRigidBody(RigidBodyComponent& rigidBody, TransformComponent transform) {

        b2BodyDef bodyDefinition;
        bodyDefinition.type = convertRigidBodyType(rigidBody.m_type);
        bodyDefinition.position.Set(transform.m_translation.x, transform.m_translation.y);
        bodyDefinition.fixedRotation = rigidBody.m_fixedRotation;
        bodyDefinition.angle = glm::radians(transform.m_rotation.z);

        b2Body* body = m_physicsWorld->CreateBody(&bodyDefinition);
        rigidBody.m_runtimeBody = body;

    }

    void Scene::createBoxFixture(b2Body* body, BoxColliderComponent& boxCollider, TransformComponent transform) {

        b2PolygonShape shape;
        shape.SetAsBox(
            (boxCollider.m_size.x * transform.m_scale.x) + boxCollider.m_padding.x,
            (boxCollider.m_size.y * transform.m_scale.y) + boxCollider.m_padding.y
        );

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = boxCollider.m_density;
        fixtureDef.friction = boxCollider.m_friction;
        fixtureDef.restitution = boxCollider.m_restitution;
        fixtureDef.restitutionThreshold = boxCollider.m_restitutionThreshold;

        auto fixture = body->CreateFixture(&fixtureDef);
        boxCollider.m_runtimeFixture = fixture;

    }

    void Scene::createCircleFixture(b2Body* body, CircleColliderComponent& boxCollider, TransformComponent transform) {

        b2CircleShape shape;

        float scale = std::max(transform.m_scale.x, transform.m_scale.x);
        shape.m_p.Set(0.0f, 0.0f); // Position the fixture relative to the center of the body' position
        shape.m_radius = (boxCollider.m_radius * scale) + boxCollider.m_padding;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = boxCollider.m_density;
        fixtureDef.friction = boxCollider.m_friction;
        fixtureDef.restitution = boxCollider.m_restitution;
        fixtureDef.restitutionThreshold = boxCollider.m_restitutionThreshold;

        auto fixture = body->CreateFixture(&fixtureDef);
        boxCollider.m_runtimeFixture = fixture;

    }

    void Scene::initScripts() {

        m_registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nativeScriptComponent) {

            // Create the instance of the native script
            nativeScriptComponent.m_nativeScriptInstance = nativeScriptComponent.m_instantiateScriptFunction();
            nativeScriptComponent.m_nativeScriptInstance->m_entity = {entity, this};

            // Call the native script's onCreate
            nativeScriptComponent.m_nativeScriptInstance->onCreate();

        });

    }

    void Scene::destroyPhysics() {

        m_physicsWorld->ClearForces();
        delete m_physicsWorld;

    }

    void Scene::destroyScripts() {

        m_registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nativeScriptComponent) {
            // Destroy the instance of the native script
            nativeScriptComponent.m_destroyScriptFunction(&nativeScriptComponent);
        });

    }

    void Scene::onUpdate(TimeStep timeStep) {

        runScripts(timeStep);
        simulatePhysics(timeStep);
        renderElements();

    }

    void Scene::runScripts(TimeStep timeStep) {

        m_registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nativeScriptComponent) {
            nativeScriptComponent.m_nativeScriptInstance->onUpdate(timeStep);
        });

    }

    void Scene::simulatePhysics(TimeStep timeStep) {

        m_physicsWorld->Step(timeStep.getSeconds(), 6, 2);

        auto group = m_registry.group<RigidBodyComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};
            auto [rigidBody, transform] = group.get<RigidBodyComponent, TransformComponent>(e);

            auto* body = (b2Body*) rigidBody.m_runtimeBody;
            const auto& position = body->GetPosition();
            transform.m_translation.x = position.x;
            transform.m_translation.y = position.y;
            transform.m_rotation.z = glm::degrees(body->GetAngle());

        }

    }

    void Scene::renderElements() {

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        renderPolygons();
        renderCircles();

        engine::Renderer::flushPolygons();
        engine::Renderer::flushCircles();

    }

    void Scene::renderPolygons() {

        auto group = m_registry.group<PolygonComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};
            auto [polygon, transform] = group.get<PolygonComponent, TransformComponent>(e);

            // MaterialComponent is optional
            auto material = entity.hasComponent<MaterialComponent>() ? entity.getComponent<MaterialComponent>() : MaterialComponent();

            Renderer::submit(polygon, transform, material);

        }

    }

    void Scene::renderCircles() {

        auto group = m_registry.group<CircleComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};
            auto [circle, transform] = group.get<CircleComponent, TransformComponent>(e);

            // MaterialComponent is optional
            auto material = entity.hasComponent<MaterialComponent>() ? entity.getComponent<MaterialComponent>() : MaterialComponent();

            Renderer::submit(circle, transform, material);

        }

    }

}