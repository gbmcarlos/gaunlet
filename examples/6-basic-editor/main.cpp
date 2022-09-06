#include "../include/Editor.h"

class TransformerTool : public gaunlet::Editor::Tool {

public:

    const char* getName() override {
        return "Transformer";
    }

    void onGuiRender() override {

        ImGui::Text("State: %s", m_moving ? "Transforming" : "Idle");

        const char* handleLabel = gaunlet::Editor::TranslationGizmo::convert(m_handle);
        ImGui::Text("Handle: %s", handleLabel);

    }

    void start() override {

        auto selectedSceneEntity = getWorkspace()->getSelectedSceneEntity();
        if (selectedSceneEntity) {
            m_gizmo = gaunlet::Editor::TranslationGizmo::create(getWorkspace()->getScene("main")->getRegistry());
            selectedSceneEntity.addChild(m_gizmo);
        }

    }

    void stop() override {

        if (m_gizmo) {
            m_gizmo.destroy();
        }

    }

    bool onEvent(gaunlet::Core::Event &event) {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GL_BIND_CALLBACK_FN(TransformerTool::onMousePressEvent));
        dispatcher.dispatch<gaunlet::Core::MouseButtonRelease>(GL_BIND_CALLBACK_FN(TransformerTool::onMouseReleaseEvent));
        dispatcher.dispatch<gaunlet::Core::CursorMoveEvent>(GL_BIND_CALLBACK_FN(TransformerTool::onCursorMoveEvent));

        return true;

    }

private:

    bool m_moving = false;
    gaunlet::Scene::Entity m_gizmo = {};
    gaunlet::Editor::TranslationGizmo::Handle m_handle = gaunlet::Editor::TranslationGizmo::Handle::None;

    bool onMousePressEvent(gaunlet::Core::MouseButtonPress& event) {

        auto uiEntity = getWorkspace()->mousePickUIEntity("main");
        getWorkspace()->selectUiEntity(uiEntity);

        m_handle = gaunlet::Editor::TranslationGizmo::convert(uiEntity.getName());

        return true;

    }

    bool onMouseReleaseEvent(gaunlet::Core::MouseButtonRelease& event) {

        m_moving = false;
        m_handle = gaunlet::Editor::TranslationGizmo::Handle::None;

        return true;

    }

    bool onCursorMoveEvent(gaunlet::Core::CursorMoveEvent& event) {

        if (!m_moving) {
            return true;
        }

        return true;

    }

};

class SelectorTool : public gaunlet::Editor::Tool {

    const char* getName() override {
        return "Selector";
    }

    bool onEvent(gaunlet::Core::Event &event) {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GL_BIND_CALLBACK_FN(SelectorTool::onMousePressEvent));

        return true;

    }

    bool onMousePressEvent(gaunlet::Core::MouseButtonPress& event) {

        auto selectedSceneEntity = getWorkspace()->mousePickSceneEntity("main");
        getWorkspace()->selectSceneEntity(selectedSceneEntity);

        return true;
    }

};

class CameraControllerTool : public gaunlet::Editor::Tool {

public:

    enum class State {
        Idle, Moving
    };

    State m_state = State::Idle;
    glm::vec2 m_initialPosition = {};
    glm::vec2 m_finalPosition = {};

    const char* getName() override {
        return "Camera Controller";
    }

    bool onEvent(gaunlet::Core::Event &event) {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GL_BIND_CALLBACK_FN(CameraControllerTool::onMousePressEvent));
        dispatcher.dispatch<gaunlet::Core::MouseButtonRelease>(GL_BIND_CALLBACK_FN(CameraControllerTool::onMouseReleaseEvent));
        dispatcher.dispatch<gaunlet::Core::CursorMoveEvent>(GL_BIND_CALLBACK_FN(CameraControllerTool::onCursorMoveEvent));
        dispatcher.dispatch<gaunlet::Core::ScrollEvent>(GL_BIND_CALLBACK_FN(CameraControllerTool::onScrollEvent));

        return true;

    }

