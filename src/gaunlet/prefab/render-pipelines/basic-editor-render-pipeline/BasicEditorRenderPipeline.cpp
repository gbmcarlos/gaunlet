#include "gaunlet/prefab/render-pipelines/basic-editor-render-pipeline/BasicEditorRenderPipeline.h"
#include "gaunlet/prefab/render-pipelines/basic-3d-render-pipeline/PropertySets.h"
#include "gaunlet/core/window/Window.h"
#include "gaunlet/editor/Tags.h"

namespace gaunlet::Prefab::BasicEditorRenderPipeline {

    BasicEditorRenderPipeline::BasicEditorRenderPipeline()
        : m_modelRenderer({"EntityPropertiesBlock", 1}), m_circleRenderer("EntityPropertiesBlock", 2) {

        loadShaders();

        auto window = Core::Window::getCurrentInstance();

        m_framebuffer = gaunlet::Core::CreateRef<gaunlet::Graphics::Framebuffer>(std::initializer_list<gaunlet::Graphics::FramebufferAttachmentSpec>{
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::RGBA, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)},
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::Integer, -1},
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::Integer, -1},
            {gaunlet::Core::FramebufferAttachmentType::DepthStencil, gaunlet::Graphics::FramebufferDataFormat::DepthStencil}
        }, window->getViewportWidth() * window->getDPI(), window->getViewportHeight() * window->getDPI());

    }

    void BasicEditorRenderPipeline::run(const Core::Ref<Scene::Scene> &scene, const Core::Ref<Scene::Camera> &camera, const Core::Ref<Scene::DirectionalLightComponent> &directionalLight, const Core::Ref<Scene::SkyboxComponent> &skybox) {

        // Start scene doesn't do any drawing, it just sets the SceneProperties uniform buffer
        startScene(scene, camera, directionalLight ? directionalLight : Core::CreateRef<Scene::DirectionalLightComponent>());

        // The framebuffer needs to be bound before we start drawing anything
        m_framebuffer->bind();

        drawScene(scene);
        drawOutlines(scene);
        drawUI(scene);
        drawSkybox(skybox);

        m_framebuffer->unbind();

    }

    void BasicEditorRenderPipeline::resize(unsigned int width, unsigned int height) {

        m_framebuffer->resize(
            width,
            height
        );

    }

    const Core::Ref<Graphics::Texture>& BasicEditorRenderPipeline::getRenderedTexture() {
        return m_framebuffer->getColorAttachment(SceneFramebufferAttachmentIndex);
    }

    int BasicEditorRenderPipeline::readFramebuffer(Editor::FramebufferLayer layer, unsigned int x, unsigned int y) {

        unsigned int attachmentIndex;

        switch (layer) {
            case Editor::FramebufferLayer::SceneEntity:
                attachmentIndex = SceneEntityIdFramebufferAttachmentIndex; break;
            case Editor::FramebufferLayer::UIEntity:
                attachmentIndex = UIEntityIdFramebufferAttachmentIndex; break;
        }

        return m_framebuffer->readPixel(attachmentIndex, x, y);

    }

    void BasicEditorRenderPipeline::drawScene(const Core::Ref<Scene::Scene>& scene) {

        // Set the draw buffers in the right order
        m_framebuffer->setDrawBuffers({
            SceneFramebufferAttachmentIndex,
            SceneEntityIdFramebufferAttachmentIndex,
            UIEntityIdFramebufferAttachmentIndex
        });

        // Clear everything
        m_framebuffer->clear();

        // Normal depth testing
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
            Core::StencilOperation::Replace
        );

        // Then draw the objects
        submitSceneModels(scene);
        submitSceneCircles(scene);

    }

    void BasicEditorRenderPipeline::drawOutlines(const Core::Ref<Scene::Scene> &scene) {

        // Draw only wherever we didn't draw anything before
        Core::RenderCommand::setStencilFunction(
            Core::DepthStencilFunction::NotEqual, 1
        );

        submitOutlines(scene);

    }

    void BasicEditorRenderPipeline::drawUI(const Core::Ref<Scene::Scene>& scene) {

        // Switch the order of the draw buffers (to write the ui entity id)
        m_framebuffer->setDrawBuffers({
            SceneFramebufferAttachmentIndex,
            UIEntityIdFramebufferAttachmentIndex,
            SceneEntityIdFramebufferAttachmentIndex
        });

        // Ignore depth (draw on top of everything)
        Core::RenderCommand::setDepthFunction(Core::DepthStencilFunction::Always);

        // Ignore stencil (always draw), and keep writing 1s
        Core::RenderCommand::setStencilFunction(Core::DepthStencilFunction::Always, 1);

        // Then draw the objects
        submitUIModels(scene);
        submitUICircles(scene);

    }

    void BasicEditorRenderPipeline::drawSkybox(const Core::Ref<Scene::SkyboxComponent>& skybox) {

        if (!skybox->m_cubeMap) {
            return;
        }

        // We don't care about depth
        Core::RenderCommand::setDepthFunction(Core::DepthStencilFunction::Always);

        // Draw only wherever we didn't draw anything before
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

    void BasicEditorRenderPipeline::submitSceneModels(const Core::Ref<Scene::Scene>& scene) {

        // Model faces: those models that don't have the Wireframe tag
        auto facesView = scene->getRegistry().view<Scene::ModelComponent, Scene::TransformComponent, Editor::SceneEntityTag>(entt::exclude<Editor::WireframeModelTag>);
        for (auto e : facesView) {
            m_modelRenderer.submitObject(
                {e, scene.get()},
                getShaderLibrary().get("model-faces")
            );
        }
        m_modelRenderer.renderObjects(getShaderLibrary().get("model-faces"));

        // Model wireframes: those models that have the Wireframe tag
        auto wireframesView = scene->getRegistry().view<Scene::ModelComponent, Scene::TransformComponent, Editor::SceneEntityTag, Editor::WireframeModelTag>();
        for (auto e : wireframesView) {
            m_modelRenderer.submitObject(
                {e, scene.get()},
                getShaderLibrary().get("model-wireframe")
            );
        }
        m_modelRenderer.renderObjects(getShaderLibrary().get("model-wireframe"));

    }

    void BasicEditorRenderPipeline::submitSceneCircles(const Core::Ref<Scene::Scene>& scene) {

        auto view = scene->getRegistry().view<Scene::CircleComponent, Scene::TransformComponent, Editor::SceneEntityTag>();
        for (auto e : view) {
            m_circleRenderer.submitObject(
                {e, scene.get()},
                getShaderLibrary().get("circle")
            );
        }
        m_circleRenderer.renderObjects(getShaderLibrary().get("circle"));

    }

    void BasicEditorRenderPipeline::submitOutlines(const Core::Ref<Scene::Scene> &scene) {

        // Model wireframes: those models that have the Wireframe tag
        auto view = scene->getRegistry().view<Scene::ModelComponent, Scene::TransformComponent, Editor::SceneEntityTag, Editor::ModelOutlineTag>();
        for (auto e : view) {
            m_modelRenderer.submitObject(
                {e, scene.get()},
                getShaderLibrary().get("model-outline")
            );
        }
        m_modelRenderer.renderObjects(getShaderLibrary().get("model-outline"));

    }

    void BasicEditorRenderPipeline::submitUIModels(const Core::Ref<Scene::Scene>& scene) {

        auto group = scene->getRegistry().group<Scene::ModelComponent>(entt::get<Scene::TransformComponent, Editor::UIEntityTag>);
        for (auto e : group) {
            m_modelRenderer.submitObject(
                {e, scene.get()},
                getShaderLibrary().get("model-faces")
            );
        }
        m_modelRenderer.renderObjects(getShaderLibrary().get("model-faces"));

    }

    void BasicEditorRenderPipeline::submitUICircles(const Core::Ref<Scene::Scene>& scene) {

        auto group = scene->getRegistry().group<Scene::CircleComponent>(entt::get<Scene::TransformComponent, Editor::UIEntityTag>);
        for (auto e : group) {
            m_circleRenderer.submitObject(
                {e, scene.get()},
                getShaderLibrary().get("circle")
            );
        }
        m_circleRenderer.renderObjects(getShaderLibrary().get("circle"));

    }

    void BasicEditorRenderPipeline::loadShaders() {

        // Create the Uniform Buffer for the Scene Properties, which will be linked to every shader
        m_scenePropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "ScenePropertiesBlock",
            0,
            sizeof (Prefab::Basic3DRenderPipeline::SceneProperties)
        );

        loadModelShaders();
        loadCircleShaders();
        loadSkyboxShaders();
        loadModelOutlineShaders();

    }

    void BasicEditorRenderPipeline::loadModelShaders() {

        // Model faces shader
        std::map<Core::ShaderType, std::string> modelFacesShaderSources {
            {Core::ShaderType::Vertex, PREFABS_PATH"/render-pipelines/basic-editor-render-pipeline/shaders/model-faces/vertex.glsl"},
            {Core::ShaderType::Fragment, PREFABS_PATH"/render-pipelines/basic-editor-render-pipeline/shaders/model-faces/fragment.glsl"}
        };
        auto modelFacesShader = m_shaderLibrary.load("model-faces", modelFacesShaderSources);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_modelRenderer.getMaxTextures(); i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            modelFacesShader->setUniform1i(textureName, i);
        }

        // Link the SceneProperties and EntityProperties uniform buffers to the polygon shader
        modelFacesShader->linkUniformBuffer(m_modelRenderer.getPropertySetsUniformBuffer());
        modelFacesShader->linkUniformBuffer(m_scenePropertiesUniformBuffer);

        // Model wireframe shader
        std::map<Core::ShaderType, std::string> modelWireframeShaderSources {
            {Core::ShaderType::Vertex, PREFABS_PATH"/render-pipelines/basic-editor-render-pipeline/shaders/model-wireframe/vertex.glsl"},
            {Core::ShaderType::Geometry, PREFABS_PATH"/render-pipelines/basic-editor-render-pipeline/shaders/model-wireframe/geometry.glsl"},
            {Core::ShaderType::Fragment, PREFABS_PATH"/render-pipelines/basic-editor-render-pipeline/shaders/model-wireframe/fragment.glsl"}
        };
        auto modelWireframeShader = m_shaderLibrary.load("model-wireframe", modelWireframeShaderSources);

        // Set all the texture slots as uniforms
        for (int i = 0; i < m_modelRenderer.getMaxTextures(); i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            modelWireframeShader->setUniform1i(textureName, i);
        }

        // Link the SceneProperties and EntityProperties uniform buffers to the polygon shader
        modelWireframeShader->linkUniformBuffer(m_modelRenderer.getPropertySetsUniformBuffer());
        modelWireframeShader->linkUniformBuffer(m_scenePropertiesUniformBuffer);

    }

    void BasicEditorRenderPipeline::loadCircleShaders() {

        // Polygon Shader
        std::map<Core::ShaderType, std::string> circleShaderSources {
            {Core::ShaderType::Vertex, PREFABS_PATH"/render-pipelines/basic-editor-render-pipeline/shaders/circle/vertex.glsl"},
            {Core::ShaderType::Fragment, PREFABS_PATH"/render-pipelines/basic-editor-render-pipeline/shaders/circle/fragment.glsl"}
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

    void BasicEditorRenderPipeline::loadSkyboxShaders() {

        std::map<Core::ShaderType, std::string> skyboxShaderSource {
            {Core::ShaderType::Vertex, PREFABS_PATH"/render-pipelines/basic-editor-render-pipeline/shaders/skybox/vertex.glsl"},
            {Core::ShaderType::Fragment, PREFABS_PATH"/render-pipelines/basic-editor-render-pipeline/shaders/skybox/fragment.glsl"}
        };
        auto skyboxShader = m_shaderLibrary.load("skybox", skyboxShaderSource);

        // Set a single "skybox" texture
        skyboxShader->setUniform1i("skybox", 0);

        // Link uniform buffers
        skyboxShader->linkUniformBuffer(m_scenePropertiesUniformBuffer);

    }

    void BasicEditorRenderPipeline::loadModelOutlineShaders() {

        std::map<Core::ShaderType, std::string> modelOutlineShaderSource {
            {Core::ShaderType::Vertex, PREFABS_PATH"/render-pipelines/basic-editor-render-pipeline/shaders/model-outline/vertex.glsl"},
            {Core::ShaderType::Fragment, PREFABS_PATH"/render-pipelines/basic-editor-render-pipeline/shaders/model-outline/fragment.glsl"}
        };
        auto modelOutlineShader = m_shaderLibrary.load("model-outline", modelOutlineShaderSource);

        // Set a single "skybox" texture
        modelOutlineShader->setUniform1f("u_width", 0.1f);

        // Link uniform buffers
        modelOutlineShader->linkUniformBuffer(m_scenePropertiesUniformBuffer);
        modelOutlineShader->linkUniformBuffer(m_modelRenderer.getPropertySetsUniformBuffer());

    }

}