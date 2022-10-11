#pragma once

#include "gaunlet/editor/docking/DockingLayout.h"
#include "gaunlet/scene/entity/Entity.h"
#include "gaunlet/scene/camera/Camera.h"
#include "gaunlet/editor/panel/Panel.h"
#include "gaunlet/editor/panel/RenderPanel.h"
#include "gaunlet/editor/tooling/Tool.h"
#include "gaunlet/core/window/Window.h"
#include "gaunlet/editor/render-pipeline/RenderPipeline.h"
#include "gaunlet/pch.h"

namespace gaunlet::Editor {

    struct WorkspaceConfiguration {

        bool addModelOutlineTagOnSelect;
        bool addWireframeModelTagOnSelect;

    };

    class Workspace {

    public:

        Workspace();
        Workspace(const WorkspaceConfiguration& configuration);

        void setLayoutSpec(const DockingLayoutSpec& layoutSpec);

        GuiPanel* pushPanel(const char* panelId, GuiPanel* panel, const char* windowId);
        RenderPanel* pushPanel(const char* panelId, RenderPanel* panel, const char* windowId, const char* cameraId, const char* sceneId, const char* renderPipelineId);

        const Core::Ref<Scene::Scene>& addScene(const char* id, const Core::Ref<Scene::Scene>& scene);
        const Core::Ref<Scene::Camera>& addCamera(const char* id, const Core::Ref<Scene::Camera>& camera);
        const Core::Ref<RenderPipeline>& addRenderPipeline(const char* id, const Core::Ref<RenderPipeline>& renderPipeline);

        void onEvent(Core::Event& event);
        void update(Core::TimeStep timeStep);
        void render();

        void addTool(const char* id, const Core::Ref<Tool>& tool);
        void activateTool(const char* id);

        void selectSceneEntity(Scene::Entity entity);
        void selectUiEntity(Scene::Entity entity);

        const Core::Ref<Scene::Scene>& getScene(const char* id);
        const Core::Ref<Scene::Camera>& getCamera(const char* id);
        const Core::Ref<RenderPipeline>& getRenderPipeline(const char* id);

        std::vector<GuiPanel*> getGuiPanels();
        std::vector<RenderPanel*> getRenderPanels();
        GuiPanel* getGuiPanel(const char* id);
        RenderPanel* getRenderPanel(const char* id);
        RenderPanel* getHoveredRenderPanel();
        RenderPanel* getFocusedRenderPanel();

        const Core::Ref<Tool>& getTool(const char* id);
        const std::vector<Core::Ref<Tool>>& getTools();
        const char* getActiveToolId();
        Core::Ref<Tool> getActiveTool();

        Scene::Entity getSelectedSceneEntity();
        Scene::Entity getSelectedUIEntity();

        glm::vec3 mousePickPoint(RenderPanel* renderPanel, glm::vec3 planePoint, glm::vec3 planeNormal);

        WorkspaceConfiguration m_configuration;

    private:
        void updateNodeProperties(Panel* node);

    private:
        DockingLayoutSpec m_layoutSpec;

        std::unordered_map<const char*, Core::Ref<Scene::Scene>> m_scenes = {};
        std::unordered_map<const char*, Core::Ref<Scene::Camera>> m_cameras = {};
        std::unordered_map<const char*, Core::Ref<RenderPipeline>> m_renderPipelines;

        struct GuiPanelSpec {
            GuiPanel* m_panel;
            const char* m_panelId;
            const char* m_windowId;
        };

        struct RenderPanelSpec {
            RenderPanel* m_panel;
            const char* m_panelId;
            const char* m_windowId;
        };

        std::vector<GuiPanelSpec> m_guiPanelSpecs = {};
        std::vector<RenderPanelSpec> m_renderPanelSpecs = {};

        std::vector<Core::Ref<Tool>> m_tools;
        const char* m_activeToolId = nullptr;

        Scene::Entity m_selectedSceneEntity = {};
        Scene::Entity m_selectedUIEntity = {};

    };

}