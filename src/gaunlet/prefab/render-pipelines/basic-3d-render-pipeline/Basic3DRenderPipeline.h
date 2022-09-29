#pragma once

#include "gaunlet/scene/camera/Camera.h"
#include "gaunlet/prefab/object-renderers/model-renderer/ModelRenderer.h"
#include "gaunlet/prefab/object-renderers/circle-renderer/CircleRenderer.h"
#include "gaunlet/prefab/object-renderers/skybox-renderer/SkyboxRenderer.h"

namespace gaunlet::Prefab::RenderPipelines {

    class Basic3DRenderPipeline {

    public:

        explicit Basic3DRenderPipeline(Core::Ref<Scene::DirectionalLightComponent> directionalLight = nullptr, Core::Ref<Scene::SkyboxComponent> skybox = nullptr);
        virtual void run(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera);

    protected:

        virtual void clearBuffers();
        void startScene(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera, const Core::Ref<Scene::DirectionalLightComponent>& directionalLight);
        virtual void renderModels(const Core::Ref<Scene::Scene>& scene);
        virtual void renderCircles(const Core::Ref<Scene::Scene>& scene);
        virtual void renderSkybox(const Core::Ref<Scene::SkyboxComponent>& skybox);

    protected:

        Core::Ref<Graphics::UniformBuffer> m_scenePropertiesUniformBuffer = nullptr;

        Prefab::ObjectRenderers::ModelRenderer m_modelRenderer;
        Prefab::ObjectRenderers::CircleRenderer m_circleRenderer;
        Prefab::ObjectRenderers::SkyboxRenderer m_skyboxRenderer;

    private:

        void prepareShaders();
        Core::Ref<Scene::DirectionalLightComponent> m_directionalLight = nullptr;
        Core::Ref<Scene::SkyboxComponent> m_skybox = nullptr;

    };

}