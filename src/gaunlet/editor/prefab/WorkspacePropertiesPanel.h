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

        }

    private:

        void windowProperties() {

            ImGui::Text("Window size: (%d, %d)", Core::Window::getCurrentInstance()->getWidth(), Core::Window::getCurrentInstance()->getHeight());
            ImGui::Text("Viewport size: (%d, %d)", Core::Window::getCurrentInstance()->getViewportWidth(), Core::Window::getCurrentInstance()->getViewportHeight());
            ImGui::Text("Mouse position: (%d, %d)", Core::Input::getMousePositionX(), Core::Input::getMousePositionY());

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

        void directionalLightProperties(Scene::DirectionalLightComponent& directionalLight) {

            glm::vec4 color = glm::vec4(directionalLight.m_color, 1.0f);
            ImGui::Text("Color: "); ImGui::SameLine(); ImGui::ColorButton("Color", *(ImVec4*)&color);
            ImGui::Text("Position: (%f, %f, %f)", directionalLight.m_position.x, directionalLight.m_position.y, directionalLight.m_position.z);

        }

    };

}