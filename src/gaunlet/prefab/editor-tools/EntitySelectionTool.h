#pragma once

#include "gaunlet/editor/workspace/Workspace.h"
#include "gaunlet/scene/entity/Entity.h"
#include "gaunlet/editor/Tags.h"

namespace gaunlet::Prefab::EditorTools {

    class EntitySelectionTool : public Editor::Tool {

    protected:

        virtual void onSceneEntitySelected(Scene::Entity sceneEntity) {}
        virtual void onSceneEntityUnselected(Scene::Entity sceneEntity) {}
        virtual void onUIEntitySelected(Scene::Entity uiEntity) {}
        virtual void onUIEntityUnselected(Scene::Entity uiEntity) {}

        Scene::Entity getSelectedSceneEntity() {return getWorkspace()->getSelectedSceneEntity(); }
        Scene::Entity getSelectedUIEntity() {return getWorkspace()->getSelectedUIEntity(); }

        Scene::Entity selectSceneEntity(Editor::RenderPanel* renderPanel) {

            auto previousSelectedSceneEntity = getWorkspace()->getSelectedSceneEntity();
            auto newSelectedSceneEntity = mousePickTaggedEntity<Editor::SceneEntityTag>(renderPanel, 1);

            if (previousSelectedSceneEntity) {
                onSceneEntityUnselected(previousSelectedSceneEntity);
            }

            if (newSelectedSceneEntity) {
                onSceneEntitySelected(newSelectedSceneEntity);
                getWorkspace()->selectSceneEntity(newSelectedSceneEntity);
            } else {
                getWorkspace()->selectSceneEntity({});
            }

            return newSelectedSceneEntity ? newSelectedSceneEntity : Scene::Entity();

        }

        Scene::Entity selectUIEntity(Editor::RenderPanel* renderPanel) {

            auto previousSelectedUIEntity = getWorkspace()->getSelectedUIEntity();
            auto newSelectedUIEntity = mousePickTaggedEntity<Editor::UIEntityTag>(renderPanel, 2);

            if (previousSelectedUIEntity) {
                onUIEntityUnselected(previousSelectedUIEntity);
            }

            if (newSelectedUIEntity) {
                onUIEntitySelected(newSelectedUIEntity);
                getWorkspace()->selectUiEntity(newSelectedUIEntity);
            } else {
                getWorkspace()->selectUiEntity({});
            }

            return newSelectedUIEntity ? newSelectedUIEntity : Scene::Entity();

        }

        template<typename T>
        Scene::Entity mousePickTaggedEntity(Editor::RenderPanel* renderPanel, unsigned int framebufferAttachmentIndex) {

            unsigned int pixelPositionX = renderPanel->getMousePositionX() * Core::Window::getCurrentInstance()->getDPI();
            unsigned int pixelPositionY = renderPanel->getMousePositionYInverted() * Core::Window::getCurrentInstance()->getDPI();

            int selectedEntityId = getWorkspace()->getRenderPipeline(renderPanel->getRenderPipelineId())->readFramebuffer(
                framebufferAttachmentIndex,
                pixelPositionX,
                pixelPositionY
            );

            auto& scene = getWorkspace()->getScene(renderPanel->getSceneId());

            Scene::Entity selectedEntity = Scene::Entity(selectedEntityId, scene);

            if (selectedEntity && selectedEntity.hasComponent<T>()) {
                return selectedEntity;
            } else {
                return {};
            }

        }

    };

}