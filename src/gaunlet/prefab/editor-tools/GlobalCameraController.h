#pragma once

#include "gaunlet/editor/tooling/Tool.h"
#include "gaunlet/core/window/Input.h"
#include "gaunlet/core/window/KeyCodes.h"
#include "gaunlet/core/event/events/MouseEvent.h"
#include "gaunlet/core/event/events/CursorEvent.h"
#include "gaunlet/core/event/events/ScrollEvent.h"
#include "gaunlet/editor/workspace/Workspace.h"

namespace gaunlet::Prefab::EditorTools {

    class GlobalCameraController : public Editor::Tool {

    public:

        GlobalCameraController()
            : GlobalCameraController(0.01f, 0.02f) {}

        GlobalCameraController(float zoomSensitivity, float panSensitivity)
            : m_zoomSensitivity(zoomSensitivity), m_panSensitivity(panSensitivity) {}

        float m_zoomSensitivity;
        float m_panSensitivity;
        const char* m_renderPanelId = nullptr;
        bool m_moving = false;
        glm::vec2 m_initialPosition = {};
        glm::vec2 m_finalPosition = {};

        const char* getName() override {
            return "Camera Controller";
        }

        bool onEvent(gaunlet::Core::Event &event) override {

            gaunlet::Core::EventDispatcher dispatcher(event);
            dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GL_BIND_CALLBACK_FN(GlobalCameraController::onMousePressEvent));
            dispatcher.dispatch<gaunlet::Core::MouseButtonRelease>(GL_BIND_CALLBACK_FN(GlobalCameraController::onMouseReleaseEvent));
            dispatcher.dispatch<gaunlet::Core::CursorMoveEvent>(GL_BIND_CALLBACK_FN(GlobalCameraController::onCursorMoveEvent));
            dispatcher.dispatch<gaunlet::Core::ScrollEvent>(GL_BIND_CALLBACK_FN(GlobalCameraController::onScrollEvent));

            return true;

        }

    protected:

        bool onMousePressEvent(gaunlet::Core::MouseButtonPress& event) {

            m_renderPanelId = getWorkspace()->getHoveredRenderPanel();
            if (!m_renderPanelId) {
                return true;
            }

            m_moving = true;

            m_initialPosition = {
                getWorkspace()->getRenderPanel(m_renderPanelId)->getMousePositionX(),
                getWorkspace()->getRenderPanel(m_renderPanelId)->getMousePositionY()
            };

            return true;
        }

        bool onMouseReleaseEvent(gaunlet::Core::MouseButtonRelease& event) {

            m_moving = false;

            return true;
        }

        bool onCursorMoveEvent(gaunlet::Core::CursorMoveEvent& event) {

            if (!m_moving) {
                return true;
            }

            glm::vec2 currentPosition = {
                getWorkspace()->getRenderPanel(m_renderPanelId)->getMousePositionX(),
                getWorkspace()->getRenderPanel(m_renderPanelId)->getMousePositionY()
            };

            glm::vec2 delta = m_initialPosition - currentPosition;

            // Alt (aka "option"): rotate
            if (gaunlet::Core::Input::isKeyPressed(GL_KEY_LEFT_ALT) || gaunlet::Core::Input::isKeyPressed(GL_KEY_RIGHT_ALT)) {

                getWorkspace()->getCamera(m_renderPanelId)->addRotation(
                    delta.x / 10,
                    -delta.y / 10
                );

                // Shift: orbit
            } else if (gaunlet::Core::Input::isKeyPressed(GL_KEY_LEFT_SHIFT) || gaunlet::Core::Input::isKeyPressed(GL_KEY_RIGHT_SHIFT)) {

                if (getWorkspace()->getSelectedSceneEntity()) {
                    getWorkspace()->getCamera(m_renderPanelId)->orbit(
                        getWorkspace()->getSelectedSceneEntity().getComponent<gaunlet::Scene::TransformComponent>(),
                        -delta.y / 10, // Moving the mouse vertically, rotates around the X axis
                        -delta.x / 10 // Moving the mouse horizontally, rotates around the Y axis
                    );
                } else {
                    getWorkspace()->getCamera(m_renderPanelId)->orbit(
                        5.0f,
                        -delta.y / 10, // Moving the mouse vertically, rotates around the X axis
                        -delta.x / 10 // Moving the mouse horizontally, rotates around the Y axis
                    );
                }

                // Drag: pan
            } else {
                getWorkspace()->getCamera(m_renderPanelId)->moveRelative({
                    delta.x * m_panSensitivity,
                    -delta.y * m_panSensitivity,
                    0
                });
            }

            m_initialPosition = currentPosition;

            return true;

        }

        bool onScrollEvent(gaunlet::Core::ScrollEvent& event) {

            m_renderPanelId = getWorkspace()->getHoveredRenderPanel();

            getWorkspace()->getCamera(m_renderPanelId)->addZoomLevel(
                -event.getYOffset() * m_zoomSensitivity
            );

            return true;

        }

    };

}