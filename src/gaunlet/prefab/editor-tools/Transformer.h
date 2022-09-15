#pragma once

#include "gaunlet/editor/tooling/SelectionTool.h"
#include "gaunlet/editor/gizmos/TranslationGizmo.h"
#include "gaunlet/core/event/events/CursorEvent.h"

namespace gaunlet::Prefab::EditorTools {

    class TransformerTool : public Editor::SelectionTool {

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

        bool m_moving;
        glm::vec3 m_entityInitialPosition;
        glm::vec3 m_handleInitialPosition;
        glm::vec3 m_handlePosition;
        glm::vec3 m_movement;
        gaunlet::Scene::Entity m_gizmo = {};
        gaunlet::Editor::TranslationGizmo::Handle m_handle = gaunlet::Editor::TranslationGizmo::Handle::PlaneXY;

        bool onMousePressEvent(gaunlet::Core::MouseButtonPress& event) {

            m_moving = false;
            auto uiEntity = selectUIEntity("main");

            if (uiEntity) {

                // Save the handle, the scene entity's and the handle's current position,
                m_handle = gaunlet::Editor::TranslationGizmo::convert(uiEntity.getName());
                m_entityInitialPosition = getSelectedSceneEntity().getComponent<gaunlet::Scene::TransformComponent>().m_translation;
                m_handleInitialPosition = getWorkspace()->mousePickPoint(
                    "main",
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
                selectSceneEntity("main");
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
                "main",
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

}