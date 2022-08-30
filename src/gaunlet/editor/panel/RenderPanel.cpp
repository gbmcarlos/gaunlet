#include "gaunlet/editor/panel/RenderPanel.h"
#include "gaunlet/core/window/Window.h"

namespace gaunlet::Editor {

    void RenderPanel::selectSceneEntity(Scene::Entity entity) {

        bool changed = false;

        if (m_selectedSceneEntity != entity) {
            changed = true;
        }

        m_selectedSceneEntity = entity;

        if (changed && m_sceneSelectionCallback) {
            m_sceneSelectionCallback(entity);
        }

    }

    void RenderPanel::selectUIEntity(Scene::Entity entity) {

        bool changed = false;

        if (m_selectedUIEntity != entity) {
            changed = true;
        }

        m_selectedUIEntity = entity;

        if (changed && m_uiSelectionCallback) {
            m_uiSelectionCallback(entity);
        }

    }

    void RenderPanel::setSceneSelectionCallback(const std::function<void(Scene::Entity &)> &callback) {
        m_sceneSelectionCallback = callback;
    }

    void RenderPanel::setUISelectionCallback(const std::function<void(Scene::Entity &)> &callback) {
        m_uiSelectionCallback = callback;
    }

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

    void RenderPanel::mousePickEntity(unsigned int mousePositionX, unsigned int mousePositionY) {

        unsigned int pixelPositionX = mousePositionX * Core::Window::getCurrentInstance()->getDPI();
        unsigned int pixelPositionY = mousePositionY * Core::Window::getCurrentInstance()->getDPI();

        Scene::Entity selectedUIEntity = {};
        Scene::Entity selectedSceneEntity = {};

        int selectedUIEntityId = getFramebuffer()->readPixel(
            gaunlet::Editor::RenderPanel::UIEntityIdFramebufferAttachmentIndex,
            pixelPositionX,
            pixelPositionY
        );

        selectedUIEntity = Scene::Entity(selectedUIEntityId, &m_scene.getRegistry());

        // If there's a selected UI entity, look for the scene entity (the UI's parent)
        if (selectedUIEntity) {

            selectedSceneEntity = selectedUIEntity.findTaggedAncestor<SceneEntityTag>();

        } else {

            int selectedSceneEntityId = getFramebuffer()->readPixel(
                gaunlet::Editor::RenderPanel::SceneEntityIdFramebufferAttachmentIndex,
                pixelPositionX,
                pixelPositionY
            );

            selectedSceneEntity = Scene::Entity(selectedSceneEntityId, &m_scene.getRegistry());

        }

        selectSceneEntity(selectedSceneEntity);
        selectUIEntity(selectedUIEntity);

    }

}