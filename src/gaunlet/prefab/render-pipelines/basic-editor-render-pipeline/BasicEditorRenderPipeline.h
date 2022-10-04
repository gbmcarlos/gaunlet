#pragma once

#include "gaunlet/prefab/object-renderers/model-renderer/ModelRenderer.h"
#include "gaunlet/prefab/object-renderers/circle-renderer/CircleRenderer.h"
#include "gaunlet/prefab/object-renderers/skybox-renderer/SkyboxRenderer.h"
#include "gaunlet/editor/render-pipeline/RenderPipeline.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"

namespace gaunlet::Prefab::RenderPipelines {

    class BasicEditorRenderPipeline : public Editor::RenderPipeline {

    public:

        explicit BasicEditorRenderPipeline(Core::Ref<Scene::DirectionalLightComponent> directionalLight, Core::Ref<Scene::SkyboxComponent> skybox, unsigned int uniformBufferBindingPointOffset = 0);
        void run(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera) override;
        void resize(unsigned int width, unsigned int height) override;
        const Core::Ref<Graphics::Texture>& getRenderTarget() override;
        static unsigned int getUniformBufferCount();

    protected:

        static const unsigned int SceneFramebufferAttachmentIndex = 0;
        static const unsigned int SceneEntityIdFramebufferAttachmentIndex = 1;
        static const unsigned int UIEntityIdFramebufferAttachmentIndex = 2;

        void startScene(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera, const Core::Ref<Scene::DirectionalLightComponent>& directionalLight);
        void drawScene(const Core::Ref<Scene::Scene>& scene);
        void drawOutlines(const Core::Ref<Scene::Scene>& scene);
        void drawUI(const Core::Ref<Scene::Scene>& scene);
        void drawSkybox(const Core::Ref<Scene::SkyboxComponent>& skybox);

        virtual void renderSceneModels(const Core::Ref<Scene::Scene>& scene);
        virtual void renderSceneCircles(const Core::Ref<Scene::Scene>& scene);

        void renderOutlines(const Core::Ref<Scene::Scene>& scene);

        virtual void renderUIModels(const Core::Ref<Scene::Scene>& scene);
        virtual void renderUICircles(const Core::Ref<Scene::Scene>& scene);

        virtual void renderSkybox(const Core::Ref<Scene::SkyboxComponent>& skybox);

    protected:

        Core::Ref<Graphics::UniformBuffer> m_scenePropertiesUniformBuffer = nullptr;

        Prefab::ObjectRenderers::ModelRenderer m_modelRenderer;
        Prefab::ObjectRenderers::CircleRenderer m_circleRenderer;
        Prefab::ObjectRenderers::SkyboxRenderer m_skyboxRenderer;

    private:

        void prepareShaders(unsigned int uniformBufferBindingPointOffset);
        void prepareFramebuffer();
        Core::Ref<Scene::DirectionalLightComponent> m_directionalLight = nullptr;
        Core::Ref<Scene::SkyboxComponent> m_skybox = nullptr;

        gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer> m_framebuffer = nullptr;

    };

}