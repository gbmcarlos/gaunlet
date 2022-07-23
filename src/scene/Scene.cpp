#include "Scene.h"

#include "entity/Entity.h"
#include "entity/GraphicsComponents.h"
#include "entity/ScriptComponents.h"
#include "renderer/Renderer.h"

#include <iostream>

namespace engine {

    Scene::Scene() {
        m_physicsWorld = new b2World({0.0f, -9.8f});
    }

    Scene::~Scene() {
        delete m_physicsWorld;
    }

    Entity Scene::createEntity() {
        entt::entity entityHandle = m_registry.create();
        return {entityHandle, this};
    }

    void Scene::start() {

        initScripts();
        initPhysics();

    }

    void Scene::initPhysics() {

        auto group = m_registry.group<RigidBodyComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};
            auto [rigidBody, transform] = group.get<RigidBodyComponent, TransformComponent>(e);

            b2BodyDef bodyDefinition;
            bodyDefinition.type = convertRigidBodyType(rigidBody.m_type);
            bodyDefinition.position.Set(transform.m_translation.x, transform.m_translation.y);
            bodyDefinition.angle = glm::radians(transform.m_rotation.z);
            b2Body* body = m_physicsWorld->CreateBody(&bodyDefinition);

            rigidBody.m_runtimeBody = body;

            if (entity.hasComponent<BoxColliderComponent>()) {

                auto& boxCollider = entity.getComponent<BoxColliderComponent>();

                b2PolygonShape box;
                box.SetAsBox(
                    (boxCollider.m_size.x * transform.m_scale.x) + boxCollider.m_padding.x,
                    (boxCollider.m_size.y * transform.m_scale.y) + boxCollider.m_padding.y
                );

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &box;
                fixtureDef.density = boxCollider.m_density;

                auto fixture = body->CreateFixture(&fixtureDef);
                boxCollider.m_runtimeFixture = fixture;

            }


        }

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