#pragma once

#include "gaunlet/editor/panel/Panel.h"

namespace gaunlet::Prefab::GuiPanels {

    class ToolsManagerPanel : public Editor::GuiPanel {

    public:

        void onGuiRender() override {

            toolSelection();
            toolProperties();

        }

    private:

        void toolSelection() {

            const char* previewLabel = getWorkspace()->getActiveToolId() ? getWorkspace()->getActiveTool()->getName() : "-";

            if (ImGui::BeginCombo("tools", previewLabel)) {

                for (auto& tool: getWorkspace()->getTools()) {

                    if (ImGui::Selectable(tool->getName(), getWorkspace()->getActiveToolId() == tool->getId())) {

                        if (getWorkspace()->getActiveToolId() == tool->getId()) {
                            getWorkspace()->activateTool(nullptr);
                        } else {
                            getWorkspace()->activateTool(tool->getId());
                        }

                    }

                }

                ImGui::EndCombo();
            }

        }

        void toolProperties() {

            auto activeTool = getWorkspace()->getActiveTool();
            if (activeTool) {
                ImGui::Text("Tool \"%s\" properties:", activeTool->getName());
                activeTool->onGuiRender();
            }

        }

    };

}