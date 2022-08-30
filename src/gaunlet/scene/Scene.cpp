#include "gaunlet/scene/Scene.h"

#include "gaunlet/scene/components/ScriptComponents.h"
#include "gaunlet/core/render/RenderCommand.h"

namespace gaunlet::Scene {

    Scene::Scene() {
        DeferredRenderer::init();
    }

    Scene::~Scene() {
        destroyScripts();
        m_physicsWorld->destroyPhysics();
    }

    const Core::Ref<PhysicsWorld>& Scene::enablePhysics(glm::vec2 gravity) {
        m_physicsWorld = Core::CreateRef<PhysicsWorld>(gravity);
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

    void Scene::update(Core::TimeStep timeStep) {

        if (m_playing) {

            runScripts(timeStep);
            if (m_physicsWorld) {
                simulatePhysics(timeStep);
            }

        }

    }

    void Scene::render(RenderMode renderMode, const Core::Ref<Camera>& camera, const DirectionalLightComponent& directionalLight) {

        DeferredRenderer::beginScene(
            renderMode,
            camera->getViewMatrix(),
            camera->getProjectionMatrix(),
            directionalLight
        );

        renderModels();
        renderCircles();

        DeferredRenderer::endScene();

    }

    void Scene::stop() {}

    void Scene::initPhysics() {

        auto group = m_registry.m_registry.group<RigidBodyComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, &m_registry};

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

    void Scene::simulatePhysics(Core::TimeStep timeStep) {

        m_physicsWorld->simulatePhysics(timeStep);

        auto group = m_registry.m_registry.group<RigidBodyComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, &m_registry};
            auto [rigidBody, transform] = group.get<RigidBodyComponent, TransformComponent>(e);

            m_physicsWorld->updateBody(rigidBody, transform);

        }

    }

    void Scene::initScripts() {

        m_registry.m_registry.view<NativeScriptComponent>().each([=](entt::entity e, NativeScriptComponent& nativeScriptComponent) {

            // Create the instance of the native script
            nativeScriptComponent.m_nativeScriptInstance = nativeScriptComponent.m_instantiateScriptFunction();
            nativeScriptComponent.m_nativeScriptInstance->m_entity = {e, &m_registry};

            // Call the native script's onCreate
            nativeScriptComponent.m_nativeScriptInstance->onCreate();

        });

    }

    void Scene::runScripts(Core::TimeStep timeStep) {

        m_registry.m_registry.view<NativeScriptComponent>().each([=](entt::entity e, NativeScriptComponent& nativeScriptComponent) {
            nativeScriptComponent.m_nativeScriptInstance->onUpdate(timeStep);
        });

    }

    void Scene::destroyScripts() {

        m_registry.m_registry.view<NativeScriptComponent>().each([=](entt::entity e, NativeScriptComponent& nativeScriptComponent) {
            // Destroy the instance of the native script
            nativeScriptComponent.m_destroyScriptFunction(&nativeScriptComponent);
        });

    }

    void Scene::renderModels() {

        auto group = m_registry.m_registry.group<ModelComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, &m_registry};
            submitModel(entity);

        }

    }

    void Scene::renderCircles() {

        auto group = m_registry.m_registry.group<CircleComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, &m_registry};
            submitCircle(entity);
        }

    }

    void Scene::submitModel(Entity entity) {

        auto model = entity.getComponent<ModelComponent>();

        // Get the entity's transform, relative to its parent's (all the way up the chain)
        auto hierarchicalTransform = getHierarchicalTransform(entity);

        // MaterialComponent is optional
        auto material = entity.hasComponent<MaterialComponent>() ? entity.getComponent<MaterialComponent>() : MaterialComponent();

        DeferredRenderer::submit(entity.getId(), model, hierarchicalTransform, material);


    }

    void Scene::submitCircle(Entity entity) {

        auto circle = entity.getComponent<CircleComponent>();

        // Get the entity's transform, relative to its parent's (all the way up the chain)
        auto hierarchicalTransform = getHierarchicalTransform(entity);

        // MaterialComponent is optional
        auto material = entity.hasComponent<MaterialComponent>() ? entity.getComponent<MaterialComponent>() : MaterialComponent();

        DeferredRenderer::submit(entity.getId(), circle, hierarchicalTransform, material);


    }

    glm::mat4 Scene::getHierarchicalTransform(Entity entity) {

        glm::mat4 result = entity.getComponent<TransformComponent>().getTransformationMatrix();
        Entity current = entity;

        while (true) {

            auto parent = current.getParent();

            if (!parent || !parent.hasComponent<TransformComponent>()) {
                break;
            }

            // Multiply with the parent's transform and move to the next generation
            result *= parent.getComponent<TransformComponent>().getTransformationMatrix();
            current = parent;

        }

        return result;

    }

}