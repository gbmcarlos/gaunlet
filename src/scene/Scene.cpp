#include "Scene.h"

#include "entity/Entity.h"
#include "entity/ScriptComponents.h"
#include "renderer/Renderer.h"

namespace engine {

    Entity Scene::createEntity() {
        entt::entity entityHandle = m_registry.create();
        return {entityHandle, this};
    }

    void Scene::start(glm::vec2 gravity) {

        initPhysics(gravity);
        initScripts();

    }

    void Scene::onUpdate(TimeStep timeStep) {

        runScripts(timeStep);
        simulatePhysics(timeStep);
        renderElements();

    }

    void Scene::stop() {

        destroyScripts();
        m_physicsWorld.destroyPhysics();

    }

    void Scene::initPhysics(glm::vec2 gravity) {

        m_physicsWorld.initPhysics(gravity);

        auto group = m_registry.group<RigidBodyComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};

            auto [rigidBody, transform] = group.get<RigidBodyComponent, TransformComponent>(e);
            m_physicsWorld.createRigidBody(rigidBody, transform);

            if (entity.hasComponent<BoxColliderComponent>()) {
                auto& boxCollider = entity.getComponent<BoxColliderComponent>();
                m_physicsWorld.createBoxFixture(rigidBody.m_runtimeBody, boxCollider, transform);
            }

            if (entity.hasComponent<CircleColliderComponent>()) {
                auto& circleCollider = entity.getComponent<CircleColliderComponent>();
                m_physicsWorld.createCircleFixture(rigidBody.m_runtimeBody, circleCollider, transform);
            }

        }

    }

    void Scene::simulatePhysics(TimeStep timeStep) {

        m_physicsWorld.simulatePhysics(timeStep);

        auto group = m_registry.group<RigidBodyComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};
            auto [rigidBody, transform] = group.get<RigidBodyComponent, TransformComponent>(e);

            m_physicsWorld.updateBody(rigidBody, transform);

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

    void Scene::runScripts(TimeStep timeStep) {

        m_registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nativeScriptComponent) {
            nativeScriptComponent.m_nativeScriptInstance->onUpdate(timeStep);
        });

    }

    void Scene::destroyScripts() {

        m_registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nativeScriptComponent) {
            // Destroy the instance of the native script
            nativeScriptComponent.m_destroyScriptFunction(&nativeScriptComponent);
        });

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