#pragma once

#include "ImGuiRenderApi.h"

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

        DockSpaceSpecs(DockSpacePosition position, float sizeRatio, std::vector<const char*> windowIds) : m_position(position), m_sizeRatio(sizeRatio), m_windowIds(std::move(windowIds)) {}

        DockSpacePosition m_position;
        float m_sizeRatio = 0.0f;
        std::vector<const char*> m_windowIds;

    };

    class ViewportLayout {

    public:
        ViewportLayout(const std::initializer_list<DockSpaceSpecs>& dockSpaceSpecs, glm::vec2 position, glm::vec2 size);
        void init();

        ImGuiDir convertDockSpacePosition(DockSpacePosition position);

    private:
        glm::vec2 m_position;
        glm::vec2 m_size;
        std::vector<DockSpaceSpecs> m_dockSpaceSpecs;

        void generateDockSpaces(ImGuiID& mainDockSpaceId);

    };

}