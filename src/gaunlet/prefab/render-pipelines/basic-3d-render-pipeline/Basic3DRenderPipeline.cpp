#include "gaunlet/prefab/render-pipelines/basic-3d-render-pipeline/Basic3DRenderPipeline.h"
#include "gaunlet/prefab/render-pipelines/basic-3d-render-pipeline/PropertySets.h"
#include "gaunlet/graphics/renderer/DirectRenderer.h"
#include "gaunlet/prefab/sprites/Sprites.h"

namespace gaunlet::Prefab::Basic3DRenderPipeline {

    Basic3DRenderPipeline::Basic3DRenderPipeline() {
        loadShaders();
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

        submitModels(scene);
        submitCircles(scene);

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

    void Basic3DRenderPipeline::startScene(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera, const Core::Ref<Scene::DirectionalLightComponent>& directionalLight) {

        SceneProperties sceneProperties(
            camera->getViewMatrix(), camera->getProjectionMatrix(),
            directionalLight->m_color, directionalLight->m_direction,
            directionalLight->m_ambientIntensity, directionalLight->m_diffuseIntensity
        );

        m_scenePropertiesUniformBuffer->setData(
            (const void*) &sceneProperties,
            sizeof(SceneProperties)
        );

    }

    void Basic3DRenderPipeline::renderSkybox(const Core::Ref<Scene::SkyboxComponent>& skybox) {

        Prefab::Sprites::CubeModel cube;

        Graphics::DirectRenderer::renderIndexedTriangles(
            cube.getMeshes()[0].getVertices(),
            cube.getMeshes()[0].getIndices(),
            skybox->m_cubeMap,
            getShaderLibrary().get("skybox")
        );

    }

    void Basic3DRenderPipeline::loadShaders() {

        // Create the Uniform Buffer for the Scene Properties, which will be linked to every shader
        m_scenePropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "ScenePropertiesBlock",
            0,
            sizeof (SceneProperties)
        );

        loadModelShaders();
        loadCircleShaders();
        loadSkyboxShaders();

    }

    void Basic3DRenderPipeline::loadModelShaders() {

        // Polygon Shader
        std::map<Core::ShaderType, std::string> modelShaderSources {
            {Core::ShaderType::Vertex, PREFABS_PATH"/render-pipelines/basic-3d-render-pipeline/shaders/model/vertex.glsl"},
            {Core::ShaderType::Fragment, PREFABS_PATH"/render-pipelines/basic-3d-render-pipeline/shaders/model/fragment.glsl"}
        };
        auto modelShader = m_shaderLibrary.load("model", modelShaderSources);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_modelRenderer.getMaxTextures(); i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            modelShader->setUniform1i(textureName, i);
        }

        // Link the SceneProperties and EntityProperties uniform buffers to the polygon shader
        modelShader->linkUniformBuffer(m_modelRenderer.getPropertySetsUniformBuffer());
        modelShader->linkUniformBuffer(m_scenePropertiesUniformBuffer);

    }

    void Basic3DRenderPipeline::loadCircleShaders() {

        // Polygon Shader
        std::map<Core::ShaderType, std::string> circleShaderSources {
            {Core::ShaderType::Vertex, PREFABS_PATH"/render-pipelines/basic-3d-render-pipeline/shaders/circle/vertex.glsl"},
            {Core::ShaderType::Fragment, PREFABS_PATH"/render-pipelines/basic-3d-render-pipeline/shaders/circle/fragment.glsl"}
        };
        auto circleShader = m_shaderLibrary.load("circle", circleShaderSources);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_circleRenderer.getMaxTextures(); i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            circleShader->setUniform1i(textureName, i);
        }

        // Link the SceneProperties and EntityProperties uniform buffers to the polygon shader
        circleShader->linkUniformBuffer(m_circleRenderer.getPropertySetsUniformBuffer());
        circleShader->linkUniformBuffer(m_scenePropertiesUniformBuffer);

    }

    void Basic3DRenderPipeline::loadSkyboxShaders() {

        std::map<Core::ShaderType, std::string> skyboxShaderSource {
            {Core::ShaderType::Vertex, PREFABS_PATH"/render-pipelines/basic-3d-render-pipeline/shaders/skybox/vertex.glsl"},
            {Core::ShaderType::Fragment, PREFABS_PATH"/render-pipelines/basic-3d-render-pipeline/shaders/skybox/fragment.glsl"}
        };
        auto skyboxShader = m_shaderLibrary.load("skybox", skyboxShaderSource);

        // Set a single "skybox" texture
        skyboxShader->setUniform1i("skybox", 0);

        // Link uniform buffers
        skyboxShader->linkUniformBuffer(m_scenePropertiesUniformBuffer);

    }

}