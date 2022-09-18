#include "gaunlet/editor/workspace/Workspace.h"

#include "gaunlet/editor/Tags.h"
#include "gaunlet/core/window/Window.h"

namespace gaunlet::Editor {

    void Workspace::setLayoutSpec(const DockingLayoutSpec& layoutSpec) {
        m_layoutSpec = layoutSpec;
    }

    GuiPanel* Workspace::pushPanel(const char* panelId, GuiPanel* panel, const char* windowId) {
        panel->m_id = panelId;
        panel->m_workspace = this;
        m_guiPanelSpecs.push_back({
            panel,
            panelId,
            windowId
        });
        return panel;
    }

    RenderPanel* Workspace::pushPanel(const char* panelId, RenderPanel* panel, const char* windowId, const char* cameraId, const char* sceneId, const char* directionalLightId, const char* skyboxId, const char* renderPipelineId) {
        panel->m_id = panelId;
        panel->m_workspace = this;
        panel->m_cameraId = cameraId;
        panel->m_sceneId = sceneId;
        panel->m_directionalLightId = directionalLightId;
        panel->m_skyboxId = skyboxId;
        panel->m_renderPipelineId = renderPipelineId;
        m_renderPanelSpecs.push_back({
            panel,
            panelId,
            windowId
        });
        return panel;
    }

    const Core::Ref<Scene::Camera>& Workspace::addCamera(const char* id, const Core::Ref<Scene::Camera>& camera) {
        m_cameras[id] = camera;
        return camera;
    }

    const Core::Ref<Scene::Scene>& Workspace::addScene(const char* id, const Core::Ref<Scene::Scene>& scene) {
        m_scenes[id] = scene;
        return scene;
    }

    const Core::Ref<Scene::DirectionalLightComponent>& Workspace::addDirectionalLight(const char* id, const Core::Ref<Scene::DirectionalLightComponent>& directionalLight) {
        m_directionalLights[id] = directionalLight;
        return directionalLight;
    }

    const Core::Ref<Scene::SkyboxComponent>& Workspace::addSkybox(const char *id, const Core::Ref<Scene::SkyboxComponent>& skybox) {
        m_skyboxes[id] = skybox;
        return skybox;
    }

