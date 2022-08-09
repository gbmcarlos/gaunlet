#include "ViewportLayout.h"

#include <imgui/imgui_internal.h>

namespace engine {

    ViewportLayout::ViewportLayout(const std::initializer_list<DockSpaceSpecs> &dockSpaceSpecs, glm::vec2 position, glm::vec2 size)
    : m_dockSpaceSpecs(dockSpaceSpecs), m_position(position), m_size(size) {}

    void ViewportLayout::init() {

        bool open = true;

        // The main viewport, aka the application window
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Prepare the main window, which will contain all the dock nodes
        // Prepare the flags
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // Prepare the properties and styles
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(ImVec2(m_size.x, m_size.y));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // Create the window (and remove the styles we've added)
        ImGui::Begin("Layout Main Window", &open, window_flags);
        ImGui::PopStyleVar(3);

        // Only needs to be created once
        if (ImGui::DockBuilderGetNode(ImGui::GetID("Layout Main DockSpace")) == NULL) {

            // The dock space itself, in the window
            ImGuiID dockspaceId = ImGui::GetID("Layout Main DockSpace");

            // Clear out the existing layout, and add an empty new one
            ImGui::DockBuilderRemoveNode(dockspaceId); // Clear out existing layout
            ImGui::DockBuilderAddNode(dockspaceId); // Add empty node
            ImGui::DockBuilderSetNodeSize(dockspaceId, ImVec2(m_size.x, m_size.y));

            // Prepare the dock spaces, according to the specs
            generateDockSpaces(dockspaceId);

            // Finish the docking
            ImGui::DockBuilderFinish(dockspaceId);

        }

        // Add the dock space and finish the main window
        ImGuiID dockspaceId = ImGui::GetID("Layout Main DockSpace");
        ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_CentralNode);
        ImGui::End();

    }

    void ViewportLayout::generateDockSpaces(ImGuiID& mainDockSpaceId) {

        for (auto& dockSpaceSpec : m_dockSpaceSpecs) {

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

            // Assign all the windows in the spec
            for (auto& windowId : dockSpaceSpec.m_windowIds) {
                ImGui::DockBuilderDockWindow(windowId, dockPositionId);
            }

        }

    }

    ImGuiDir ViewportLayout::convertDockSpacePosition(DockSpacePosition position) {

        switch (position) {
            case DockSpacePosition::Left: return ImGuiDir_Left;
            case DockSpacePosition::Right: return ImGuiDir_Right;
            case DockSpacePosition::Up: return ImGuiDir_Up;
            case DockSpacePosition::Down: return ImGuiDir_Down;
        }

        throw std::runtime_error("Unknown dock space position");

    }

}