#pragma once

#include "gaunlet/editor/panel/Panel.h"
#include "gaunlet/editor/workspace/Workspace.h"
#include "gaunlet/pch.h"

namespace gaunlet::Editor {

    class WorkspacePropertiesPanel : public GuiPanel {

    public:

        void onGuiRender() override {

            if (ImGui::CollapsingHeader("Window Properties")) {
                windowProperties();
            }

            panels();

            if (ImGui::CollapsingHeader("Selection")) {
                selectionProperties();
            }

        }

    private:

        void windowProperties() {

            ImGui::Text("Window size: (%d, %d)", Core::Window::getCurrentInstance()->getWidth(), Core::Window::getCurrentInstance()->getHeight());
            ImGui::Text("Viewport size: (%d, %d)", Core::Window::getCurrentInstance()->getViewportWidth(), Core::Window::getCurrentInstance()->getViewportHeight());
            ImGui::Text("Mouse position: (%d, %d)", Core::Input::getMousePositionX(), Core::Input::getMousePositionY());

        }

        void selectionProperties() {

            auto selectedSceneEntity = getWorkspace()->getSelectedSceneEntity();
            if (selectedSceneEntity) {
                const char* sceneEntityLabel = selectedSceneEntity.hasName() ? selectedSceneEntity.getName() : "Unnamed entity";
                ImGui::Text("Selected Scene Entity: %s", sceneEntityLabel);
                entityProperties(selectedSceneEntity);
            } else {
                ImGui::Text("Selected Scene Entity: None");
            }

            auto selectedUIEntity = getWorkspace()->getSelectedUIEntity();
            if (selectedUIEntity) {
                const char* uiEntityLabel = selectedUIEntity.hasName() ? selectedUIEntity.getName() : "Unnamed entity";
                ImGui::Text("Selected UI Entity: %s", uiEntityLabel);
                entityProperties(selectedUIEntity);
            } else {
                ImGui::Text("Selected UI Entity: None");
            }

        }

        void panels() {

            if (ImGui::CollapsingHeader("GUI Panels")) {
                for (auto& guiPanel : getWorkspace()->getGuiPanels()) {
                    if (ImGui::TreeNodeEx(guiPanel->getId())) {
                        panelProperties(guiPanel);
                        ImGui::TreePop();
                    }
                }
            }

            if (ImGui::CollapsingHeader("Render Panels")) {
                for (auto& renderPanel : getWorkspace()->getRenderPanels()) {
                    if (ImGui::TreeNodeEx(renderPanel->getId())) {
                        panelProperties(renderPanel);
                        renderPanelProperties(renderPanel);
                        ImGui::TreePop();
                    }
                }
            }

        }

        void panelProperties(Panel* panel) {

            ImGui::Text("Size: (%d %d)", panel->getWidth(),  panel->getHeight());

            ImGui::Text("Mouse position: ");
            if (panel->isHovered()) {
                ImGui::SameLine();
                ImGui::Text("(%d, %d)", panel->getMousePositionX(), panel->getMousePositionY());
            }

        }

        void renderPanelProperties(RenderPanel* renderPanel) {

            ImGui::Text("Pixel position: ");
            if (renderPanel->isHovered()) {
                ImGui::SameLine();
                ImGui::Text("(%d, %d)", (int) (renderPanel->getMousePositionX() * Core::Window::getCurrentInstance()->getDPI()), (int) (renderPanel->getMousePositionYInverted() * Core::Window::getCurrentInstance()->getDPI()));
            }

            ImGui::Text("Render Mode: %s", renderPanel->getRenderMove() == Scene::RenderMode::Faces ? "Faces" : "Wireframe");

            std::string cameraNodeLabel = std::string("Camera: " + std::string(renderPanel->getCameraId() ));
            if (ImGui::TreeNodeEx(cameraNodeLabel.c_str())) {
                cameraProperties(getWorkspace()->getCamera(renderPanel->getCameraId()));
                ImGui::TreePop();
            }

            std::string sceneNodeLabel = std::string("Scene: " + std::string(renderPanel->getSceneId() ));
            if (ImGui::TreeNodeEx(sceneNodeLabel.c_str())) {
                sceneProperties(getWorkspace()->getScene(renderPanel->getSceneId()));
                ImGui::TreePop();
            }

            std::string directionalLightNodeLabel = std::string("Directional Light: " + std::string(renderPanel->getDirectionalLightId() ));
            if (ImGui::TreeNodeEx(directionalLightNodeLabel.c_str())) {
                directionalLightProperties(getWorkspace()->getDirectionalLight(renderPanel->getDirectionalLightId()));
                ImGui::TreePop();
            }

        }

        void cameraProperties(const Core::Ref<Scene::Camera>& camera) {

            const char* projectionType = "Unknown";
            if (camera->getProjectionType() == Scene::Camera::ProjectionType::Orthographic) {
                projectionType = "Orthographic";
            } else if (camera->getProjectionType() == Scene::Camera::ProjectionType::Perspective) {
                projectionType = "Perspective";
            }

            ImGui::Text("Projection type: %s", projectionType);
            ImGui::Text("Position: (%f, %f, %f)", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
            ImGui::Text("Yaw, Pitch: (%f, %f)", camera->getYaw(), camera->getPitch());
            ImGui::Text("Zoom Level: %f", camera->getZoomLevel());

        }

        void sceneProperties(const Core::Ref<Scene::Scene>& scene) {

            ImGui::Text("Scene Entities: %d", scene->getRegistry().countTaggedEntities<SceneEntityTag>());
            ImGui::Text("UI Entities: %d", scene->getRegistry().countTaggedEntities<UIEntityTag>());

        }

        void directionalLightProperties(const Core::Ref<Scene::DirectionalLightComponent>& directionalLight) {

            glm::vec4 color = glm::vec4(directionalLight->m_color, 1.0f);
            ImGui::Text("Color: "); ImGui::SameLine(); ImGui::ColorButton("Color", *(ImVec4*)&color);
            ImGui::Text("Position: (%f, %f, %f)", directionalLight->m_direction.x, directionalLight->m_direction.y, directionalLight->m_direction.z);

        }

        void entityProperties(Scene::Entity entity) {

            ImGui::Text("Components:");

            if (entity.hasComponent<Scene::TransformComponent>()) {
                auto& transform = entity.getComponent<Scene::TransformComponent>();
                ImGui::Text("Transform:");
                ImGui::Text("Translation: (%f %f %f)", transform.m_translation.x, transform.m_translation.y, transform.m_translation.z);
                ImGui::Text("Rotation: (%f %f %f)", transform.m_rotation.x, transform.m_rotation.y, transform.m_rotation.z);
            }

            ImGui::Text("Tags: "); ImGui::SameLine();
            if (entity.hasComponent<Editor::SceneEntityTag>()) {
                ImGui::Text("SceneEntity"); ImGui::SameLine();
            }
            if (entity.hasComponent<Editor::UIEntityTag>()) {
                ImGui::Text("UIEntity"); ImGui::SameLine();
            }

            ImGui::NewLine();

        }

    };

}