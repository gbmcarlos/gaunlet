#pragma once

#include "DockedNode.h"

#include "../../core/application/Layer.h"
#include "../../core/window/Window.h"

#include "../graphics/framebuffer/Framebuffer.h"
#include "../scene/camera/Camera.h"

#include "../docking/DockingLayout.h"

#include "../pch.h"

namespace engine {

    class LayoutLayer : public Layer {

    public:
        explicit LayoutLayer(Ref<Window> window);

        void setLayoutSpec(const DockingLayoutSpec& layoutSpec);
        void pushNode(const char* windowId, GuiDockedNode* node);
        void pushNode(const char* windowId, RenderDockedNode* node, Ref<Camera> camera, Ref<Framebuffer> framebuffer, unsigned int colorAttachmentIndex);

        void onEvent(Event& event) override;
        void onGuiRender() override;

    private:

        DockingLayoutSpec m_layoutSpec;
        Ref<Window> m_window = nullptr;

        struct GuiNode {
            const char* m_windowId;
            GuiDockedNode* m_node;
        };

        struct RenderNode {
            const char* m_windowId;
            RenderDockedNode* m_node;
            Ref<Camera> m_camera;
            Ref<Framebuffer> m_framebuffer;
            unsigned int m_colorAttachmentIndex;
        };

        std::vector<GuiNode> m_guiNodes = {};
        std::vector<RenderNode> m_renderNodes = {};

        void handleMouseEvent(Event& event);
        void handleKeyboardEvent(Event& event);

        void updateNodeProperties(DockedNode* node);

    };

}