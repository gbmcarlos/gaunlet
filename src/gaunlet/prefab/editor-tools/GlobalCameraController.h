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

        bool m_moving;
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

            m_moving = true;

            m_initialPosition = {
                getWorkspace()->getRenderPanel("main")->getMousePositionX(),
                getWorkspace()->getRenderPanel("main")->getMousePositionY()
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
                getWorkspace()->getRenderPanel("main")->getMousePositionX(),
                getWorkspace()->getRenderPanel("main")->getMousePositionY()
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

}