#pragma once

#include "gaunlet/editor/tooling/Tool.h"
#include "gaunlet/editor/workspace/Workspace.h"
#include "gaunlet/core/event/events/MouseEvent.h"
#include "gaunlet/core/event/events/CursorEvent.h"

namespace gaunlet::Prefab::EditorTools {

    class FirstPersonCameraController : public Editor::Tool {

    public:

        explicit FirstPersonCameraController(const char* renderPanelId);
        FirstPersonCameraController(const char* renderPanelId, float movementSensitivity, float rotationSensitivity);

        virtual const char* getName() override;
        virtual void onGuiRender() override;
        virtual void onUpdate(Core::TimeStep timeStep) override;

        bool onEvent(gaunlet::Core::Event &event) override;
        bool onMousePressEvent(gaunlet::Core::MouseButtonPress& event);
        bool onMouseReleaseEvent(gaunlet::Core::MouseButtonRelease& event);
        bool onCursorMoveEvent(gaunlet::Core::CursorMoveEvent& event);

        void selectRenderPanel();

    protected:

        const char* m_renderPanelId;
        Editor::RenderPanel* m_renderPanel;
        float m_movementSensitivity;
        float m_rotationSensitivity;
        bool m_rotating = false;
        glm::vec2 m_initialPosition = {};
        glm::vec2 m_rotationDelta = {};

    };

}