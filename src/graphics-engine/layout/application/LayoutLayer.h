#pragma once

#include "graphics-engine/core/application/Layer.h"
#include "graphics-engine/core/window/Window.h"

#include "graphics-engine/graphics/framebuffer/Framebuffer.h"
#include "graphics-engine/scene/camera/Camera.h"

#include "graphics-engine/layout/docking/DockingLayout.h"
#include "graphics-engine/layout/application/Panel.h"

#include "graphics-engine/pch.h"

namespace engine::Layout {

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