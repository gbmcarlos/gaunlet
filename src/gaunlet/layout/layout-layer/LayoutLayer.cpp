#include "gaunlet/layout/layout-layer/LayoutLayer.h"

namespace gaunlet::Layout {

    LayoutLayer::LayoutLayer(Core::Ref<Core::Window> window) : m_window(std::move(window)) {}

    void LayoutLayer::setLayoutSpec(const DockingLayoutSpec& layoutSpec) {
        m_layoutSpec = layoutSpec;
    }

    void LayoutLayer::pushPanel(const char* windowId, GuiPanel* panel) {
        m_guiPanelWindows.push_back({windowId, panel});
    }

    void LayoutLayer::pushPanel(const char* windowId, RenderPanel* panel) {
        prepareRenderNode(*panel);
        m_renderPanelWindows.push_back({windowId, panel});
    }

    void LayoutLayer::onUpdate(gaunlet::Core::TimeStep timeStep) {

        for (auto& renderPanelWindow : m_renderPanelWindows) {
            renderPanelWindow.m_node->onUpdate(timeStep);
        }

    }

    void LayoutLayer::onGuiRender() {

        if (!m_layoutSpec.m_width) {
            return;
        }

        DockingLayout dockingLayout(m_layoutSpec);

        // Init the layout
        dockingLayout.begin("Main Window");
        dockingLayout.end();

        // Update and run the GUI panel windows
        for (auto& guiPanelWindow : m_guiPanelWindows) {

            ImGui::Begin(guiPanelWindow.m_windowId);

            updateNodeProperties(guiPanelWindow.m_panel);

            guiPanelWindow.m_panel->onGuiRender();

            ImGui::End();

        }

        // Update and run the Render panel windows
        for (auto& renderPanelWindow : m_renderPanelWindows) {

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin(renderPanelWindow.m_windowId);

            updateNodeProperties(renderPanelWindow.m_node);

            // If the aspect ratio has changed, the camera and the framebuffer need to be resized
            if (renderPanelWindow.m_node->m_camera->getAspectRatio() != renderPanelWindow.m_node->getNodeAspectRatio()) {

                renderPanelWindow.m_node->m_camera->resize(
                    renderPanelWindow.m_node->getNodeWidth(),
                    renderPanelWindow.m_node->getNodeHeight()
                );
                renderPanelWindow.m_node->m_framebuffer->resize(
                    renderPanelWindow.m_node->getNodeWidth() * m_window->getDPI(),
                    renderPanelWindow.m_node->getNodeHeight() * m_window->getDPI()
                );

            }

            // Render the framebuffer's color attachment texture as an ImGui image
            auto& colorAttachmentTexture = renderPanelWindow.m_node->m_framebuffer->getColorAttachment(RenderPanel::SceneFramebufferAttachmentIndex);
            ImGui::Image(
                (void *)(intptr_t)colorAttachmentTexture->getRendererId(),
                ImGui::GetContentRegionAvail(),
                ImVec2(0, 1), ImVec2(1, 0)
            );

            ImGui::End();
            ImGui::PopStyleVar(1);

        }

    }

    void LayoutLayer::onEvent(Core::Event& event) {

        if (event.getCategory() == Core::EventCategory::Mouse || event.getCategory() == Core::EventCategory::Cursor || event.getCategory() == Core::EventCategory::Scroll) {
            handleMouseEvent(event);
        } else if (event.getCategory() == Core::EventCategory::Keyboard) {
            handleKeyboardEvent(event);
        }

    }

    void LayoutLayer::handleMouseEvent(Core::Event& event) {

        for (auto& renderNode : m_renderPanelWindows) {
            if (renderNode.m_node->m_isHovered) {
                renderNode.m_node->onEvent(event);
                break;
            }
        }

    }

    void LayoutLayer::handleKeyboardEvent(Core::Event& event) {

        for (auto& renderNode : m_renderPanelWindows) {
            bool handled = renderNode.m_node->onEvent(event);
            if (handled) {
                break;
            }
        }

    }

    void LayoutLayer::prepareRenderNode(RenderPanel &renderPanel) {

        renderPanel.m_camera = gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(
            45.0f,
            (float) m_window->getViewportWidth() /(float) m_window->getViewportHeight(),
            100,
            1.0f, 100.0f
        );

        renderPanel.m_camera->setTranslation({0.0f, 2.0f, 10.0f});

        renderPanel.m_framebuffer = gaunlet::Core::CreateRef<gaunlet::Graphics::Framebuffer>(std::initializer_list<gaunlet::Graphics::FramebufferAttachmentSpec>{
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::RGBA, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)},
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::Integer, -1},
            {gaunlet::Core::FramebufferAttachmentType::Depth, gaunlet::Graphics::FramebufferDataFormat::Depth}
        }, m_window->getViewportWidth() * m_window->getDPI(), m_window->getViewportHeight() * m_window->getDPI());

    }

    void LayoutLayer::updateNodeProperties(Panel *node) {

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