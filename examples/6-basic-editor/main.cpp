#include "../include/Editor.h"
#include "../include/Prefab.h"

class TransformerTool : public gaunlet::Prefab::EditorTools::EntitySelectionTool {

public:

    const char* getName() override {
        return "Transformer";
    }

    void onGuiRender() override {

        const char* handleLabel = gaunlet::Editor::TranslationGizmo::convert(m_handle);
        ImGui::Text("Handle: %s", handleLabel);
        ImGui::Text("Position: (%f %f %f)", m_handlePosition.x, m_handlePosition.y, m_handlePosition.z);
        ImGui::Text("Movement: (%f %f %f)", m_movement.x, m_movement.y, m_movement.z);

    }

    void start() override {

        if (getSelectedSceneEntity()) {
            createGizmo(getSelectedSceneEntity());
        }

    }

    void stop() override {
        destroyGizmo();
    }

    bool onEvent(gaunlet::Core::Event &event) override {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GL_BIND_CALLBACK_FN(TransformerTool::onMousePressEvent));
        dispatcher.dispatch<gaunlet::Core::MouseButtonRelease>(GL_BIND_CALLBACK_FN(TransformerTool::onMouseReleaseEvent));
        dispatcher.dispatch<gaunlet::Core::CursorMoveEvent>(GL_BIND_CALLBACK_FN(TransformerTool::onCursorMoveEvent));

        return true;

    }

protected:

    void onSceneEntitySelected(gaunlet::Scene::Entity sceneEntity) override {
        createGizmo(sceneEntity);
    }

    void onSceneEntityUnselected(gaunlet::Scene::Entity sceneEntity) override {
        destroyGizmo();
    }

private:

    bool m_moving = false;
    glm::vec3 m_entityInitialPosition;
    glm::vec3 m_handleInitialPosition;
    glm::vec3 m_handlePosition;
    glm::vec3 m_movement;
    gaunlet::Scene::Entity m_gizmo = {};
    gaunlet::Editor::TranslationGizmo::Handle m_handle = gaunlet::Editor::TranslationGizmo::Handle::PlaneXY;

    bool onMousePressEvent(gaunlet::Core::MouseButtonPress& event) {

        m_moving = false;
        auto uiEntity = selectUIEntity(getWorkspace()->getRenderPanel("main"));

        if (uiEntity) {

            // Save the handle, the scene entity's and the handle's current position,
            m_handle = gaunlet::Editor::TranslationGizmo::convert(uiEntity.getName());
            m_entityInitialPosition = getSelectedSceneEntity().getComponent<gaunlet::Scene::TransformComponent>().m_translation;
            m_handleInitialPosition = getWorkspace()->mousePickPoint(
                getWorkspace()->getRenderPanel("main"),
                getSelectedSceneEntity().getComponent<gaunlet::Scene::TransformComponent>().m_translation,
                gaunlet::Editor::TranslationGizmo::getPlaneNormal(m_handle)
            );
            m_handlePosition = m_handleInitialPosition;

        }

        return true;

    }

    bool onMouseReleaseEvent(gaunlet::Core::MouseButtonRelease& event) {

        // If the cursor hasn't moved between press and release, it's a simple click, so try to select an entity
        if (!m_moving) {
            selectSceneEntity(getWorkspace()->getRenderPanel("main"));
            return true;
        }

        // Otherwise, reset everything
        getWorkspace()->selectUiEntity({});
        m_handle = gaunlet::Editor::TranslationGizmo::Handle::None;
        m_movement = {0, 0, 0};
        m_moving = false;

        return true;

    }

    bool onCursorMoveEvent(gaunlet::Core::CursorMoveEvent& event) {

        m_moving = true;

        gaunlet::Scene::Entity handle = getWorkspace()->getSelectedUIEntity();

        // If we don't have any handle clicked, there's nothing to do
        if (!handle) {
            return true;
        }

        m_handlePosition = getWorkspace()->mousePickPoint(
            getWorkspace()->getRenderPanel("main"),
            getSelectedSceneEntity().getComponent<gaunlet::Scene::TransformComponent>().m_translation,
            gaunlet::Editor::TranslationGizmo::getPlaneNormal(m_handle)
        );

        if (glm::any(glm::isnan(m_handlePosition))) {
            return true;
        }

        m_movement = gaunlet::Editor::TranslationGizmo::constraintMovement(m_handle, m_handlePosition - m_handleInitialPosition);

        getSelectedSceneEntity().getComponent<gaunlet::Scene::TransformComponent>().m_translation = m_entityInitialPosition + m_movement;

        return true;

    }

    void createGizmo(gaunlet::Scene::Entity sceneEntity) {

        m_gizmo = gaunlet::Editor::TranslationGizmo::create(
            getWorkspace()->getScene("main"),
            1.5f, 0.1f,
            0.5f, 0.5f
        );
        sceneEntity.addChild(m_gizmo);

    }

    void destroyGizmo() {

        if (m_gizmo) {
            m_gizmo.destroy();
            m_gizmo = {};
        }

    }

};

class SelectorTool : public gaunlet::Prefab::EditorTools::EntitySelectionTool {

    const char* getName() override {
        return "Selector";
    }

