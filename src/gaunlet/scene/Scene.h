#pragma once

#include "gaunlet/core/application/TimeStep.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"
#include "gaunlet/scene/camera/OrthographicCamera.h"
#include "gaunlet/scene/entity/GraphicsComponents.h"
#include "gaunlet/scene/entity/PhysicsComponents.h"
#include "gaunlet/scene/entity/LightingComponents.h"
#include "gaunlet/scene/physics/PhysicsWorld.h"
#include "gaunlet/scene/deferred-renderer/DeferredRenderer.h"
#include "gaunlet/core/render/RenderCommand.h"

#include <entt/entt.hpp>

/*
 * 2 tricks in this file
 * The Scene and the Entity depend on each other (the Scene composes an Entity object, using its constructor, and the Entity contains a reference to the Scene, and uses its methods)
 * The Scene has templated methods
 * So, define the Scene with a forward-declared Entity, and then define the Entity
 * The methods of the Scene that depend on the Entity's constructor need to be defined after the Entity has been defined:
 * Those that are non-templated, are implemented in the source file
 * Those that are templated, are implemented in the header itself, but after the Entity has been defined
 */

namespace gaunlet::Scene {

    class Entity;

    class Scene {

        friend class Entity;

    public:
        Scene();
        ~Scene();

        Entity createEntity();

        const Core::Ref<PhysicsWorld>& enablePhysics(glm::vec2 gravity);
        void start();
        void play();
        void pause();
        void togglePlay();

        void update(Core::TimeStep timeStep);

        template<typename T>
        void renderTagged(RenderMode renderMode, const Core::Ref<Camera>& camera, const DirectionalLightComponent& directionalLight);
        void render(RenderMode renderMode, const Core::Ref<Camera>& camera, const DirectionalLightComponent& directionalLight);

        void stop();

    private:

        bool m_playing = false;
        entt::registry m_registry;
        Core::Ref<Camera> m_camera = nullptr;
        Core::Ref<PhysicsWorld> m_physicsWorld = nullptr;

        void initPhysics();
        void simulatePhysics(Core::TimeStep timeStep);

        void initScripts();
        void runScripts(Core::TimeStep timeStep);
        void destroyScripts();

        template<typename T>
        void renderTaggedModels();
        void renderModels();

        template<typename T>
        void renderTaggedCircles();
        void renderCircles();

    };

    class Entity {

    public:

        Entity() = default;
        Entity(entt::entity entityHandle, Scene* scene) : m_handle(entityHandle), m_scene(scene) {}

        int getId() {
            return (int) m_handle;
        }

        template<typename T>
        bool hasComponent() {
            return m_scene->m_registry.all_of<T>(m_handle);
        }

        template<typename T>
        T& getComponent() {
            if (!hasComponent<T>()) {
                throw std::runtime_error("Component not found");
            }
            return m_scene->m_registry.get<T>(m_handle);
        }

        template<typename T, typename... Args>
        T& addComponent(Args&&... args) {
            return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
        }

        template<typename T, typename... Args>
        void addEmptyComponent(Args&&... args) {
            m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
        }

    private:
        entt::entity m_handle = entt::null;
        Scene* m_scene = nullptr;

    };

    template<typename T>
    void Scene::renderTagged(RenderMode renderMode, const Core::Ref<Camera> &camera, const DirectionalLightComponent &directionalLight) {

        DeferredRenderer::beginScene(
            renderMode,
            camera->getViewMatrix(),
            camera->getProjectionMatrix(),
            directionalLight
        );

        renderTaggedModels<T>();
        renderTaggedCircles<T>();

        DeferredRenderer::endScene();

    }

    template<typename T>
    void Scene::renderTaggedModels() {

        auto group = m_registry.view<ModelComponent, TransformComponent, T>();
        for (auto e : group) {

            Entity entity = {e, this};
            auto [model, transform] = group.template get<ModelComponent, TransformComponent>(e);

            // MaterialComponent is optional
            auto material = entity.hasComponent<MaterialComponent>() ? entity.getComponent<MaterialComponent>() : MaterialComponent();

            DeferredRenderer::submit(entity.getId(), model, transform, material);

        }

    }

    template<typename T>
    void Scene::renderTaggedCircles() {

        auto group = m_registry.view<CircleComponent, TransformComponent, T>();
        for (auto e : group) {

            Entity entity = {e, this};
            auto [circle, transform] = group.template get<CircleComponent, TransformComponent>(e);

            // MaterialComponent is optional
            auto material = entity.hasComponent<MaterialComponent>() ? entity.getComponent<MaterialComponent>() : MaterialComponent();

            DeferredRenderer::submit(entity.getId(), circle, transform, material);

        }

    }

}