#include "gaunlet/prefab/render-pipelines/basic-editor-render-pipeline/BasicEditorRenderPipeline.h"

#include "gaunlet/graphics/framebuffer/attachment-specs/ColorAttachmentSpecFactory.h"
#include "gaunlet/graphics/framebuffer/attachment-specs/BaseDepthStencilAttachmentSpec.h"
#include "gaunlet/prefab/render-pipelines/SceneProperties.h"
#include "gaunlet/core/window/Window.h"
#include "gaunlet/editor/Tags.h"
#include "gaunlet/prefab/render-pipeline-extensions/EntitySelectionExtension.h"

namespace gaunlet::Prefab::RenderPipelines {

    BasicEditorRenderPipeline::BasicEditorRenderPipeline(Core::Ref<Scene::DirectionalLightComponent> directionalLight, Core::Ref<Scene::SkyboxComponent> skybox, unsigned int uniformBufferBindingPointOffset)
        : m_directionalLight(std::move(directionalLight)), m_skybox(std::move(skybox)), m_modelRenderer(1 + uniformBufferBindingPointOffset), m_circleRenderer(2 + uniformBufferBindingPointOffset) {

        prepareShaders(uniformBufferBindingPointOffset);
        prepareFramebuffer();

        addExtension<RenderPipelineExtensions::EntitySelectionExtension>(Core::CreateRef<RenderPipelineExtensions::EntitySelectionExtension>(
            m_framebuffer,
            1, 2
        ));

    }

    void BasicEditorRenderPipeline::run(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera) {

        // Set the draw buffers in the right order
        m_framebuffer->setDrawBuffers({
            SceneFramebufferAttachmentIndex,
            SceneEntityIdFramebufferAttachmentIndex,
            UIEntityIdFramebufferAttachmentIndex
        });

        // Clear everything
        m_framebuffer->clear();

        // Start scene doesn't do any drawing, it just sets the SceneProperties uniform buffer
        startScene(scene, camera, m_directionalLight ? m_directionalLight : Core::CreateRef<Scene::DirectionalLightComponent>());

        // The framebuffer needs to be bound before we start drawing anything
        m_framebuffer->bind();

        drawScene(scene);
        drawOutlines(scene);
        drawUI(scene);
        drawSkybox(m_skybox);

        m_framebuffer->unbind();

    }

    void BasicEditorRenderPipeline::resize(unsigned int width, unsigned int height) {

        m_framebuffer->resize(
            width,
            height
        );

    }

    const Core::Ref<Graphics::TextureImage2D>& BasicEditorRenderPipeline::getRenderTarget() {
        return m_framebuffer->getColorAttachment(SceneFramebufferAttachmentIndex);
    }

    unsigned int BasicEditorRenderPipeline::getUniformBufferCount() {
        return
            ObjectRenderers::ModelRenderer::getUniformBufferCount() +
            ObjectRenderers::CircleRenderer::getUniformBufferCount() +
            ObjectRenderers::SkyboxRenderer::getUniformBufferCount() +
            1; // The SceneProperties uniform buffer that this render pipeline manages itself
    }