    bool onEvent(gaunlet::Core::Event &event) override {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GL_BIND_CALLBACK_FN(SelectorTool::onMousePressEvent));

        return true;

    }

    bool onMousePressEvent(gaunlet::Core::MouseButtonPress& event) {

        selectSceneEntity(getWorkspace()->getRenderPanel("main"));

        return true;
    }

};

class BasicEditorApplication : public gaunlet::Core::Application {

public:
    explicit BasicEditorApplication() : gaunlet::Core::Application() {}

    void onReady() override {

        auto window = gaunlet::Core::Window::getCurrentInstance();
        unsigned int viewportWidth = window->getViewportWidth();
        unsigned int viewportHeight = window->getViewportHeight();

        m_workspace = new gaunlet::Editor::Workspace();

        // Set the docking layout
        m_workspace->setLayoutSpec({
            {
                {gaunlet::Editor::DockSpacePosition::Left, 0.2f,  {"Workspace Properties"}},
                {gaunlet::Editor::DockSpacePosition::Down, 0.4, 0, {"Entity Components"}},
                {gaunlet::Editor::DockSpacePosition::Down, 0.4, 0, {"Render Panel Properties"}},
                {gaunlet::Editor::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar},
                {gaunlet::Editor::DockSpacePosition::Right, 0.25f,  {"Tools Manager"}},
            }, viewportWidth, viewportHeight
        });

        // Create and push the GUI panels
        m_workspace->pushPanel("settings", new gaunlet::Prefab::GuiPanels::WorkspacePropertiesPanel, "Workspace Properties");
        m_workspace->pushPanel("render-panel", new gaunlet::Prefab::GuiPanels::RenderPanelComponentsPanel, "Render Panel Properties");
        m_workspace->pushPanel("components", new gaunlet::Prefab::GuiPanels::EntityComponentsPanel, "Entity Components");
        m_workspace->pushPanel("tools", new gaunlet::Prefab::GuiPanels::ToolsManagerPanel, "Tools Manager");

        // Prepare the components of the main render panel
        m_workspace->addCamera("main", gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth / (float) viewportHeight, 1.0f, 100.0f));
        m_workspace->addScene("main", gaunlet::Core::CreateRef<gaunlet::Scene::Scene>());
        m_workspace->addRenderPipeline("main", gaunlet::Core::CreateRef<gaunlet::Prefab::RenderPipelines::BasicEditorRenderPipeline>(
            gaunlet::Core::CreateRef<gaunlet::Scene::DirectionalLightComponent>(
                glm::vec3(0.8f, 0.8f, 0.8f),
                glm::vec3(-0.2f, -1.0f, -0.3f),
                0.5f, 0.7f
            ),
            gaunlet::Core::CreateRef<gaunlet::Scene::SkyboxComponent>(gaunlet::Core::CreateRef<gaunlet::Prefab::Skyboxes::SimpleSkyboxCubeMap>())
        ));

        // Create and push the main render panel, referencing the main components
        m_workspace->pushPanel(
            "main",
            new gaunlet::Editor::RenderPanel(),
            "Scene",
            "main",
            "main",
            "main"
        );

        // Create and push the tools
        m_workspace->addTool("selector", gaunlet::Core::CreateRef<SelectorTool>());
        m_workspace->addTool("fp-camera-controller", gaunlet::Core::CreateRef<gaunlet::Prefab::EditorTools::FirstPersonCameraController>("main"));
        m_workspace->addTool("transformer", gaunlet::Core::CreateRef<TransformerTool>());
        m_workspace->activateTool("fp-camera-controller");

        // Prepare the scene
        auto& mainScene = m_workspace->getScene("main");
        auto& mainCamera = m_workspace->getCamera("main");
        mainCamera->setPosition({0.0f, 4.0f, 6.0f});
        mainCamera->setZoomLevel(1.5f);
        mainCamera->lookAt({0, 2, 0});

        auto cup1 = mainScene->createTaggedEntity<gaunlet::Editor::SceneEntityTag>("Cup1");
        cup1.addComponent<gaunlet::Prefab::Components::ModelComponent>(gaunlet::Scene::Model("assets/cup/cup.obj"));
        cup1.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-2.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.5f)
        );
        cup1.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

        auto cup2 = mainScene->createTaggedEntity<gaunlet::Editor::SceneEntityTag>("Cup2");
        cup2.addComponent<gaunlet::Prefab::Components::ModelComponent>(gaunlet::Scene::Model("assets/cup/cup.obj"));
        cup2.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.5f)
        );
        cup2.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.2f, 0.2f, 0.8f, 1.0f));

    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {
        m_workspace->update(timeStep);
    }

    void onGuiRender() override {
        m_workspace->render();
    }

    void onEvent(gaunlet::Core::Event &event) override {
        m_workspace->onEvent(event);
    }

private:
    gaunlet::Editor::Workspace* m_workspace = nullptr;

};

int main() {

    auto window = gaunlet::Core::CreateRef<gaunlet::Core::Window>("Basic editor");
    gaunlet::Core::RunLoop runLoop(window);
    BasicEditorApplication app;

    runLoop.run(app);


    return 0;

}