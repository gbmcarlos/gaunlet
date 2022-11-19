#include "gaunlet/prefab/editor-tools/first-person-camera-controller/FirstPersonCameraController.h"

#include "gaunlet/core/window/Input.h"
#include "gaunlet/core/window/KeyCodes.h"

namespace gaunlet::Prefab::EditorTools {

    FirstPersonCameraController::FirstPersonCameraController(const char* renderPanelId)
        : FirstPersonCameraController(renderPanelId, 2.0f, 1.0f) {}

    FirstPersonCameraController::FirstPersonCameraController(const char* renderPanelId, float movementSensitivity, float rotationSensitivity)
        : m_renderPanelId(renderPanelId), m_movementSensitivity(movementSensitivity), m_rotationSensitivity(rotationSensitivity) {}

    const char* FirstPersonCameraController::getName() {
        return "First Person Camera Controller";
    }

    void FirstPersonCameraController::onGuiRender() {
        ImGui::SliderFloat("Movement Sensitivity: ", &m_movementSensitivity, 0.1f, 1000.0f);
        ImGui::SliderFloat("Rotation Sensitivity: ", &m_rotationSensitivity, 0.1f, 10.0f);
    }

    void FirstPersonCameraController::onUpdate(Core::TimeStep timeStep) {

        selectRenderPanel();
        if (m_renderPanel == nullptr) {
            return;
        }

        auto& camera = m_renderPanel->getCamera();

        float movement = timeStep * m_movementSensitivity;
        float rotation = timeStep * m_rotationSensitivity;

        bool shiftPressed = Core::Input::isKeyPressed(GL_KEY_LEFT_SHIFT) || Core::Input::isKeyPressed(GL_KEY_RIGHT_SHIFT);

        // MOVEMENT
        if (Core::Input::isKeyPressed(GL_KEY_UP)) {
            if (!shiftPressed) {
                camera->moveSemiRelative({0, movement, 0});
            } else {
                camera->moveRelative({0, movement, 0});
            }
        }

        if (Core::Input::isKeyPressed(GL_KEY_DOWN)) {
            if (!shiftPressed) {
                camera->moveSemiRelative({0, -movement, 0});
            } else {
                camera->moveRelative({0, -movement, 0});
            }
        }

        // MOVEMENT
        if (Core::Input::isKeyPressed(GL_KEY_W)) {
            if (!shiftPressed) {
                camera->moveSemiRelative({0, 0, -movement});
            } else {
                camera->moveRelative({0, 0, -movement});
            }
        }

        if (Core::Input::isKeyPressed(GL_KEY_S)) {
            if (!shiftPressed) {
                camera->moveSemiRelative({0, 0, movement});
            } else {
                camera->moveRelative({0, 0, movement});
            }
        }

        if (Core::Input::isKeyPressed(GL_KEY_D)) {
            camera->moveRelative({movement, 0, 0});
        }

        if (Core::Input::isKeyPressed(GL_KEY_A)) {
            camera->moveRelative({-movement, 0, 0});
        }

        // ROTATION
        if (m_rotating) {
            camera->rotate({0, 1, 0, -m_rotationDelta.x * rotation}); // yaw
            camera->rotate({camera->getRight(), -m_rotationDelta.y * rotation}); // pitch
        }

    }

    bool FirstPersonCameraController::onEvent(gaunlet::Core::Event& event) {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GL_BIND_CALLBACK_FN(FirstPersonCameraController::onMousePressEvent));
        dispatcher.dispatch<gaunlet::Core::MouseButtonRelease>(GL_BIND_CALLBACK_FN(FirstPersonCameraController::onMouseReleaseEvent));
        dispatcher.dispatch<gaunlet::Core::CursorMoveEvent>(GL_BIND_CALLBACK_FN(FirstPersonCameraController::onCursorMoveEvent));

        return true;

    }

    bool FirstPersonCameraController::onMousePressEvent(gaunlet::Core::MouseButtonPress& event) {

        selectRenderPanel();
        if (m_renderPanel == nullptr) {
            return true;
        }

        m_rotating = true;

        m_initialPosition = {
            m_renderPanel->getMousePositionX(),
            m_renderPanel->getMousePositionY()
        };
        m_rotationDelta = {0, 0};

        return true;

    }

    bool FirstPersonCameraController::onMouseReleaseEvent(gaunlet::Core::MouseButtonRelease& event) {

        m_rotating = false;
        return true;

    }

    bool FirstPersonCameraController::onCursorMoveEvent(gaunlet::Core::CursorMoveEvent& event) {

        selectRenderPanel();
        if (m_renderPanel == nullptr) {
            return true;
        }

        if (!m_rotating) {
            return true;
        }

        glm::vec2 currentPosition = {
            m_renderPanel->getMousePositionX(),
            m_renderPanel->getMousePositionY()
        };

        m_rotationDelta = m_initialPosition - currentPosition;

        return true;

    }

    void FirstPersonCameraController::selectRenderPanel() {

        if (m_renderPanelId) {

            // If a render panel id was specified, make sure it's focused
            Editor::RenderPanel* renderPanel = getWorkspace()->getRenderPanel(m_renderPanelId);
            if (renderPanel->isFocused()) {
                m_renderPanel = renderPanel;
            } else {
                m_renderPanel = nullptr;
            }

        } else {
            m_renderPanel = getWorkspace()->getFocusedRenderPanel();
        }

    }

}