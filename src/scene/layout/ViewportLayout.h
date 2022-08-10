#pragma once

#include <imgui.h>

#include <glm/glm.hpp>

#include <utility>
#include <vector>
#include <string>

namespace engine {

    enum class DockSpacePosition {
        Left, Right,
        Up, Down,
        Center
    };

    struct DockSpaceSpecs {

        DockSpaceSpecs(DockSpacePosition position, float sizeRatio, std::vector<const char*> windowIds, int dockNodeFalgs = 0) : m_position(position), m_sizeRatio(sizeRatio), m_windowIds(std::move(windowIds)), m_dockNodeFlags(dockNodeFalgs) {}

        DockSpacePosition m_position;
        float m_sizeRatio = 0.0f;
        std::vector<const char*> m_windowIds;
        int m_dockNodeFlags;

    };

    class ViewportLayout {

    public:
        ViewportLayout(const std::initializer_list<DockSpaceSpecs>& dockSpaceSpecs, glm::vec2 position, glm::vec2 size, int windowFlags = 0);
        void begin(const char* name);
        void end();

        ImGuiDir convertDockSpacePosition(DockSpacePosition position);

    private:
        std::vector<DockSpaceSpecs> m_dockSpaceSpecs;
        glm::vec2 m_position;
        glm::vec2 m_size;
        int m_windowFlags = 0;

        void generateDockSpaces(ImGuiID& mainDockSpaceId);

    };

}