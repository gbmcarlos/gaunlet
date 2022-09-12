#include "gaunlet/prefab/render-pipelines/basic-2d-render-pipeline/Basic2DRenderPipeline.h"
#include "gaunlet/core/render/RenderCommand.h"

namespace gaunlet::Prefab::Basic2DRenderPipeline {

    Basic2DRenderPipeline::Basic2DRenderPipeline()
        : m_modelRenderer({"EntityPropertiesBlock", 1}), m_circleRenderer("EntityPropertiesBlock", 2) {
        loadShaders();
    }

    void Basic2DRenderPipeline::run(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera) {

        clearBuffers();
        startScene(scene, camera);
        submitModels(scene);
        submitCircles(scene);

    }

    void Basic2DRenderPipeline::clearBuffers() {

        gaunlet::Core::RenderCommand::clearColorBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        gaunlet::Core::RenderCommand::clearDepthStencilBuffer();

    }

    void Basic2DRenderPipeline::startScene(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera) {

        SceneProperties sceneProperties(
            camera->getViewMatrix(), camera->getProjectionMatrix()
        );

        m_scenePropertiesUniformBuffer->setData(
            (const void*) &sceneProperties,
            sizeof(SceneProperties)
        );

    }

    void Basic2DRenderPipeline::submitModels(const Core::Ref<Scene::Scene>& scene) {

        auto group = scene->getRegistry().group<Scene::ModelComponent>(entt::get<Scene::TransformComponent>);
        for (auto e : group) {
            m_modelRenderer.submitObject(
                {e, scene.get()},
                getShaderLibrary().get("model")
            );
        }
        m_modelRenderer.renderObjects(getShaderLibrary().get("model"));

    }

    void Basic2DRenderPipeline::submitCircles(const Core::Ref<Scene::Scene>& scene) {

        auto group = scene->getRegistry().group<Scene::CircleComponent>(entt::get<Scene::TransformComponent>);
        for (auto e : group) {
            m_circleRenderer.submitObject(
                {e, scene.get()},
                getShaderLibrary().get("circle")
            );
        }
        m_circleRenderer.renderObjects(getShaderLibrary().get("circle"));

    }

    void Basic2DRenderPipeline::loadShaders() {

        // Create the Uniform Buffer for the Scene Properties, which will be linked to every shader
        m_scenePropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "ScenePropertiesBlock",
            0,
            sizeof (SceneProperties)
        );

        loadModelShaders();
        loadCircleShaders();

    }

    void Basic2DRenderPipeline::loadModelShaders() {

        // Polygon Shader
        std::map<Core::ShaderType, std::string> modelShaderSources {
            {Core::ShaderType::Vertex, PREFABS_PATH"/render-pipelines/basic-2d-render-pipeline/shaders/model/vertex.glsl"},
            {Core::ShaderType::Fragment, PREFABS_PATH"/render-pipelines/basic-2d-render-pipeline/shaders/model/fragment.glsl"}
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

    void Basic2DRenderPipeline::loadCircleShaders() {

        // Polygon Shader
        std::map<Core::ShaderType, std::string> circleShaderSources {
            {Core::ShaderType::Vertex, PREFABS_PATH"/render-pipelines/basic-2d-render-pipeline/shaders/circle/vertex.glsl"},
            {Core::ShaderType::Fragment, PREFABS_PATH"/render-pipelines/basic-2d-render-pipeline/shaders/circle/fragment.glsl"}
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

}