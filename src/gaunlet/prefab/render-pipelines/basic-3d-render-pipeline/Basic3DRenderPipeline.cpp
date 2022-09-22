#include "gaunlet/prefab/render-pipelines/basic-3d-render-pipeline/Basic3DRenderPipeline.h"

namespace gaunlet::Prefab::Basic3DRenderPipeline {

    Basic3DRenderPipeline::Basic3DRenderPipeline()
        : m_modelRenderer(1), m_circleRenderer(2) {
        prepareShaders();
    }

    void Basic3DRenderPipeline::run(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera, const Core::Ref<Scene::DirectionalLightComponent>& directionalLight, const Core::Ref<Scene::SkyboxComponent>& skybox) {

        clearBuffers();
        startScene(scene, camera, directionalLight ? directionalLight : Core::CreateRef<Scene::DirectionalLightComponent>());

        Core::RenderCommand::setDepthFunction(Core::DepthStencilFunction::Less);

        // Always write 1 to the stencil buffer
        Core::RenderCommand::setStencilFunction(
            Core::DepthStencilFunction::Always, 1
        );

        // Write to the stencil buffer only when we draw
        Core::RenderCommand::setStencilOperation(
            true,
            Core::StencilOperation::Keep,
            Core::StencilOperation::Keep,
            Core::StencilOperation::Replace // We only write if we actually draw
        );

        renderCircles(scene);
        renderModels(scene);

        if (skybox->m_cubeMap) {

            Core::RenderCommand::setDepthFunction(Core::DepthStencilFunction::Always);

            // Draw only if the stencil buffer is empty
            Core::RenderCommand::setStencilFunction(
                Core::DepthStencilFunction::NotEqual, 1
            );

            // Don't write to the stencil buffer
            Core::RenderCommand::setStencilOperation(
                false,
                Core::StencilOperation::Keep,
                Core::StencilOperation::Keep,
                Core::StencilOperation::Keep
            );

            renderSkybox(skybox);

        }

    }

    void Basic3DRenderPipeline::clearBuffers() {

        gaunlet::Core::RenderCommand::clearColorBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        gaunlet::Core::RenderCommand::clearDepthStencilBuffer();

    }

    void Basic3DRenderPipeline::startScene(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera, const Core::Ref<Scene::DirectionalLightComponent>& directionalLight) {

        SceneProperties sceneProperties(
            camera->getViewMatrix(), camera->getProjectionMatrix(),
            directionalLight->m_color, directionalLight->m_direction,
            directionalLight->m_ambientIntensity, directionalLight->m_diffuseIntensity
        );

        unsigned int viewportX0, viewportY0, viewportX1, viewportY1;
        Core::RenderCommand::getViewport(
            viewportX0, viewportY0, viewportX1, viewportY1
        );

        sceneProperties.m_viewport = {viewportX0, viewportY0, viewportX1, viewportY1};

        m_scenePropertiesUniformBuffer->setData(
            (const void*) &sceneProperties,
            sizeof(SceneProperties)
        );

    }

    void Basic3DRenderPipeline::renderModels(const Core::Ref<Scene::Scene> &scene) {

        auto group = scene->getRegistry().group<Scene::ModelComponent>(entt::get<Scene::TransformComponent>);
        for (auto e : group) {
            m_modelRenderer.submitObject(
                {e, scene},
                m_modelRenderer.getShaders().get("model-faces")
            );
        }
        m_modelRenderer.renderObjects(m_modelRenderer.getShaders().get("model-faces"));

    }

    void Basic3DRenderPipeline::renderCircles(const Core::Ref<Scene::Scene> &scene) {

        auto group = scene->getRegistry().group<Scene::CircleComponent>(entt::get<Scene::TransformComponent>);
        for (auto e : group) {
            m_circleRenderer.submitObject(
                {e, scene},
                m_circleRenderer.getShaders().get("circle-faces")
            );
        }
        m_circleRenderer.renderObjects(m_circleRenderer.getShaders().get("circle-faces"));

    }

    void Basic3DRenderPipeline::renderSkybox(const Core::Ref<Scene::SkyboxComponent>& skybox) {

        if (skybox->m_cubeMap) {
            m_skyboxRenderer.render(
                skybox->m_cubeMap,
                m_skyboxRenderer.getShaders().get("skybox")
            );
        }

    }

    void Basic3DRenderPipeline::prepareShaders() {

        // Create the Uniform Buffer for the Scene Properties, which will be linked to every shader
        m_scenePropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "SceneProperties",
            0,
            sizeof (SceneProperties)
        );

        m_modelRenderer.getShaders().get("model-faces")->linkUniformBuffer(m_scenePropertiesUniformBuffer);
        m_circleRenderer.getShaders().get("circle-faces")->linkUniformBuffer(m_scenePropertiesUniformBuffer);
        m_skyboxRenderer.getShaders().get("skybox")->linkUniformBuffer(m_scenePropertiesUniformBuffer);

    }

}