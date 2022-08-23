#pragma once

#include "core/application/Layer.h"
#include "core/window/Window.h"

#include "graphics/framebuffer/Framebuffer.h"
#include "scene/camera/Camera.h"

#include "layout/docking/DockingLayout.h"
#include "layout/application/DockedNode.h"

#include "pch.h"

namespace engine::Layout {

class LayoutLayer : public Core::Layer {

    public:
        explicit LayoutLayer(Core::Ref<Core::Window> window);

        void setLayoutSpec(const DockingLayoutSpec& layoutSpec);
        void pushNode(const char* windowId, GuiDockedNode* node);
        void pushNode(const char* windowId, RenderDockedNode* node, Core::Ref<Scene::Camera> camera, Core::Ref<Graphics::Framebuffer> framebuffer, unsigned int colorAttachmentIndex);

        void onEvent(Core::Event& event) override;
        void onGuiRender() override;

    private:

        DockingLayoutSpec m_layoutSpec;
        Core::Ref<Core::Window> m_window = nullptr;

        struct GuiNode {
            const char* m_windowId;
            GuiDockedNode* m_node;
        };

        struct RenderNode {
            const char* m_windowId;
            RenderDockedNode* m_node;
            Core::Ref<Scene::Camera> m_camera;
            Core::Ref<Graphics::Framebuffer> m_framebuffer;
            unsigned int m_colorAttachmentIndex;
        };

        std::vector<GuiNode> m_guiNodes = {};
        std::vector<RenderNode> m_renderNodes = {};

        void handleMouseEvent(Core::Event& event);
        void handleKeyboardEvent(Core::Event& event);

        void updateNodeProperties(DockedNode* node);

    };

}