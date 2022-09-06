#include "gaunlet/editor/panel/RenderPanel.h"

#include "gaunlet/editor/Tags.h"
#include "gaunlet/core/window/Window.h"
#include "gaunlet/editor/workspace/Workspace.h"

namespace gaunlet::Editor {

    RenderPanel::RenderPanel() {

        auto window = Core::Window::getCurrentInstance();

        m_framebuffer = gaunlet::Core::CreateRef<gaunlet::Graphics::Framebuffer>(std::initializer_list<gaunlet::Graphics::FramebufferAttachmentSpec>{
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::RGBA, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)},
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::Integer, -1},
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::Integer, -1},
            {gaunlet::Core::FramebufferAttachmentType::Depth, gaunlet::Graphics::FramebufferDataFormat::Depth}
        }, window->getViewportWidth() * window->getDPI(), window->getViewportHeight() * window->getDPI());

    }

    void RenderPanel::onUpdate(Core::TimeStep timeStep) {

        m_framebuffer->bind();

        // First, make sure the draw buffers are in the right order, and clear everything
        m_framebuffer->setDrawBuffers({
            RenderPanel::SceneFramebufferAttachmentIndex,
            RenderPanel::SceneEntityIdFramebufferAttachmentIndex,
            RenderPanel::UIEntityIdFramebufferAttachmentIndex
        });
        m_framebuffer->clear();

        // Render the scene entities
        getWorkspace()->getScene(m_sceneId)->renderTagged<SceneEntityTag>(
            m_renderMode,
            getWorkspace()->getCamera(m_cameraId),
            getWorkspace()->getDirectionalLight(m_directionalLightId),
            getWorkspace()->getSkybox(m_skyboxId)
        );

        // Then clear just the depth buffer, and switch the 2 entity id buffers
        m_framebuffer->clearDepthAttachment();
        m_framebuffer->setDrawBuffers({
            RenderPanel::SceneFramebufferAttachmentIndex,
            RenderPanel::UIEntityIdFramebufferAttachmentIndex,
            RenderPanel::SceneEntityIdFramebufferAttachmentIndex
        });

        // And render the UI entities
        getWorkspace()->getScene(m_sceneId)->renderTagged<UIEntityTag>(
            m_renderMode,
            getWorkspace()->getCamera(m_cameraId),
            Core::CreateRef<Scene::DirectionalLightComponent>(),
            Core::CreateRef<Scene::SkyboxComponent>()
        );

        m_framebuffer->unbind();

    }

    void RenderPanel::resize() {

        getWorkspace()->getCamera(m_cameraId)->resize(
            getWidth(),
            getHeight()
        );
        m_framebuffer->resize(
            getWidth() * Core::Window::getCurrentInstance()->getDPI(),
            getHeight() * Core::Window::getCurrentInstance()->getDPI()
        );

    }

    const Core::Ref<Graphics::Texture>& RenderPanel::getRenderedTexture() {
        return m_framebuffer->getColorAttachment(RenderPanel::SceneFramebufferAttachmentIndex);
    }

}