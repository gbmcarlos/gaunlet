#pragma once

#include "gaunlet/editor/workspace/Workspace.h"
#include "gaunlet/scene/entity/Entity.h"

namespace gaunlet::Editor {

    class SelectionTool : public Tool {

    protected:

        virtual void onSceneEntitySelected(Scene::Entity sceneEntity) {}
        virtual void onSceneEntityUnselected(Scene::Entity sceneEntity) {}
        virtual void onUIEntitySelected(Scene::Entity uiEntity) {}
        virtual void onUIEntityUnselected(Scene::Entity uiEntity) {}

        Scene::Entity getSelectedSceneEntity() {return getWorkspace()->getSelectedSceneEntity(); }
        Scene::Entity getSelectedUIEntity() {return getWorkspace()->getSelectedUIEntity(); }

        Scene::Entity selectSceneEntity(RenderPanel* renderPanel) {

            auto previousSelectedSceneEntity = getWorkspace()->getSelectedSceneEntity();
            auto newSelectedSceneEntity = getWorkspace()->mousePickSceneEntity(renderPanel);

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

        Scene::Entity selectUIEntity(RenderPanel* renderPanel) {

            auto previousSelectedUIEntity = getWorkspace()->getSelectedUIEntity();
            auto newSelectedUIEntity = getWorkspace()->mousePickUIEntity(renderPanel);

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

    };

}