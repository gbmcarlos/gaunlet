#pragma once

#include "gaunlet/prefab/render-pipelines/basic-3d-render-pipeline/Basic3DRenderPipeline.h"
#include "gaunlet/prefab/render-pipelines/basic-editor-render-pipeline/Renderer.h"
#include "gaunlet/editor/workspace/FramebufferRenderPipeline.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"

namespace gaunlet::Prefab::BasicEditorRenderPipeline {

    class BasicEditorRenderPipeline : public Editor::FramebufferRenderPipeline, Basic3DRenderPipeline::Basic3DRenderPipeline {

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

        void drawScene(const Core::Ref<Scene::Scene>& scene);
        void drawOutlines(const Core::Ref<Scene::Scene>& scene);
        void drawUI(const Core::Ref<Scene::Scene>& scene);
        void drawSkybox(const Core::Ref<Scene::SkyboxComponent>& skybox);

        virtual void submitSceneModels(const Core::Ref<Scene::Scene>& scene);
        virtual void submitSceneCircles(const Core::Ref<Scene::Scene>& scene);

        void submitOutlines(const Core::Ref<Scene::Scene>& scene);

        virtual void submitUIModels(const Core::Ref<Scene::Scene>& scene);
        virtual void submitUICircles(const Core::Ref<Scene::Scene>& scene);


    protected:

        ModelRenderer m_modelRenderer;
        CircleRenderer m_circleRenderer;

    private:

        gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer> m_framebuffer = nullptr;
        Graphics::ShaderLibrary m_shaderLibrary;
        inline virtual Graphics::ShaderLibrary& getShaderLibrary() override {return m_shaderLibrary; }
        void loadShaders();
        void loadModelShaders();
        void loadCircleShaders();
        void loadSkyboxShaders();
        void loadModelOutlineShaders();

    };

}