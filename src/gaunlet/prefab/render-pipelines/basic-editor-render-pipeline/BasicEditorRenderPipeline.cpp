#include "gaunlet/prefab/render-pipelines/basic-editor-render-pipeline/BasicEditorRenderPipeline.h"

#include "gaunlet/prefab/render-pipelines/basic-editor-render-pipeline/PropertySets.h"
#include "gaunlet/core/window/Window.h"
#include "gaunlet/editor/Tags.h"

namespace gaunlet::Prefab::BasicEditorRenderPipeline {

    BasicEditorRenderPipeline::BasicEditorRenderPipeline()
        : m_modelRenderer(1), m_circleRenderer(2), m_planeRenderer(3) {

        prepareShaders();

        auto window = Core::Window::getCurrentInstance();

        m_framebuffer = gaunlet::Core::CreateRef<gaunlet::Graphics::Framebuffer>(std::initializer_list<gaunlet::Graphics::FramebufferAttachmentSpec>{
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::RGBA, glm::vec4(0, 0, 0, 1)},
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::Integer, -1},
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::Integer, -1},
            {gaunlet::Core::FramebufferAttachmentType::DepthStencil, gaunlet::Graphics::FramebufferDataFormat::DepthStencil}
        }, window->getViewportWidth() * window->getDPI(), window->getViewportHeight() * window->getDPI());

    }

    void BasicEditorRenderPipeline::run(const Core::Ref<Scene::Scene> &scene, const Core::Ref<Scene::Camera> &camera, const Core::Ref<Scene::DirectionalLightComponent> &directionalLight, const Core::Ref<Scene::SkyboxComponent> &skybox) {

        // Set the draw buffers in the right order
        m_framebuffer->setDrawBuffers({
            SceneFramebufferAttachmentIndex,
            SceneEntityIdFramebufferAttachmentIndex,
            UIEntityIdFramebufferAttachmentIndex
        });

        // Clear everything
        m_framebuffer->clear();

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

    void BasicEditorRenderPipeline::startScene(const Core::Ref<Scene::Scene> &scene, const Core::Ref<Scene::Camera> &camera, const Core::Ref<Scene::DirectionalLightComponent> &directionalLight) {

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

    void BasicEditorRenderPipeline::drawScene(const Core::Ref<Scene::Scene>& scene) {

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
        renderSceneModels(scene);
        renderSceneCircles(scene);
        submitScenePlanes(scene);

    }

    void BasicEditorRenderPipeline::drawOutlines(const Core::Ref<Scene::Scene> &scene) {

        // Draw only wherever we didn't draw anything before
        Core::RenderCommand::setStencilFunction(
            Core::DepthStencilFunction::NotEqual, 1
        );

        renderOutlines(scene);

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
        renderUIModels(scene);
        renderUICircles(scene);

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

    void BasicEditorRenderPipeline::renderSceneModels(const Core::Ref<Scene::Scene>& scene) {

        // Model faces: those models that don't have the Wireframe tag
        auto facesView = scene->getRegistry().view<Scene::ModelComponent, Scene::TransformComponent, Editor::SceneEntityTag>(entt::exclude<Editor::WireframeModelTag>);
        for (auto e : facesView) {
            m_modelRenderer.submitObject(
                {e, scene.get()},
                m_modelRenderer.getShaders().get("model-faces")
            );
        }
        m_modelRenderer.renderObjects(m_modelRenderer.getShaders().get("model-faces"));

        // Model wireframes: those models that have the Wireframe tag
        auto wireframesView = scene->getRegistry().view<Scene::ModelComponent, Scene::TransformComponent, Editor::SceneEntityTag, Editor::WireframeModelTag>();
        for (auto e : wireframesView) {
            m_modelRenderer.submitObject(
                {e, scene.get()},
                m_modelRenderer.getShaders().get("model-wireframe")
            );
        }
        m_modelRenderer.renderObjects(m_modelRenderer.getShaders().get("model-wireframe"));

    }

    void BasicEditorRenderPipeline::renderSceneCircles(const Core::Ref<Scene::Scene>& scene) {

        auto view = scene->getRegistry().view<Scene::CircleComponent, Scene::TransformComponent, Editor::SceneEntityTag>();
        for (auto e : view) {
            m_circleRenderer.submitObject(
                {e, scene.get()},
                m_circleRenderer.getShaders().get("circle-faces")
            );
        }
        m_circleRenderer.renderObjects(m_circleRenderer.getShaders().get("circle-faces"));

    }

    void BasicEditorRenderPipeline::submitScenePlanes(const Core::Ref<Scene::Scene> &scene) {

        // Model faces: those models that don't have the Wireframe tag
        auto facesView = scene->getRegistry().view<Scene::PlaneComponent, Scene::TransformComponent, Editor::SceneEntityTag>(entt::exclude<Editor::WireframeModelTag>);
        for (auto e : facesView) {
            m_planeRenderer.submitObject(
                {e, scene.get()},
                m_planeRenderer.getShaders().get("plane-faces")
            );
        }
        m_planeRenderer.renderObjects(m_planeRenderer.getShaders().get("plane-faces"));

        // Model wireframes: those models that have the Wireframe tag
        auto wireframesView = scene->getRegistry().view<Scene::PlaneComponent, Scene::TransformComponent, Editor::SceneEntityTag, Editor::WireframeModelTag>();
        for (auto e : wireframesView) {
            m_planeRenderer.submitObject(
                {e, scene.get()},
                m_planeRenderer.getShaders().get("plane-wireframe")
            );
        }
        m_planeRenderer.renderObjects(m_planeRenderer.getShaders().get("plane-wireframe"));

    }

    void BasicEditorRenderPipeline::renderOutlines(const Core::Ref<Scene::Scene> &scene) {

        // Model wireframes: those models that have the Wireframe tag
        auto view = scene->getRegistry().view<Scene::ModelComponent, Scene::TransformComponent, Editor::SceneEntityTag, Editor::ModelOutlineTag>();
        for (auto e : view) {
            m_modelRenderer.submitObject(
                {e, scene.get()},
                m_modelRenderer.getShaders().get("model-outline")
            );
        }
        m_modelRenderer.renderObjects(m_modelRenderer.getShaders().get("model-outline"));

    }

    void BasicEditorRenderPipeline::renderUIModels(const Core::Ref<Scene::Scene>& scene) {

        auto group = scene->getRegistry().group<Scene::ModelComponent>(entt::get<Scene::TransformComponent, Editor::UIEntityTag>);
        for (auto e : group) {
            m_modelRenderer.submitObject(
                {e, scene.get()},
                m_modelRenderer.getShaders().get("model-faces")
            );
        }
        m_modelRenderer.renderObjects(m_modelRenderer.getShaders().get("model-faces"));

    }

    void BasicEditorRenderPipeline::renderUICircles(const Core::Ref<Scene::Scene>& scene) {

        auto group = scene->getRegistry().group<Scene::CircleComponent>(entt::get<Scene::TransformComponent, Editor::UIEntityTag>);
        for (auto e : group) {
            m_circleRenderer.submitObject(
                {e, scene.get()},
                m_circleRenderer.getShaders().get("circle-faces")
            );
        }
        m_circleRenderer.renderObjects(m_circleRenderer.getShaders().get("circle-faces"));

    }

    void BasicEditorRenderPipeline::renderSkybox(const Core::Ref<Scene::SkyboxComponent> &skybox) {

        if (skybox->m_cubeMap) {
            m_skyboxRenderer.render(
                skybox->m_cubeMap,
                m_skyboxRenderer.getShaders().get("skybox")
            );
        }

    }

    void BasicEditorRenderPipeline::prepareShaders() {

        // Create the Uniform Buffer for the Scene Properties, which will be linked to every shader
        m_scenePropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "SceneProperties",
            0,
            sizeof (SceneProperties)
        );

        m_modelRenderer.getShaders().get("model-faces")->linkUniformBuffer(m_scenePropertiesUniformBuffer);
        m_modelRenderer.getShaders().get("model-wireframe")->linkUniformBuffer(m_scenePropertiesUniformBuffer);

        m_modelRenderer.getShaders().get("model-outline")
            ->linkUniformBuffer(m_scenePropertiesUniformBuffer)
            ->setUniform1f("u_width", 0.015f)
            ->setUniform3f("u_color", {0.9f, 0.9f, 0.1f});

        m_circleRenderer.getShaders().get("circle-faces")->linkUniformBuffer(m_scenePropertiesUniformBuffer);
        m_skyboxRenderer.getShaders().get("skybox")->linkUniformBuffer(m_scenePropertiesUniformBuffer);

    }

}