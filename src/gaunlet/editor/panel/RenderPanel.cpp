#include "gaunlet/editor/panel/RenderPanel.h"

namespace gaunlet::Editor {

    void RenderPanel::onUpdate(gaunlet::Core::TimeStep) {

        m_framebuffer->bind();

        // First, make sure the draw buffers are in the right order, and clear everything
        m_framebuffer->setDrawBuffers({
            RenderPanel::SceneFramebufferAttachmentIndex,
            RenderPanel::SceneEntityIdFramebufferAttachmentIndex,
            RenderPanel::UIEntityIdFramebufferAttachmentIndex
        });
        m_framebuffer->clear();

        // Render the scene entities
        m_scene.renderTagged<SceneEntityTag>(m_renderMode, m_camera, Scene::DirectionalLightComponent());

        // Then clear just the depth buffer, and switch the 2 entity id buffers
        m_framebuffer->clearDepthAttachment();
        m_framebuffer->setDrawBuffers({
            RenderPanel::SceneFramebufferAttachmentIndex,
            RenderPanel::UIEntityIdFramebufferAttachmentIndex,
            RenderPanel::SceneEntityIdFramebufferAttachmentIndex
        });

        // And render the UI entities
        m_scene.renderTagged<UIEntityTag>(m_renderMode, m_camera, Scene::DirectionalLightComponent());

        m_framebuffer->unbind();

    }

    gaunlet::Scene::Entity RenderPanel::createSceneEntity() {

        auto entity = m_scene.getRegistry().createEntity();
        entity.addEmptyComponent<SceneEntityTag>();

        return entity;

    }

    gaunlet::Scene::Entity RenderPanel::createUIEntity() {

        auto entity = m_scene.getRegistry().createEntity();
        entity.addEmptyComponent<UIEntityTag>();

        return entity;

    }

}