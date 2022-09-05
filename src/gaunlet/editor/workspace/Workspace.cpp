#include "gaunlet/editor/workspace/Workspace.h"

#include "gaunlet/core/window/Window.h"

namespace gaunlet::Editor {

    void Workspace::setLayoutSpec(const DockingLayoutSpec& layoutSpec) {
        m_layoutSpec = layoutSpec;
    }

    void Workspace::pushPanel(const char* panelId, GuiPanel* panel, const char* windowId) {
        panel->m_id = panelId;
        panel->m_workspace = this;
        m_guiPanelSpecs.push_back({
            panel,
            panelId,
            windowId
        });
    }

    void Workspace::pushPanel(const char* panelId, RenderPanel* panel, const char* windowId, const char* cameraId, const char* sceneId, const char* directionalLightId, Scene::RenderMode renderMode) {
        panel->m_id = panelId;
        panel->m_workspace = this;
        panel->m_cameraId = cameraId;
        panel->m_sceneId = sceneId;
        panel->m_directionalLightId = directionalLightId;
        panel->m_renderMode = renderMode;
        m_renderPanelSpecs.push_back({
            panel,
            panelId,
            windowId
        });
    }

    void Workspace::addCamera(const char* id, const Core::Ref<Scene::Camera>& camera) {
        m_cameras[id] = camera;
    }

    void Workspace::addScene(const char* id, const Core::Ref<Scene::Scene>& scene) {
        m_scenes[id] = scene;
    }

    void Workspace::addDirectionalLight(const char* id, Scene::DirectionalLightComponent directionalLight) {
        m_directionalLights[id] = directionalLight;
    }

    const Core::Ref<Scene::Camera>& Workspace::getCamera(const char* id) {

        auto iterator = m_cameras.find(id);

        if (iterator == m_cameras.end()) {
            throw std::runtime_error("Camera not found");
        }

        return iterator->second;

    }

    const Core::Ref<Scene::Scene>& Workspace::getScene(const char* id) {

        auto iterator = m_scenes.find(id);

        if (iterator == m_scenes.end()) {
            throw std::runtime_error("Scene not found");
        }

        return iterator->second;

    }

    Scene::DirectionalLightComponent& Workspace::getDirectionalLight(const char* id) {

        auto iterator = m_directionalLights.find(id);

        if (iterator == m_directionalLights.end()) {
            throw std::runtime_error("DirectionalLight not found");
        }

        return iterator->second;

    }

    std::vector<GuiPanel*> Workspace::getGuiPanels() {

        std::vector<GuiPanel*> guiPanels = {};

        for (auto& guiPanelSpec : m_guiPanelSpecs) {
            guiPanels.push_back(guiPanelSpec.m_panel);
        }

        return guiPanels;

    }

    std::vector<RenderPanel*> Workspace::getRenderPanels() {

        std::vector<RenderPanel*> renderPanels = {};

        for (auto& renderPanelSpec : m_renderPanelSpecs) {
            renderPanels.push_back(renderPanelSpec.m_panel);
        }

        return renderPanels;

    }

    GuiPanel* Workspace::getGuiPanel(const char* id) {

        for (auto& guiPanelSpec : m_guiPanelSpecs) {
            if (guiPanelSpec.m_panelId == id) {
                return guiPanelSpec.m_panel;
            }
        }

        throw std::runtime_error("Panel not found");

    }

    RenderPanel* Workspace::getRenderPanel(const char* id) {

        for (auto& renderPanelSpec : m_renderPanelSpecs) {
            if (renderPanelSpec.m_panelId == id) {
                return renderPanelSpec.m_panel;
            }
        }

        throw std::runtime_error("Panel not found");

    }

    const Core::Ref<Tool>& Workspace::getTool(const char* id) {

        auto iterator = m_tools.find(id);

        if (iterator == m_tools.end()) {
            throw std::runtime_error("Tool not found");
        }

        return iterator->second;

    }

    Scene::Entity Workspace::getSelectedSceneEntity() {
        return m_selectedSceneEntity;
    }

    Scene::Entity Workspace::getSelectedUIEntity() {
        return m_selectedUIEntity;
    }

