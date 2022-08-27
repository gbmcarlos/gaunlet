#pragma once

#include "gaunlet/core/application/TimeStep.h"
#include "gaunlet/core/application/Layer.h"
#include "gaunlet/core/window/Window.h"

#include "gaunlet/graphics/framebuffer/Framebuffer.h"
#include "gaunlet/scene/camera/Camera.h"

#include "gaunlet/layout/docking/DockingLayout.h"
#include "gaunlet/layout/panel/Panel.h"
#include "gaunlet/layout/panel/RenderPanel.h"

#include "gaunlet/pch.h"

namespace gaunlet::Layout {

class LayoutLayer : public Core::Layer {

    public:
        explicit LayoutLayer(Core::Ref<Core::Window> window);

        void setLayoutSpec(const DockingLayoutSpec& layoutSpec);
        void pushPanel(const char* windowId, GuiPanel* panel);
        void pushPanel(const char* windowId, RenderPanel* panel);

        void onGuiRender() override;
        void onUpdate(gaunlet::Core::TimeStep) override;
        void onEvent(Core::Event& event) override;

    private:

        DockingLayoutSpec m_layoutSpec;
        Core::Ref<Core::Window> m_window = nullptr;

        struct GuiPanelWindow {
            const char* m_windowId;
            GuiPanel* m_panel;
        };

        struct RenderPanelWindow {
            const char* m_windowId;
            RenderPanel* m_node;
        };

        std::vector<GuiPanelWindow> m_guiPanelWindows = {};
        std::vector<RenderPanelWindow> m_renderPanelWindows = {};

        void handleMouseEvent(Core::Event& event);
        void handleKeyboardEvent(Core::Event& event);

        void prepareRenderNode(RenderPanel& renderPanel);
        void updateNodeProperties(Panel* node);

    };

}