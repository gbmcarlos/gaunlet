#include "../include/Editor.h"

class FPCameraController : public gaunlet::Editor::Tool {

public:

    enum class State {
        Idle, Moving
    };

    State m_state = State::Idle;
    glm::vec2 m_initialPosition = {};
    glm::vec2 m_finalPosition = {};

    bool onEvent(gaunlet::Core::Event &event) {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GL_BIND_CALLBACK_FN(FPCameraController::onMousePressEvent));
        dispatcher.dispatch<gaunlet::Core::MouseButtonRelease>(GL_BIND_CALLBACK_FN(FPCameraController::onMouseReleaseEvent));
        dispatcher.dispatch<gaunlet::Core::CursorMoveEvent>(GL_BIND_CALLBACK_FN(FPCameraController::onCursorMoveEvent));
        dispatcher.dispatch<gaunlet::Core::ScrollEvent>(GL_BIND_CALLBACK_FN(FPCameraController::onScrollEvent));

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

            getWorkspace()->getCamera("main")->orbit(
                -delta.y / 10, // Moving the mouse vertically, rotates around the X axis
                -delta.x / 10 // Moving the mouse horizontally, rotates around the Y axis
            );

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
                {gaunlet::Editor::DockSpacePosition::Left, 0.25f,  {"Workspace Properties"}},
                {gaunlet::Editor::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar}
            }, viewportWidth, viewportHeight
        });

        // Create and push the GUI panels
        m_workspace->pushPanel("settings", new gaunlet::Editor::WorkspacePropertiesPanel, "Workspace Properties");

        // Prepare the components of the main render panel
        m_workspace->addCamera("main", gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth / (float) viewportHeight, 1.0f, 100.0f));
        m_workspace->addScene("main", gaunlet::Core::CreateRef<gaunlet::Scene::Scene>());
        m_workspace->addDirectionalLight("main", gaunlet::Scene::DirectionalLightComponent(
            {0.8f, 0.8f, 0.8f},
            {2.0f, 5.0f, 4.0f},
            0.2f, 0.5f
        ));

        // Create and push the main render panel, referencing the main components
        m_workspace->pushPanel(
            "main",
            new gaunlet::Editor::RenderPanel(),
            "Scene",
            "main",
            "main",
            "main",
            gaunlet::Scene::RenderMode::Faces
        );

        // Create, push, and activate the camera controller tool
        m_workspace->addTool("camera-controller", gaunlet::Core::CreateRef<FPCameraController>());
        m_workspace->activateTool("camera-controller");

        // Prepare the scene
        auto& mainScene = m_workspace->getScene("main");
        auto& mainCamera = m_workspace->getCamera("main");
        mainCamera->setPosition({0.0f, 4.0f, 6.0f});
        mainCamera->setZoomLevel(1.5f);
        mainCamera->lookAt({0, 2, 0});

        auto cup = mainScene->getRegistry().createTaggedEntity<gaunlet::Editor::SceneEntityTag>();
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