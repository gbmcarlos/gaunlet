#pragma once

#include "gaunlet/pch.h"

#include <imgui.h>

namespace gaunlet::Editor {

    enum class DockSpacePosition {
        Left, Right,
        Up, Down,
        Center
    };

    struct DockSpaceSpec {

        DockSpaceSpec(DockSpacePosition position, float sizeRatio, std::vector<const char*> windowIds, int dockNodeFalgs = 0)
            : m_position(position), m_sizeRatio(sizeRatio), m_windowIds(std::move(windowIds)), m_dockNodeFlags(dockNodeFalgs) {}

        DockSpacePosition m_position;
        float m_sizeRatio = 0.0f;
        std::vector<const char*> m_windowIds = {};
        int m_dockNodeFlags = 0;

    };

    struct DockingLayoutSpec {

        DockingLayoutSpec() = default;
        DockingLayoutSpec(const std::initializer_list<DockSpaceSpec>& dockSpaceSpecs, unsigned int width, unsigned height, int windowFlags = 0)
            : m_dockSpaceSpecs(dockSpaceSpecs), m_width(width), m_height(height), m_windowFlags(windowFlags) {}

        std::vector<DockSpaceSpec> m_dockSpaceSpecs = {};
        unsigned int m_width = 0;
        unsigned int m_height = 0;
        int m_windowFlags = 0;

    };

    class DockingLayout {

    public:
        explicit DockingLayout(DockingLayoutSpec viewportLayoutSpec) : m_dockingLayoutSpec(std::move(viewportLayoutSpec)) {}

        void begin(const char* name);
        void end();

    private:
        DockingLayoutSpec m_dockingLayoutSpec;

        void generateDockSpaces(ImGuiID& mainDockSpaceId);
        ImGuiDir convertDockSpacePosition(DockSpacePosition position);

    };

}