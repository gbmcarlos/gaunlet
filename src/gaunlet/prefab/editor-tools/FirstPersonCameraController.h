#pragma once

#include "gaunlet/editor/tooling/Tool.h"
#include "gaunlet/core/window/Input.h"
#include "gaunlet/core/window/KeyCodes.h"
#include "gaunlet/editor/workspace/Workspace.h"
#include "gaunlet/core/event/events/MouseEvent.h"
#include "gaunlet/core/event/events/CursorEvent.h"

namespace gaunlet::Prefab::EditorTools {

    class FirstPersonCameraController : public Editor::Tool {

    public:

        FirstPersonCameraController(const char* renderPanelId)
            : FirstPersonCameraController(renderPanelId, 2.0f, 1.0f) {}

        FirstPersonCameraController(const char* renderPanelId, float movementSensitivity, float rotationSensitivity)
            : m_renderPanelId(renderPanelId), m_movementSensitivity(movementSensitivity), m_rotationSensitivity(rotationSensitivity) {}

        const char * getName() override {
            return "First Person Camera Controller";
        }

        void onUpdate(Core::TimeStep timeStep) override {

            selectRenderPanel();
            if (m_renderPanel == nullptr) {
                return;
            }

            auto& camera = getWorkspace()->getCamera(m_renderPanel->getCameraId());

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
                camera->addRotation(-m_rotationDelta.x * rotation, m_rotationDelta.y * rotation);
            }

        }

        bool onEvent(gaunlet::Core::Event &event) override {

            gaunlet::Core::EventDispatcher dispatcher(event);
            dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GL_BIND_CALLBACK_FN(FirstPersonCameraController::onMousePressEvent));
            dispatcher.dispatch<gaunlet::Core::MouseButtonRelease>(GL_BIND_CALLBACK_FN(FirstPersonCameraController::onMouseReleaseEvent));
            dispatcher.dispatch<gaunlet::Core::CursorMoveEvent>(GL_BIND_CALLBACK_FN(FirstPersonCameraController::onCursorMoveEvent));

            return true;

        }

        bool onMousePressEvent(gaunlet::Core::MouseButtonPress& event) {

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

        bool onMouseReleaseEvent(gaunlet::Core::MouseButtonRelease& event) {

            m_rotating = false;
            return true;

        }

        bool onCursorMoveEvent(gaunlet::Core::CursorMoveEvent& event) {

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

        void selectRenderPanel() {

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

    private:

        const char* m_renderPanelId;
        Editor::RenderPanel* m_renderPanel;
        float m_movementSensitivity;
        float m_rotationSensitivity;
        bool m_rotating = false;
        glm::vec2 m_initialPosition = {};
        glm::vec2 m_rotationDelta = {};

    };

}