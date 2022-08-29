#include "gaunlet/editor/panel/RenderPanel.h"
#include "gaunlet/core/window/Window.h"

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

    Scene::Entity RenderPanel::createSceneEntity() {

        auto entity = m_scene.getRegistry().createEntity();
        entity.addEmptyComponent<SceneEntityTag>();

        return entity;

    }

    Scene::Entity RenderPanel::createUIEntity() {

        auto entity = m_scene.getRegistry().createEntity();
        entity.addEmptyComponent<UIEntityTag>();

        return entity;

    }

    Scene::Entity RenderPanel::createUIEntity(Scene::Entity sceneEntity) {

        auto entity = sceneEntity.createChild();
        entity.addEmptyComponent<UIEntityTag>();

        return entity;

    }

    void RenderPanel::mousePickEntity(unsigned int mousePositionX, unsigned int mousePositionY) {

        // Reset both to the default (empty, null) entity
        m_selectedSceneEntity = Scene::Entity();
        m_selectedUIEntity = Scene::Entity();

        unsigned int pixelPositionX = mousePositionX * Core::Window::getCurrentInstance()->getDPI();
        unsigned int pixelPositionY = mousePositionY * Core::Window::getCurrentInstance()->getDPI();

        int selectedUIEntityId = getFramebuffer()->readPixel(
            gaunlet::Editor::RenderPanel::UIEntityIdFramebufferAttachmentIndex,
            pixelPositionX,
            pixelPositionY
        );

        m_selectedUIEntity = Scene::Entity(selectedUIEntityId, &m_scene.getRegistry());

        // If there's a selected UI entity, look for the scene entity (the UI's parent)
        if (m_selectedUIEntity) {

            auto sceneEntity = m_selectedUIEntity.getParent();
            if (sceneEntity) {
                m_selectedSceneEntity = sceneEntity;
            }

        } else {

            int selectedSceneEntityId = getFramebuffer()->readPixel(
                gaunlet::Editor::RenderPanel::SceneEntityIdFramebufferAttachmentIndex,
                pixelPositionX,
                pixelPositionY
            );

            m_selectedSceneEntity = Scene::Entity(selectedSceneEntityId, &m_scene.getRegistry());

        }

    }

}