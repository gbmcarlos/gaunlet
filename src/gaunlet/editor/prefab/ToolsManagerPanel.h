#pragma once

#include "gaunlet/editor/panel/Panel.h"

namespace gaunlet::Editor {

    class ToolsManagerPanel : public GuiPanel {

    public:

        void onGuiRender() override {

            toolSelection();

        }

    private:

        void toolSelection() {

            const char* previewLabel = getWorkspace()->getActiveToolId() ? getWorkspace()->getActiveTool()->getName() : "-";

            if (ImGui::BeginCombo("tools", previewLabel)) {

                for (auto &tool: getWorkspace()->getTools()) {

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

    };

}