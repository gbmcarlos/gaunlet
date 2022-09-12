#pragma once

#include "gaunlet/prefab/render-pipelines/basic-2d-render-pipeline/Basic2DRenderPipeline.h"
#include "gaunlet/scene/components/LightingComponents.h"

namespace gaunlet::Prefab::Basic3DRenderPipeline {

    class Basic3DRenderPipeline : public gaunlet::Prefab::Basic2DRenderPipeline::Basic2DRenderPipeline {

    public:

        Basic3DRenderPipeline();
        virtual void run(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera, const Core::Ref<Scene::DirectionalLightComponent>& directionalLight, const Core::Ref<Scene::SkyboxComponent>& skybox);

    protected:

        void startScene(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera, const Core::Ref<Scene::DirectionalLightComponent>& directionalLight);
        void renderSkybox(const Core::Ref<Scene::SkyboxComponent>& skybox);

    private:

        Graphics::ShaderLibrary m_shaderLibrary;
        inline Graphics::ShaderLibrary & getShaderLibrary() override {return m_shaderLibrary; }
        void loadShaders();
        void loadModelShaders();
        void loadCircleShaders();
        void loadSkyboxShaders();

    };

}