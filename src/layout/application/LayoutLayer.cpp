#include "LayoutLayer.h"

namespace engine {

    LayoutLayer::LayoutLayer(std::shared_ptr<Window> window) : m_window(std::move(window)) {}

    void LayoutLayer::setLayoutSpec(const DockingLayoutSpec& layoutSpec) {
        m_layoutSpec = layoutSpec;
    }

    void LayoutLayer::pushNode(const char* windowId, GuiDockedNode* node) {
        m_guiNodes.push_back({windowId, node});
    }

    void LayoutLayer::pushNode(const char* windowId, RenderDockedNode* node, std::shared_ptr<Camera> camera, std::shared_ptr<Framebuffer> framebuffer, unsigned int colorAttachmentIndex) {
        m_renderNodes.push_back({windowId, node, std::move(camera), std::move(framebuffer), colorAttachmentIndex});
    }

    void LayoutLayer::onGuiRender() {

        if (!m_layoutSpec.m_width) {
            return;
        }

        DockingLayout dockingLayout(m_layoutSpec);

        // Init the layout
        dockingLayout.begin("Main Window");
        dockingLayout.end();

        // Update and run the GUI nodes
        for (auto& guiNode : m_guiNodes) {

            ImGui::Begin(guiNode.m_windowId);

            updateNodeProperties(guiNode.m_node);

            guiNode.m_node->onGuiRender();

            ImGui::End();

        }

        // Update and run the Render nodes
        for (auto& renderNode : m_renderNodes) {

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin(renderNode.m_windowId);

            updateNodeProperties(renderNode.m_node);

            auto& colorAttachmentTexture = renderNode.m_framebuffer->getColorAttachment(renderNode.m_colorAttachmentIndex);

            // If the aspect ratio has changed, the camera and the framebuffer need to be resized
            if (renderNode.m_camera->getAspectRatio() != renderNode.m_node->getNodeAspectRatio()) {

                renderNode.m_camera->resize(
                    renderNode.m_node->getNodeWidth(),
                    renderNode.m_node->getNodeHeight()
                );
                renderNode.m_framebuffer->resize(
                    renderNode.m_node->getNodeWidth() * m_window->getDPI(),
                    renderNode.m_node->getNodeHeight() * m_window->getDPI()
                );

            }

            ImGui::Image(
                (void *)(intptr_t)colorAttachmentTexture->getRendererId(),
                ImGui::GetContentRegionAvail(),
                ImVec2(0, 1), ImVec2(1, 0)
            );

            ImGui::End();
            ImGui::PopStyleVar(1);

        }

    }

    void LayoutLayer::onEvent(Event& event) {

        if (event.getCategory() == EventCategory::Mouse || event.getCategory() == EventCategory::Cursor || event.getCategory() == EventCategory::Scroll) {
            handleMouseEvent(event);
        } else if (event.getCategory() == EventCategory::Keyboard) {
            handleKeyboardEvent(event);
        }

    }

    void LayoutLayer::handleMouseEvent(Event& event) {

        for (auto& renderNode : m_renderNodes) {
            if (renderNode.m_node->m_isHovered) {
                renderNode.m_node->onEvent(event);
                break;
            }
        }

    }

    void LayoutLayer::handleKeyboardEvent(Event& event) {

        for (auto& renderNode : m_renderNodes) {
            bool handled = renderNode.m_node->onEvent(event);
            if (handled) {
                break;
            }
        }

    }

    void LayoutLayer::updateNodeProperties(DockedNode *node) {

        ImVec2 windowMin = ImGui::GetWindowContentRegionMin();
        ImVec2 windowMax = ImGui::GetWindowContentRegionMax();
        ImVec2 windowPosition = ImGui::GetWindowPos();
        ImVec2 mousePosition = ImGui::GetMousePos();
        ImVec2 padding = ImGui::GetStyle().WindowPadding;

        windowMin.x += windowPosition.x - padding.x;
        windowMin.y += windowPosition.y - padding.y;
        windowMax.x += windowPosition.x + padding.x;
        windowMax.y += windowPosition.y + padding.y;

        node->m_nodeWidth = windowMax.x - windowMin.x;
        node->m_nodeHeight = windowMax.y - windowMin.y;
        node->m_isHovered = ImGui::IsWindowHovered();
        node->m_mousePositionX = std::max(mousePosition.x - windowMin.x, 0.0f);
        node->m_mousePositionY = std::max(mousePosition.y - windowMin.y, 0.0f);
        node->m_mousePositionYInverted = windowMax.y - node->m_mousePositionY;

    }

}