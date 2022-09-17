#pragma once

#include "gaunlet/editor/tooling/Tool.h"
#include "gaunlet/core/window/Input.h"
#include "gaunlet/core/window/KeyCodes.h"
#include "gaunlet/editor/workspace/Workspace.h"

namespace gaunlet::Prefab::EditorTools {

    class FirstPersonCameraController : public Editor::Tool {

    public:

        FirstPersonCameraController(const char* renderPanelId)
            : FirstPersonCameraController(renderPanelId, 2.0f, 15.0f) {}

        FirstPersonCameraController(const char* renderPanelId, float movementSensitivity, float rotationSensitivity)
            : m_renderPanelId(renderPanelId), m_movementSensitivity(movementSensitivity), m_rotationSensitivity(rotationSensitivity) {}

        const char * getName() override {
            return "First Person Camera Controller";
        }

        void onUpdate(Core::TimeStep timeStep) override {

            auto renderPanel = getWorkspace()->getRenderPanel(m_renderPanelId);
            auto& camera = getWorkspace()->getCamera(renderPanel->getCameraId());

            float movement = 1.0f * timeStep * m_movementSensitivity;
            float rotation = 1.0f * timeStep * m_rotationSensitivity;

            // ROTATION
            if (Core::Input::isKeyPressed(GL_KEY_UP)) {
                camera->addRotation(0, rotation);
            }

            if (Core::Input::isKeyPressed(GL_KEY_DOWN)) {
                camera->addRotation(0, -rotation);
            }

            if (Core::Input::isKeyPressed(GL_KEY_RIGHT)) {
                camera->addRotation(rotation, 0);
            }

            if (Core::Input::isKeyPressed(GL_KEY_LEFT)) {
                camera->addRotation(-rotation, 0);
            }

            // MOVEMENT
            if (Core::Input::isKeyPressed(GL_KEY_W)) {
                camera->moveRelative({0, 0, movement});
            }

            if (Core::Input::isKeyPressed(GL_KEY_S)) {
                camera->moveRelative({0, 0, -movement});
            }

            if (Core::Input::isKeyPressed(GL_KEY_D)) {
                camera->moveRelative({movement, 0, 0});
            }

            if (Core::Input::isKeyPressed(GL_KEY_A)) {
                camera->moveRelative({-movement, 0, 0});
            }

        }

    private:

        const char* m_renderPanelId;
        float m_movementSensitivity;
        float m_rotationSensitivity;

    };

}