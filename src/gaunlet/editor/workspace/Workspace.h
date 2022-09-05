#pragma once

#include "gaunlet/editor/docking/DockingLayout.h"
#include "gaunlet/scene/Scene.h"
#include "gaunlet/scene/camera/Camera.h"
#include "gaunlet/editor/panel/Panel.h"
#include "gaunlet/editor/panel/RenderPanel.h"
#include "gaunlet/editor/tooling/Tool.h"
#include "gaunlet/pch.h"

namespace gaunlet::Editor {

    class Workspace {

    public:

        void setLayoutSpec(const DockingLayoutSpec& layoutSpec);

        void pushPanel(const char* panelId, GuiPanel* panel, const char* windowId);
        void pushPanel(const char* panelId, RenderPanel* panel, const char* windowId, const char* cameraId, const char* sceneId, const char* directionalLightId, Scene::RenderMode renderMode);

        void addCamera(const char* id, const Core::Ref<Scene::Camera>& camera);
        void addScene(const char* id, const Core::Ref<Scene::Scene>& scene);
        void addDirectionalLight(const char* id, Scene::DirectionalLightComponent directionalLight);

        void onEvent(Core::Event& event);
        void update(Core::TimeStep timeStep);
        void render();

        void addTool(const char* id, const Core::Ref<Tool>& tool);
        void activateTool(const char* id);

        void selectSceneEntity(Scene::Entity entity);
        void selectUiEntity(Scene::Entity entity);

        const Core::Ref<Scene::Camera>& getCamera(const char* id);
        const Core::Ref<Scene::Scene>& getScene(const char* id);
        Scene::DirectionalLightComponent& getDirectionalLight(const char* id);

        std::vector<GuiPanel*> getGuiPanels();
        std::vector<RenderPanel*> getRenderPanels();
        GuiPanel* getGuiPanel(const char* id);
        RenderPanel* getRenderPanel(const char* id);

        const Core::Ref<Tool>& getTool(const char* id);

        Scene::Entity getSelectedSceneEntity();
        Scene::Entity getSelectedUIEntity();

    private:
        void updateNodeProperties(Panel* node);

    private:
        DockingLayoutSpec m_layoutSpec;

        std::unordered_map<const char*, Core::Ref<Scene::Camera>> m_cameras = {};
        std::unordered_map<const char*, Core::Ref<Scene::Scene>> m_scenes = {};
        std::unordered_map<const char*, Scene::DirectionalLightComponent> m_directionalLights = {};

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

        std::unordered_map<const char*, Core::Ref<Tool>> m_tools;
        const char* m_activeToolId = nullptr;

        Scene::Entity m_selectedSceneEntity = {};
        Scene::Entity m_selectedUIEntity = {};

    };

}