    void Workspace::onEvent(Core::Event& event) {

        if (event.getCategory() == Core::EventCategory::Mouse || event.getCategory() == Core::EventCategory::Scroll) {

            bool renderPanelHovered = false;
            for (auto renderPanelSpec : m_renderPanelSpecs) {
                renderPanelHovered = renderPanelHovered || renderPanelSpec.m_panel->isHovered();
            }

            if (!renderPanelHovered) {
                return;
            }

        }

        if (m_activeToolId) {
            auto& activeTool = getTool(m_activeToolId);

            activeTool->onEvent(event);

        }

    }

    void Workspace::update(Core::TimeStep timeStep) {

        for (auto& renderPanelSpec : m_renderPanelSpecs) {
            renderPanelSpec.m_panel->onUpdate(timeStep);
        }

    }

    void Workspace::render() {

        if (!m_layoutSpec.m_width) {
            return;
        }

        DockingLayout dockingLayout(m_layoutSpec);

        // Init the layout
        dockingLayout.begin("Main Window");
        dockingLayout.end();

        // Update and run the GUI panel windows
        for (auto& guiPanelSpec : m_guiPanelSpecs) {

            ImGui::Begin(guiPanelSpec.m_windowId);

            updateNodeProperties(guiPanelSpec.m_panel);

            guiPanelSpec.m_panel->onGuiRender();

            ImGui::End();

        }

        // Update and run the Render panel windows
        for (auto& renderPanelSpec : m_renderPanelSpecs) {

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin(renderPanelSpec.m_windowId);

            updateNodeProperties(renderPanelSpec.m_panel);

            auto panelCamera = getCamera(renderPanelSpec.m_panel->m_cameraId);

            // If the aspect ratio has changed, the panel needs to be resized
            if (panelCamera->getAspectRatio() != renderPanelSpec.m_panel->getNodeAspectRatio()) {
                renderPanelSpec.m_panel->resize();
            }

            // Render the framebuffer's color attachment texture as an ImGui image
            auto& colorAttachmentTexture = renderPanelSpec.m_panel->getRenderedTexture();
            ImGui::Image(
                (void *)(intptr_t)colorAttachmentTexture->getRendererId(),
                ImGui::GetContentRegionAvail(),
                ImVec2(0, 1), ImVec2(1, 0)
            );

            ImGui::End();
            ImGui::PopStyleVar(1);

        }

    }

    void Workspace::addTool(const char* id, const Core::Ref<Tool>& tool) {
        tool->m_workspace = this;
        m_tools[id] = tool;
    }

    void Workspace::activateTool(const char* id) {
        m_activeToolId = id;
    }

    void Workspace::selectSceneEntity(Scene::Entity entity) {

        bool changed = false;

        if (m_selectedSceneEntity != entity) {
            changed = true;
        }

        m_selectedSceneEntity = entity;

        // Event callback

    }

    void Workspace::selectUiEntity(Scene::Entity entity) {

        bool changed = false;

        if (m_selectedUIEntity != entity) {
            changed = true;
        }

        m_selectedUIEntity = entity;

        // Event callback

    }

    void Workspace::updateNodeProperties(Panel* node) {

        ImVec2 windowMin = ImGui::GetWindowContentRegionMin();
        ImVec2 windowMax = ImGui::GetWindowContentRegionMax();
        ImVec2 windowPosition = ImGui::GetWindowPos();
        ImVec2 mousePosition = ImGui::GetMousePos();
        ImVec2 padding = ImGui::GetStyle().WindowPadding;

        windowMin.x += windowPosition.x - padding.x;
        windowMin.y += windowPosition.y - padding.y;
        windowMax.x += windowPosition.x + padding.x;
        windowMax.y += windowPosition.y + padding.y;

        node->m_width = windowMax.x - windowMin.x;
        node->m_height = windowMax.y - windowMin.y;
        node->m_isHovered = ImGui::IsWindowHovered();
        node->m_mousePositionX = std::max(mousePosition.x - windowMin.x, 0.0f);
        node->m_mousePositionY = std::max(mousePosition.y - windowMin.y, 0.0f);
        node->m_mousePositionYInverted = windowMax.y - node->m_mousePositionY;

    }

}