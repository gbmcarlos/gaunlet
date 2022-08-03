#include "Scene.h"

#include "entity/Entity.h"
#include "entity/ScriptComponents.h"
#include "renderer/Renderer.h"

namespace engine {

    Scene::Scene() {
        engine::Renderer::init();
    }

    Scene::~Scene() {
        destroyScripts();
        m_physicsWorld->destroyPhysics();
    }

    Entity Scene::createEntity() {
        entt::entity entityHandle = m_registry.create();
        return {entityHandle, this};
    }

    const std::shared_ptr<PhysicsWorld>& Scene::enablePhysics(glm::vec2 gravity) {
        m_physicsWorld = std::make_shared<PhysicsWorld>(gravity);
        return m_physicsWorld;
    }

    void Scene::start(const std::shared_ptr<Camera>& camera) {

        m_camera = camera;

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

    void Scene::onUpdate(TimeStep timeStep) {

        if (m_camera == nullptr) {
            throw std::runtime_error("Scene hasn't been started with ::start");
        }

        if (m_playing) {

            runScripts(timeStep);
            if (m_physicsWorld) {
                simulatePhysics(timeStep);
            }

        }

        renderElements();

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

    void Scene::renderElements() {

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        engine::Renderer::beginScene(m_camera->getViewProjectionMatrix());

        renderPolygons();
        renderCircles();

        engine::Renderer::endScene();

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

        engine::Renderer::flushPolygons();

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

        engine::Renderer::flushCircles();

    }

}