    void BasicEditorRenderPipeline::startScene(const Core::Ref<Scene::Scene> &scene, const Core::Ref<Scene::Camera> &camera, const Core::Ref<Scene::DirectionalLightComponent> &directionalLight) {

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

        if (skybox == nullptr || !skybox->m_cubeMap) {
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
        auto facesView = scene->getRegistry().view<Prefab::Components::ModelComponent, Scene::TransformComponent, Editor::SceneEntityTag>(entt::exclude<Editor::WireframeModelTag>);
        for (auto e : facesView) {
            m_modelRenderer.submitObject(
                {e, scene},
                m_modelRenderer.getShaders().get("model-faces")
            );
        }
        m_modelRenderer.renderObjects(m_modelRenderer.getShaders().get("model-faces"));

        // Model wireframes: those models that have the Wireframe tag
        auto wireframesView = scene->getRegistry().view<Prefab::Components::ModelComponent, Scene::TransformComponent, Editor::SceneEntityTag, Editor::WireframeModelTag>();
        for (auto e : wireframesView) {
            m_modelRenderer.submitObject(
                {e, scene},
                m_modelRenderer.getShaders().get("model-wireframe")
            );
        }
        m_modelRenderer.renderObjects(m_modelRenderer.getShaders().get("model-wireframe"));

    }

    void BasicEditorRenderPipeline::renderSceneCircles(const Core::Ref<Scene::Scene>& scene) {

        auto view = scene->getRegistry().view<Prefab::Components::CircleComponent, Scene::TransformComponent, Editor::SceneEntityTag>();
        for (auto e : view) {
            m_circleRenderer.submitObject(
                {e, scene},
                m_circleRenderer.getShaders().get("circle-faces")
            );
        }
        m_circleRenderer.renderObjects(m_circleRenderer.getShaders().get("circle-faces"));

    }

    void BasicEditorRenderPipeline::renderOutlines(const Core::Ref<Scene::Scene> &scene) {

        // Model wireframes: those models that have the Wireframe tag
        auto view = scene->getRegistry().view<Prefab::Components::ModelComponent, Scene::TransformComponent, Editor::SceneEntityTag, Editor::ModelOutlineTag>();
        for (auto e : view) {
            m_modelRenderer.submitObject(
                {e, scene},
                m_modelRenderer.getShaders().get("model-outline")
            );
        }
        m_modelRenderer.renderObjects(m_modelRenderer.getShaders().get("model-outline"));

    }

    void BasicEditorRenderPipeline::renderUIModels(const Core::Ref<Scene::Scene>& scene) {

        auto group = scene->getRegistry().group<Prefab::Components::ModelComponent>(entt::get<Scene::TransformComponent, Editor::UIEntityTag>);
        for (auto e : group) {
            m_modelRenderer.submitObject(
                {e, scene},
                m_modelRenderer.getShaders().get("model-faces")
            );
        }
        m_modelRenderer.renderObjects(m_modelRenderer.getShaders().get("model-faces"));

    }

    void BasicEditorRenderPipeline::renderUICircles(const Core::Ref<Scene::Scene>& scene) {

        auto group = scene->getRegistry().group<Prefab::Components::CircleComponent>(entt::get<Scene::TransformComponent, Editor::UIEntityTag>);
        for (auto e : group) {
            m_circleRenderer.submitObject(
                {e, scene},
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

    void BasicEditorRenderPipeline::prepareShaders(unsigned int uniformBufferBindingPointOffset) {

        // Create the Uniform Buffer for the Scene Properties, which will be linked to every shader
        m_scenePropertiesUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            "SceneProperties",
            0 + uniformBufferBindingPointOffset,
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

    void BasicEditorRenderPipeline::prepareFramebuffer() {

        auto window = Core::Window::getCurrentInstance();

        m_framebuffer = gaunlet::Core::CreateRef<gaunlet::Graphics::Framebuffer>(
            window->getViewportWidth() * window->getDPI(),
            window->getViewportHeight() * window->getDPI()
        );

        // The render target
        m_framebuffer->addColorAttachment<glm::vec4>(
            gaunlet::Graphics::BaseColorAttachmentSpec::Channels::CHANNELS_4,
            gaunlet::Graphics::BaseColorAttachmentSpec::Type::TYPE_UNI,
            gaunlet::Graphics::BaseColorAttachmentSpec::Size::SIZE_8,
            glm::vec4(0.1f, 0.1f, 0.1f, 1)
        );

        // For scene entity ids
        m_framebuffer->addColorAttachment<int>(
            gaunlet::Graphics::BaseColorAttachmentSpec::Channels::CHANNELS_1,
            gaunlet::Graphics::BaseColorAttachmentSpec::Type::TYPE_SI,
            gaunlet::Graphics::BaseColorAttachmentSpec::Size::SIZE_32,
            -1
        );

        // For UI entity ids
        m_framebuffer->addColorAttachment<int>(
            gaunlet::Graphics::BaseColorAttachmentSpec::Channels::CHANNELS_1,
            gaunlet::Graphics::BaseColorAttachmentSpec::Type::TYPE_SI,
            gaunlet::Graphics::BaseColorAttachmentSpec::Size::SIZE_32,
            -1
        );

        m_framebuffer->setDepthStencilAttachment(1.0f, 0);

    }

}