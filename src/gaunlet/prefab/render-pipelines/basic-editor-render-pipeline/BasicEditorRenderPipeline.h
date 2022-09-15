#pragma once

#include "gaunlet/prefab/object-renderers/model-renderer/ModelRenderer.h"
#include "gaunlet/prefab/object-renderers/circle-renderer/CircleRenderer.h"
#include "gaunlet/prefab/object-renderers/skybox-renderer/SkyboxRenderer.h"
#include "gaunlet/prefab/render-pipelines/basic-3d-render-pipeline/Basic3DRenderPipeline.h"
#include "gaunlet/editor/workspace/FramebufferRenderPipeline.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"

namespace gaunlet::Prefab::BasicEditorRenderPipeline {

    class BasicEditorRenderPipeline : public Editor::FramebufferRenderPipeline {

    public:

        BasicEditorRenderPipeline();
        void run(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera, const Core::Ref<Scene::DirectionalLightComponent>& directionalLight, const Core::Ref<Scene::SkyboxComponent>& skybox) override;
        void resize(unsigned int width, unsigned int height) override;
        const Core::Ref<Graphics::Texture>& getRenderedTexture() override;
        int readFramebuffer(Editor::FramebufferLayer layer, unsigned int x, unsigned int y) override;

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
//        virtual void submitScenePlanes(const Core::Ref<Scene::Scene>& scene);

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

        void prepareShaders();

        gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer> m_framebuffer = nullptr;

    };

}