    const Core::Ref<FramebufferRenderPipeline>& Workspace::addRenderPipeline(const char *id, const Core::Ref<FramebufferRenderPipeline>& renderPipeline) {
        m_renderPipelines[id] = renderPipeline;
        return renderPipeline;
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

    const Core::Ref<Scene::DirectionalLightComponent>& Workspace::getDirectionalLight(const char* id) {

        if (id == nullptr) {
            return m_nullDirectionalLight;
        }

        auto iterator = m_directionalLights.find(id);

        if (iterator == m_directionalLights.end()) {
            throw std::runtime_error("DirectionalLight not found");
        }

        return iterator->second;

    }

    const Core::Ref<Scene::SkyboxComponent>& Workspace::getSkybox(const char *id) {

        if (id == nullptr) {
            return m_nullSkybox;
        }

        auto iterator = m_skyboxes.find(id);

        if (iterator == m_skyboxes.end()) {
            throw std::runtime_error("Skybox not found");
        }

        return iterator->second;

    }

    const Core::Ref<FramebufferRenderPipeline>& Workspace::getRenderPipeline(const char *id) {

        auto iterator = m_renderPipelines.find(id);

        if (iterator == m_renderPipelines.end()) {
            throw std::runtime_error("Skybox not found");
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

    RenderPanel* Workspace::getHoveredRenderPanel() {

        for (auto& renderPanelSpec : m_renderPanelSpecs) {

            if (renderPanelSpec.m_panel->isHovered()) {
                return renderPanelSpec.m_panel;
            }

        }

        return nullptr;

    }

    RenderPanel* Workspace::getFocusedRenderPanel() {

        for (auto& renderPanelSpec : m_renderPanelSpecs) {

            if (renderPanelSpec.m_panel->isHovered()) {
                return renderPanelSpec.m_panel;
            }

        }

        return nullptr;

    }

    const Core::Ref<Tool>& Workspace::getTool(const char* id) {

        for (auto& tool : m_tools) {
            if (tool->m_id == id) {
                return tool;
            }
        }

        throw std::runtime_error("Tool not found");

    }

    const std::vector<Core::Ref<Tool>>& Workspace::getTools() {
        return m_tools;
    }

    const char* Workspace::getActiveToolId() {
        return m_activeToolId;
    }

    Core::Ref<Tool> Workspace::getActiveTool() {
        return m_activeToolId != nullptr ? getTool(m_activeToolId) : nullptr;
    }

    Scene::Entity Workspace::getSelectedSceneEntity() {
        return m_selectedSceneEntity;
    }

    Scene::Entity Workspace::getSelectedUIEntity() {
        return m_selectedUIEntity;
    }

    Scene::Entity Workspace::mousePickSceneEntity(RenderPanel* renderPanel) {
        return mousePickTaggedEntity<SceneEntityTag>(renderPanel, FramebufferLayer::SceneEntity);
    }

    Scene::Entity Workspace::mousePickUIEntity(RenderPanel* renderPanel) {
        return mousePickTaggedEntity<UIEntityTag>(renderPanel, FramebufferLayer::UIEntity);
    }

    glm::vec3 Workspace::mousePickPoint(RenderPanel* renderPanel, glm::vec3 planePoint, glm::vec3 planeNormal) {

        auto window = gaunlet::Core::Window::getCurrentInstance();
        auto camera = getCamera(renderPanel->getCameraId());

        glm::mat4 projection = camera->getProjectionMatrix();
        glm::mat4 view = camera->getViewMatrix();

        auto ray = camera->ray(
            {renderPanel->getMousePositionX(), window->getMousePositionY()},
            {renderPanel->getWidth(), renderPanel->getHeight()}
        );

        auto intersection = camera->rayPlaneIntersection(
            camera->getPosition(),
            ray,
            planePoint,
            planeNormal
        );

        return intersection;

    }

    void Workspace::onEvent(Core::Event& event) {

        // Mouse and scroll will be ignored if the cursor is not hovering any render panel
        if (event.getCategory() == Core::EventCategory::Mouse || event.getCategory() == Core::EventCategory::Scroll) {

            bool renderPanelHovered = false;
            for (auto renderPanelSpec : m_renderPanelSpecs) {
                renderPanelHovered = renderPanelHovered || renderPanelSpec.m_panel->isHovered();
            }

            if (!renderPanelHovered) {
                return;
            }

        }

        // Delegate the event to the active tool
        if (m_activeToolId) {
            auto& activeTool = getTool(m_activeToolId);
            activeTool->onEvent(event);
        }

    }

    void Workspace::update(Core::TimeStep timeStep) {

        if (m_activeToolId) {
            auto& activeTool = getTool(m_activeToolId);
            activeTool->onUpdate(timeStep);
        }

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
        for (unsigned int i = 0; i < m_renderPanelSpecs.size(); i++) {

            auto& renderPanelSpec = m_renderPanelSpecs[i];

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
                ImVec2(renderPanelSpec.m_panel->getWidth(), renderPanelSpec.m_panel->getHeight()),
                ImVec2(0, 1), ImVec2(1, 0)
            );

            ImGui::End();
            ImGui::PopStyleVar(1);

        }

    }

    void Workspace::addTool(const char* id, const Core::Ref<Tool>& tool) {
        tool->m_workspace = this;
        tool->m_id = id;
        m_tools.push_back(tool);
    }

    void Workspace::activateTool(const char* id) {

        // Stop the current active tool, if any
        auto previousActiveTool = getActiveTool();
        if (previousActiveTool) {
            previousActiveTool->stop();
        }

        m_activeToolId = id;

        // Start the newly active tool, if any
        auto newActiveTool = getActiveTool();
        if (newActiveTool) {
            newActiveTool->start();
        }

    }

    void Workspace::selectSceneEntity(Scene::Entity entity) {

        bool changed = false;

        if (m_selectedSceneEntity != entity) {
            changed = true;
        }

        auto previousEntity = m_selectedSceneEntity;
        m_selectedSceneEntity = entity;

        // If we have newly selected an entity
        if (changed && m_selectedSceneEntity) {
            m_selectedSceneEntity.addEmptyComponent<ModelOutlineTag>();
            m_selectedSceneEntity.addEmptyComponent<WireframeModelTag>();
        }

        // If an entity has been unselected
        if (changed && previousEntity) {
            previousEntity.removeComponent<ModelOutlineTag>();
            previousEntity.removeComponent<WireframeModelTag>();
        }

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
        node->m_isFocused = ImGui::IsWindowFocused();
        node->m_mousePositionX = std::max(mousePosition.x - windowMin.x, 0.0f);
        node->m_mousePositionY = std::max(mousePosition.y - windowMin.y, 0.0f);
        node->m_mousePositionYInverted = windowMax.y - node->m_mousePositionY;

    }

}