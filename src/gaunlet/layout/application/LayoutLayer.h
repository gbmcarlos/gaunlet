#pragma once

#include "gaunlet/core/application/Layer.h"
#include "gaunlet/core/window/Window.h"

#include "gaunlet/graphics/framebuffer/Framebuffer.h"
#include "gaunlet/scene/camera/Camera.h"

#include "gaunlet/layout/docking/DockingLayout.h"
#include "gaunlet/layout/application/Panel.h"

#include "gaunlet/pch.h"

namespace gaunlet::Layout {

class LayoutLayer : public Core::Layer {

    public:
        explicit LayoutLayer(Core::Ref<Core::Window> window);

        void setLayoutSpec(const DockingLayoutSpec& layoutSpec);
        void pushPanel(const char* windowId, GuiPanel* panel);
        void pushPanel(const char* windowId, RenderPanel* panel, Core::Ref<Scene::Camera> camera, Core::Ref<Graphics::Framebuffer> framebuffer, unsigned int colorAttachmentIndex);

        void onEvent(Core::Event& event) override;
        void onGuiRender() override;

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
            Core::Ref<Scene::Camera> m_camera;
            Core::Ref<Graphics::Framebuffer> m_framebuffer;
            unsigned int m_colorAttachmentIndex;
        };

        std::vector<GuiPanelWindow> m_guiPanelWindows = {};
        std::vector<RenderPanelWindow> m_renderPanelWindows = {};

        void handleMouseEvent(Core::Event& event);
        void handleKeyboardEvent(Core::Event& event);

        void updateNodeProperties(Panel* node);

    };

}