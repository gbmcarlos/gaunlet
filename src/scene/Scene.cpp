#include "Scene.h"

#include "entity/Entity.h"
#include "entity/ScriptComponents.h"
#include "deferred-renderer/DeferredRenderer.h"

namespace engine {

    Scene::Scene() {
        DeferredRenderer::init();
    }

    Scene::~Scene() {
        destroyScripts();
        m_physicsWorld->destroyPhysics();
    }

    Entity Scene::createEntity() {
        entt::entity entityHandle = m_registry.create();
        return {entityHandle, this};
    }

    const Ref<PhysicsWorld>& Scene::enablePhysics(glm::vec2 gravity) {
        m_physicsWorld = CreateRef<PhysicsWorld>(gravity);
        return m_physicsWorld;
    }

    void Scene::start() {

        if (m_physicsWorld) {
            initPhysics();
        }

        initScripts();

        play();

    }

    void Scene::play() {
        m_playing = true;
    }

    void Scene::pause() {
        m_playing = false;
    }

    void Scene::togglePlay() {
        m_playing = !m_playing;
    }

    void Scene::update(TimeStep timeStep) {

        if (m_playing) {

            runScripts(timeStep);
            if (m_physicsWorld) {
                simulatePhysics(timeStep);
            }

        }

    }

    void Scene::render(const Ref<Camera>& camera, const Ref<Framebuffer>& framebuffer) {

        DeferredRenderer::beginScene(
            camera->getViewMatrix(),
            camera->getProjectionMatrix(),
            framebuffer,
            DirectionalLightComponent()
        );

        renderPolygons();
        renderCircles();

        DeferredRenderer::endScene();

    }

    void Scene::render(const Ref<Camera>& camera, const DirectionalLightComponent& directionalLight, const Ref<Framebuffer>& framebuffer) {

        DeferredRenderer::beginScene(
            camera->getViewMatrix(),
            camera->getProjectionMatrix(),
            framebuffer,
            directionalLight
        );

        renderPolygons();
        renderCircles();

        DeferredRenderer::endScene();

    }

    void Scene::stop() {}

    void Scene::initPhysics() {

        auto group = m_registry.group<RigidBodyComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};

            auto [rigidBody, transform] = group.get<RigidBodyComponent, TransformComponent>(e);
            m_physicsWorld->createRigidBody(rigidBody, transform);

            if (entity.hasComponent<BoxColliderComponent>()) {
                auto& boxCollider = entity.getComponent<BoxColliderComponent>();
                m_physicsWorld->createBoxFixture(rigidBody.m_runtimeBody, boxCollider, transform);
            }

            if (entity.hasComponent<CircleColliderComponent>()) {
                auto& circleCollider = entity.getComponent<CircleColliderComponent>();
                m_physicsWorld->createCircleFixture(rigidBody.m_runtimeBody, circleCollider, transform);
            }

        }

    }

    void Scene::simulatePhysics(TimeStep timeStep) {

        m_physicsWorld->simulatePhysics(timeStep);

        auto group = m_registry.group<RigidBodyComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};
            auto [rigidBody, transform] = group.get<RigidBodyComponent, TransformComponent>(e);

            m_physicsWorld->updateBody(rigidBody, transform);

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

    void Scene::renderPolygons() {

        auto group = m_registry.group<PolygonModelComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};
            auto [polygon, transform] = group.get<PolygonModelComponent, TransformComponent>(e);

            // MaterialComponent is optional
            auto material = entity.hasComponent<MaterialComponent>() ? entity.getComponent<MaterialComponent>() : MaterialComponent();

            DeferredRenderer::submit(entity.getId(), polygon, transform, material);

        }

    }

    void Scene::renderCircles() {

        auto group = m_registry.group<CircleModelComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};
            auto [circle, transform] = group.get<CircleModelComponent, TransformComponent>(e);

            // MaterialComponent is optional
            auto material = entity.hasComponent<MaterialComponent>() ? entity.getComponent<MaterialComponent>() : MaterialComponent();

            DeferredRenderer::submit(entity.getId(), circle, transform, material);

        }

    }

}