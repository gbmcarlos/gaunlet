#pragma once

#include "gaunlet/editor/workspace/Workspace.h"
#include "gaunlet/scene/entity/Entity.h"
#include "gaunlet/editor/Tags.h"
#include "gaunlet/editor/render-pipeline/RenderPipeline.h"
#include "gaunlet/prefab/render-pipeline-extensions/EntitySelectionExtension.h"

namespace gaunlet::Prefab::EditorTools {

    class EntitySelectionTool : public Editor::Tool {

    protected:

        Editor::RenderPanel* m_renderPanel = nullptr;

        virtual void onSceneEntitySelected(Scene::Entity sceneEntity) {}
        virtual void onSceneEntityUnselected(Scene::Entity sceneEntity) {}
        virtual void onUIEntitySelected(Scene::Entity uiEntity) {}
        virtual void onUIEntityUnselected(Scene::Entity uiEntity) {}

        Scene::Entity getSelectedSceneEntity() {return getWorkspace()->getSelectedSceneEntity(); }
        Scene::Entity getSelectedUIEntity() {return getWorkspace()->getSelectedUIEntity(); }

        void selectFocusedRenderPanel() {
            m_renderPanel = getWorkspace()->getFocusedRenderPanel();
        }

        void selectHoveredRenderPanel() {
            m_renderPanel = getWorkspace()->getHoveredRenderPanel();
        }

        template<typename T>
        void selectFocusedRenderPanelWithExtension() {

            auto renderPanel = getWorkspace()->getFocusedRenderPanel();
            if (renderPanel && renderPanel->getRenderPipeline()->hasExtension<T>()) {
                m_renderPanel = renderPanel;
            } else {
                m_renderPanel = nullptr;
            }

        }

        template<typename T>
        void selectHoveredRenderPanelWithExtension() {

            auto renderPanel = getWorkspace()->getHoveredRenderPanel();
            if (renderPanel && renderPanel->getRenderPipeline()->hasExtension<T>()) {
                m_renderPanel = renderPanel;
            } else {
                m_renderPanel = nullptr;
            }

        }

        Scene::Entity selectSceneEntity(Editor::RenderPanel* renderPanel) {

            auto previousSelectedSceneEntity = getWorkspace()->getSelectedSceneEntity();
            auto newSelectedSceneEntity = mousePickTaggedEntity<Editor::SceneEntityTag>(renderPanel, RenderPipelineExtensions::EntitySelectionExtension::EntityLayer::SceneLayer);

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

            // Make sure the render panel's render pipeline has entity selection
            if (!renderPanel->getRenderPipeline()->hasExtension<RenderPipelineExtensions::EntitySelectionExtension>()) {
                return {};
            }

            auto previousSelectedUIEntity = getWorkspace()->getSelectedUIEntity();
            auto newSelectedUIEntity = mousePickTaggedEntity<Editor::UIEntityTag>(renderPanel, RenderPipelineExtensions::EntitySelectionExtension::EntityLayer::UILayer);

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
        Scene::Entity mousePickTaggedEntity(Editor::RenderPanel* renderPanel, RenderPipelineExtensions::EntitySelectionExtension::EntityLayer entityLayer) {

            // Make sure the render panel's render pipeline has entity selection
            if (!renderPanel->getRenderPipeline()->hasExtension<RenderPipelineExtensions::EntitySelectionExtension>()) {
                return {};
            }

            unsigned int pixelPositionX = renderPanel->getMousePositionX() * Core::Window::getCurrentInstance()->getDPI();
            unsigned int pixelPositionY = renderPanel->getMousePositionYInverted() * Core::Window::getCurrentInstance()->getDPI();

            auto entitySelectionExtension = renderPanel->getRenderPipeline()->getExtension<RenderPipelineExtensions::EntitySelectionExtension>();
            auto selectedEntityId = entitySelectionExtension->getEntityId(
                entityLayer,
                pixelPositionX,
                pixelPositionY
            );

            auto& scene = renderPanel->getScene();

            Scene::Entity selectedEntity = Scene::Entity(selectedEntityId, scene);

            if (selectedEntity && selectedEntity.hasComponent<T>()) {
                return selectedEntity;
            } else {
                return {};
            }

        }

    };

}