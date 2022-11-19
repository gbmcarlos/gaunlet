#pragma once

#include "gaunlet/editor/panel/Panel.h"
#include "gaunlet/editor/workspace/Workspace.h"
#include "gaunlet/editor/Tags.h"

#include "gaunlet/pch.h"

namespace gaunlet::Prefab::GuiPanels {

    class WorkspacePropertiesPanel : public Editor::GuiPanel {

    public:

        void onGuiRender() override {

            if (ImGui::CollapsingHeader("Workspace Properties")) {
                workspaceProperties();
            }

            if (ImGui::CollapsingHeader("Window Properties")) {
                windowProperties();
            }

            if (ImGui::CollapsingHeader("Selection")) {
                selectionProperties();
            }

        }

    private:

        void workspaceProperties() {

            ImGui::Checkbox("ModelOutline tag on select", &getWorkspace()->m_configuration.addModelOutlineTagOnSelect);
            ImGui::Checkbox("WireframeModel tag on select", &getWorkspace()->m_configuration.addWireframeModelTagOnSelect);

        }

        void windowProperties() {

            ImGui::Text("Window size: (%d, %d)", Core::Window::getCurrentInstance()->getWidth(), Core::Window::getCurrentInstance()->getHeight());
            ImGui::Text("Viewport size: (%d, %d)", Core::Window::getCurrentInstance()->getViewportWidth(), Core::Window::getCurrentInstance()->getViewportHeight());
            ImGui::Text("Mouse position: (%d, %d)", Core::Input::getMousePositionX(), Core::Input::getMousePositionY());

        }

        void selectionProperties() {

            auto selectedSceneEntity = getWorkspace()->getSelectedSceneEntity();
            if (selectedSceneEntity) {
                const char* sceneEntityLabel = selectedSceneEntity.hasName() ? selectedSceneEntity.getName() : "Unnamed entity";
                ImGui::Text("Selected Scene Entity: %s", sceneEntityLabel);
            } else {
                ImGui::Text("Selected Scene Entity: None");
            }

            auto selectedUIEntity = getWorkspace()->getSelectedUIEntity();
            if (selectedUIEntity) {
                const char* uiEntityLabel = selectedUIEntity.hasName() ? selectedUIEntity.getName() : "Unnamed entity";
                ImGui::Text("Selected UI Entity: %s", uiEntityLabel);
            } else {
                ImGui::Text("Selected UI Entity: None");
            }

        }

    };

}