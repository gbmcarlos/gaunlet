#pragma once

#include "gaunlet/prefab/render-pipelines/basic-2d-render-pipeline/Renderer.h"
#include "gaunlet/prefab/render-pipelines/basic-2d-render-pipeline/PropertySets.h"
#include "gaunlet/graphics/renderer/BatchedRenderer.h"
#include "gaunlet/scene/entity/Entity.h"
#include "gaunlet/scene/camera/Camera.h"

namespace gaunlet::Prefab::Basic2DRenderPipeline {

    class Basic2DRenderPipeline {

    public:

        Basic2DRenderPipeline();
        virtual void run(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera);

    protected:

        virtual void clearBuffers();
        virtual void startScene(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera);
        virtual void submitModels(const Core::Ref<Scene::Scene>& scene);
        virtual void submitCircles(const Core::Ref<Scene::Scene>& scene);

        Core::Ref<Graphics::UniformBuffer> m_scenePropertiesUniformBuffer = nullptr;
        ModelRenderer m_modelRenderer;
        CircleRenderer m_circleRenderer;

    private:

        inline ModelRenderer& getModelRenderer() {return m_modelRenderer; }
        inline CircleRenderer& getCircleRenderer() {return m_circleRenderer; }

        Graphics::ShaderLibrary m_shaderLibrary;
        inline virtual Graphics::ShaderLibrary& getShaderLibrary() {return m_shaderLibrary; }
        void loadShaders();
        void loadModelShaders();
        void loadCircleShaders();

    };

}