protected:

    bool onMousePressEvent(gaunlet::Core::MouseButtonPress& event) {

        m_state = State::Moving;

        m_initialPosition = {
            gaunlet::Core::Input::getMousePositionX(),
            gaunlet::Core::Input::getMousePositionY()
        };

        return true;
    }

    bool onMouseReleaseEvent(gaunlet::Core::MouseButtonRelease& event) {

        m_state = State::Idle;

        return true;
    }

    bool onCursorMoveEvent(gaunlet::Core::CursorMoveEvent& event) {

        if (m_state != State::Moving) {
            return true;
        }

        glm::vec2 currentPosition = {
            gaunlet::Core::Input::getMousePositionX(),
            gaunlet::Core::Input::getMousePositionY()
        };

        glm::vec2 delta = m_initialPosition - currentPosition;

        // Alt (aka "option"): rotate
        if (gaunlet::Core::Input::isKeyPressed(GL_KEY_LEFT_ALT) || gaunlet::Core::Input::isKeyPressed(GL_KEY_RIGHT_ALT)) {

            getWorkspace()->getCamera("main")->addRotation(
                delta.x / 10,
                -delta.y / 10
            );

        // Shift: orbit
        } else if (gaunlet::Core::Input::isKeyPressed(GL_KEY_LEFT_SHIFT) || gaunlet::Core::Input::isKeyPressed(GL_KEY_RIGHT_SHIFT)) {

            if (getWorkspace()->getSelectedSceneEntity()) {
                getWorkspace()->getCamera("main")->orbit(
                    getWorkspace()->getSelectedSceneEntity().getComponent<gaunlet::Scene::TransformComponent>(),
                    -delta.y / 10, // Moving the mouse vertically, rotates around the X axis
                    -delta.x / 10 // Moving the mouse horizontally, rotates around the Y axis
                );
            } else {
                getWorkspace()->getCamera("main")->orbit(
                    5.0f,
                    -delta.y / 10, // Moving the mouse vertically, rotates around the X axis
                    -delta.x / 10 // Moving the mouse horizontally, rotates around the Y axis
                );
            }

        // Drag: pan
        } else {
            getWorkspace()->getCamera("main")->moveRelative({
                delta.x / 50,
                -delta.y / 50,
                0
            });
        }

        m_initialPosition = currentPosition;

        return true;

    }

    bool onScrollEvent(gaunlet::Core::ScrollEvent& event) {

        getWorkspace()->getCamera("main")->addZoomLevel(
            0.1f * -event.getYOffset()
        );

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
                {gaunlet::Editor::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar},
                {gaunlet::Editor::DockSpacePosition::Right, 0.25f,  {"Tools Manager"}},
            }, viewportWidth, viewportHeight
        });

        // Create and push the GUI panels
        m_workspace->pushPanel("settings", new gaunlet::Editor::WorkspacePropertiesPanel, "Workspace Properties");
        m_workspace->pushPanel("tools", new gaunlet::Editor::ToolsManagerPanel, "Tools Manager");

        // Prepare the components of the main render panel
        m_workspace->addCamera("main", gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth / (float) viewportHeight, 1.0f, 100.0f));
        m_workspace->addScene("main", gaunlet::Core::CreateRef<gaunlet::Scene::Scene>());
        m_workspace->addDirectionalLight("main", gaunlet::Core::CreateRef<gaunlet::Scene::DirectionalLightComponent>(
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(2.0f, 5.0f, 4.0f),
            0.2f, 0.5f
        ));
        m_workspace->addSkybox("main", gaunlet::Core::CreateRef<gaunlet::Scene::SkyboxComponent>(gaunlet::Core::CreateRef<gaunlet::Scene::SimpleSkyboxCubeMap>()));

        // Create and push the main render panel, referencing the main components
        m_workspace->pushPanel(
            "main",
            new gaunlet::Editor::RenderPanel(),
            "Scene",
            "main",
            "main",
            "main",
            "main",
            gaunlet::Scene::RenderMode::Faces
        );

        // Create and push the tools
        m_workspace->addTool("camera-controller", gaunlet::Core::CreateRef<CameraControllerTool>());
        m_workspace->addTool("selector", gaunlet::Core::CreateRef<SelectorTool>());
        m_workspace->addTool("transformer", gaunlet::Core::CreateRef<TransformerTool>());

        // Prepare the scene
        auto& mainScene = m_workspace->getScene("main");
        auto& mainCamera = m_workspace->getCamera("main");
        mainCamera->setPosition({0.0f, 4.0f, 6.0f});
        mainCamera->setZoomLevel(1.5f);
        mainCamera->lookAt({0, 2, 0});

        auto cup = mainScene->getRegistry().createTaggedEntity<gaunlet::Editor::SceneEntityTag>("Cup");
        cup.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Model("assets/cup/cup.obj"));
        cup.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.5f)
        );
        cup.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {
        m_workspace->update(timeStep);
    }

    void onGuiRender() override {
        m_workspace->render();
    }

    void onEvent(gaunlet::Core::Event &event) {
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