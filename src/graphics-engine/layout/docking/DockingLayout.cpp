#include "graphics-engine/layout/docking/DockingLayout.h"

#include "imgui/imgui_internal.h"

namespace engine::Layout {

    void DockingLayout::begin(const char* name) {

        bool open = true;

        // The main viewport, aka the application window
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Prepare the main window, which will contain all the dock nodes
        // Prepare the flags
        ImGuiWindowFlags windowDefaultFlags = ImGuiWindowFlags_NoDocking;
        windowDefaultFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowDefaultFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        windowDefaultFlags |= m_dockingLayoutSpec.m_windowFlags;

        // Prepare the properties and styles
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(ImVec2((float) m_dockingLayoutSpec.m_width, (float) m_dockingLayoutSpec.m_height));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // Create the window (and remove the styles we've added)
        ImGui::Begin(name, &open, windowDefaultFlags);
        ImGui::PopStyleVar(3);

        // Only needs to be created once
        if (ImGui::DockBuilderGetNode(ImGui::GetID("Layout Main DockSpace")) == NULL) {

            // The dock space itself, in the window
            ImGuiID dockspaceId = ImGui::GetID("Layout Main DockSpace");

            // Clear out the existing layout, and add an empty new one
            ImGui::DockBuilderRemoveNode(dockspaceId); // Clear out existing layout
            ImGui::DockBuilderAddNode(dockspaceId); // Add empty node
            ImGui::DockBuilderSetNodeSize(dockspaceId, ImVec2((float) m_dockingLayoutSpec.m_width, (float) m_dockingLayoutSpec.m_height));

            // Prepare the dock spaces, according to the specs
            generateDockSpaces(dockspaceId);

            // Finish the docking
            ImGui::DockBuilderFinish(dockspaceId);

        }

        // Add the dock space
        ImGuiID dockspaceId = ImGui::GetID("Layout Main DockSpace");
        ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_CentralNode);

    }

    void DockingLayout::end() {
        ImGui::End();
    }

    void DockingLayout::generateDockSpaces(ImGuiID& mainDockSpaceId) {

        for (auto& dockSpaceSpec : m_dockingLayoutSpec.m_dockSpaceSpecs) {

            ImGuiID dockPositionId;

            // The "Center" position means using the main dock space. Otherwise, split it
            if (dockSpaceSpec.m_position == DockSpacePosition::Center) {
                dockPositionId = mainDockSpaceId;
            } else {
                dockPositionId = ImGui::DockBuilderSplitNode(
                    mainDockSpaceId,
                    convertDockSpacePosition(dockSpaceSpec.m_position),
                    dockSpaceSpec.m_sizeRatio,
                    NULL,
                    &mainDockSpaceId
                );
            }

            if (dockSpaceSpec.m_dockNodeFlags != 1) {
                ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockPositionId);
                node->LocalFlags |= dockSpaceSpec.m_dockNodeFlags;
            }

            // Assign all the windows in the spec
            for (auto& windowId : dockSpaceSpec.m_windowIds) {
                ImGui::DockBuilderDockWindow(windowId, dockPositionId);
            }

        }

    }

    ImGuiDir DockingLayout::convertDockSpacePosition(DockSpacePosition position) {

        switch (position) {
            case DockSpacePosition::Left: return ImGuiDir_Left;
            case DockSpacePosition::Right: return ImGuiDir_Right;
            case DockSpacePosition::Up: return ImGuiDir_Up;
            case DockSpacePosition::Down: return ImGuiDir_Down;
            case DockSpacePosition::Center: return 0;
        }

        throw std::runtime_error("Unknown dock space position");

    }

}