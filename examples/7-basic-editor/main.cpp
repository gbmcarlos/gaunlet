#include "../include/Editor.h"

class FPCameraController {

public:

    enum class State {
        Idle, Moving
    };

    State m_state = State::Idle;
    glm::vec2 m_initialPosition = {};
    glm::vec2 m_finalPosition = {};
    gaunlet::Core::Ref<gaunlet::Scene::Camera> m_camera;

    bool onEvent(gaunlet::Core::Event &event) {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GL_BIND_CALLBACK_FN(FPCameraController::onMousePressEvent));
        dispatcher.dispatch<gaunlet::Core::MouseButtonRelease>(GL_BIND_CALLBACK_FN(FPCameraController::onMouseReleaseEvent));
        dispatcher.dispatch<gaunlet::Core::CursorMoveEvent>(GL_BIND_CALLBACK_FN(FPCameraController::onCursorMoveEvent));

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

        if (m_state != State::Moving || m_camera == nullptr) {
            return true;
        }

        glm::vec2 currentPosition = {
            gaunlet::Core::Input::getMousePositionX(),
            gaunlet::Core::Input::getMousePositionY()
        };

        glm::vec2 delta = m_initialPosition - currentPosition;

        // Alt (aka "option"): rotate
        if (gaunlet::Core::Input::isKeyPressed(GL_KEY_LEFT_ALT) || gaunlet::Core::Input::isKeyPressed(GL_KEY_RIGHT_ALT)) {

            m_camera->addRotation(
                delta.x / 10,
                -delta.y / 10
            );

        // Shift: orbit
        } else if (gaunlet::Core::Input::isKeyPressed(GL_KEY_LEFT_SHIFT) || gaunlet::Core::Input::isKeyPressed(GL_KEY_RIGHT_SHIFT)) {

            m_camera->orbit(
                -delta.y / 10, // Moving the mouse vertically, rotates around the X axis
                -delta.x / 10 // Moving the mouse horizontally, rotates around the Y axis
            );

        } else {
            m_camera->moveRelative({
                delta.x / 50,
                -delta.y / 50,
                0
            });
        }

        m_initialPosition = currentPosition;

        return true;

    }

};

class ScenePanel : public gaunlet::Editor::RenderPanel {

public:

    FPCameraController m_cameraController;

    void onReady() override {
        m_cameraController.m_camera = getCamera();
    }

    bool onEvent(gaunlet::Core::Event &event) override {

        return m_cameraController.onEvent(event);

    }

};

class SettingsPanel : public gaunlet::Editor::GuiPanel {

public:

    explicit SettingsPanel(ScenePanel* scenePanel) : m_scenePanel(scenePanel) {}

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getNodeWidth(), getNodeHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        } else {
            ImGui::NewLine();
        }

        ImGui::Text("Scene layer:");

        if (m_scenePanel->isHovered()) {
            ImGui::Text("%d %d", m_scenePanel->getMousePositionX(), m_scenePanel->getMousePositionYInverted());
        } else {
            ImGui::NewLine();
        }

        ImGui::Text("Camera control state: ");
        if (m_scenePanel->m_cameraController.m_state == FPCameraController::State::Idle) {
            ImGui::Text("Idle");
        } else if (m_scenePanel->m_cameraController.m_state == FPCameraController::State::Moving) {
            ImGui::Text("Moving");
        }

    }


private:
    ScenePanel* m_scenePanel = nullptr;

};

class ViewportLayoutApplication : public gaunlet::Core::Application {

public:
    explicit ViewportLayoutApplication(const std::string &name) : gaunlet::Core::Application(name) {}

    void onReady() override {

        // Create and set up the layer that will show the different dock nodes
        m_editorLayer = new gaunlet::Editor::LayoutLayer(m_window);
        m_editorLayer->setLayoutSpec({
{
                    {gaunlet::Editor::DockSpacePosition::Left, 0.25f,  {"Settings"}},
                    {gaunlet::Editor::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar},
               }, m_window->getViewportWidth(), m_window->getViewportHeight()
        });

        auto* scenePanel = new ScenePanel();

        m_editorLayer->pushPanel("Settings", new SettingsPanel(scenePanel));
        m_editorLayer->pushPanel("Scene", scenePanel);

        scenePanel->getCamera()->setPosition({0.0f, 4.0f, 6.0f});
        scenePanel->getCamera()->lookAt({0, 2, 0});

        auto cup = scenePanel->getScene().getRegistry().createTaggedEntity<gaunlet::Editor::SceneEntityTag>();
        cup.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Model("assets/cup/cup.obj"));
        cup.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.5f)
        );
        cup.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

        auto triangle = scenePanel->getScene().getRegistry().createTaggedEntity<gaunlet::Editor::SceneEntityTag>();
        triangle.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Triangle2DModel());
        triangle.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, 4.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        triangle.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

        scenePanel->startScene();

        pushLayer(m_editorLayer);
    }

private:
    gaunlet::Editor::LayoutLayer* m_editorLayer = nullptr;

};

int main() {

    ViewportLayoutApplication app("Viewport Layout");
    gaunlet::Core::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}