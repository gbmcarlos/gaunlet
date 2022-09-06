#pragma once

#include "gaunlet/core/application/TimeStep.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"
#include "gaunlet/scene/camera/OrthographicCamera.h"
#include "gaunlet/scene/components/GraphicsComponents.h"
#include "gaunlet/scene/components/LightingComponents.h"
#include "gaunlet/scene/deferred-renderer/DeferredRenderer.h"
#include "gaunlet/core/render/RenderCommand.h"
#include "gaunlet/scene/entity/Entity.h"

namespace gaunlet::Scene {

    class Scene {

    public:
        Scene();
        ~Scene();

        Registry& getRegistry() {return m_registry; }
        void start();
        void play();
        void pause();
        void togglePlay();

        void update(Core::TimeStep timeStep);

        template<typename T>
        void renderTagged(RenderMode renderMode, const Core::Ref<Camera>& camera, const Core::Ref<DirectionalLightComponent>& directionalLight, const Core::Ref<SkyboxComponent>& skybox);
        void render(RenderMode renderMode, const Core::Ref<Camera>& camera, const Core::Ref<DirectionalLightComponent>& directionalLight, const Core::Ref<SkyboxComponent>& skybox);

        void stop();

    private:

        bool m_playing = false;
        Registry m_registry;

        void initScripts();
        void runScripts(Core::TimeStep e);
        void destroyScripts();

        template<typename T>
        void renderTaggedModels();
        void renderModels();

        template<typename T>
        void renderTaggedCircles();
        void renderCircles();

        void submitModel(Entity entity);
        void submitCircle(Entity entity);

        static glm::mat4 getHierarchicalTransform(Entity entity);

    };

    template<typename T>
    void Scene::renderTagged(RenderMode renderMode, const Core::Ref<Camera> &camera, const Core::Ref<DirectionalLightComponent>& directionalLight, const Core::Ref<SkyboxComponent>& skybox) {

        DeferredRenderer::beginScene(
            renderMode,
            camera->getViewMatrix(),
            camera->getProjectionMatrix(),
            directionalLight,
            skybox->m_cubeMap
        );

        renderTaggedModels<T>();
        renderTaggedCircles<T>();

        DeferredRenderer::endScene();

    }

    template<typename T>
    void Scene::renderTaggedModels() {

        auto group = m_registry.m_registry.view<ModelComponent, TransformComponent, T>();
        for (auto e : group) {

            Entity entity = {e, &m_registry};
            submitModel(entity);

        }

    }

    template<typename T>
    void Scene::renderTaggedCircles() {

        auto group = m_registry.m_registry.view<CircleComponent, TransformComponent, T>();
        for (auto e : group) {

            Entity entity = {e, &m_registry};
            submitCircle(entity);

        }

    }

}