#pragma once

#include "gaunlet/editor/panel/Panel.h"
#include "gaunlet/scene/camera/Camera.h"
#include "gaunlet/editor/workspace/Workspace.h"

namespace gaunlet::Prefab::GuiPanels {

    class RenderPanelComponentsPanel : public Editor::GuiPanel {

    public:

        void onGuiRender() override {

            if (ImGui::BeginCombo("tools", "Select Render Panel")) {

                for (auto& renderPanel: getWorkspace()->getRenderPanels()) {

                    if (ImGui::Selectable(renderPanel->getId(), renderPanel->getId() == m_selectedRenderPanelId)) {
                        m_selectedRenderPanelId = renderPanel->getId();
                    }

                }

                ImGui::EndCombo();
            }

            if (m_selectedRenderPanelId != nullptr) {
                sections(getWorkspace()->getRenderPanel(m_selectedRenderPanelId));
            }

        }

    private:

        const char* m_selectedRenderPanelId = nullptr;

        void sections(Editor::RenderPanel* renderPanel) {

            panelProperties(renderPanel);
            sceneProperties(renderPanel->getScene());
            cameraProperties(renderPanel->getCamera());
//            directionalLightProperties(getWorkspace()->getDirectionalLight(renderPanel->getDirectionalLightId()));

        }

        void panelProperties(Editor::RenderPanel* renderPanel) {

            ImGui::Text("ID: %s", renderPanel->getId());
            ImGui::Text("Size: (%d %d)", renderPanel->getWidth(),  renderPanel->getHeight());

            ImGui::Text("Mouse position: ");
            if (renderPanel->isHovered()) {
                ImGui::SameLine();
                ImGui::Text("(%d, %d)", renderPanel->getMousePositionX(), renderPanel->getMousePositionY());
            }

            ImGui::Text("Pixel position: ");
            if (renderPanel->isHovered()) {
                ImGui::SameLine();
                ImGui::Text("(%d, %d)", (int) (renderPanel->getMousePositionX() * Core::Window::getCurrentInstance()->getDPI()), (int) (renderPanel->getMousePositionYInverted() * Core::Window::getCurrentInstance()->getDPI()));
            }

        }

        void sceneProperties(const Core::Ref<Scene::Scene>& scene) {

            if (ImGui::CollapsingHeader("Scene Properties")) {
                ImGui::Text("Scene Entities: %d", scene->countTaggedEntities<Editor::SceneEntityTag>());
                ImGui::Text("UI Entities: %d", scene->countTaggedEntities<Editor::UIEntityTag>());
            }

        }

        void cameraProperties(const Core::Ref<Scene::Camera>& camera) {

            if (ImGui::CollapsingHeader("Camera")) {
                const char* projectionType = "Unknown";
                if (camera->getProjectionType() == Scene::Camera::ProjectionType::Orthographic) {
                    projectionType = "Orthographic";
                } else if (camera->getProjectionType() == Scene::Camera::ProjectionType::Perspective) {
                    projectionType = "Perspective";
                }

                ImGui::Text("Projection type: %s", projectionType);
                ImGui::Text("Aspect Ratio: %f", camera->getAspectRatio());
                ImGui::DragFloat3("Position: ", &camera->getPosition().x);
                ImGui::SliderFloat("Yaw: ", &camera->getYaw(), 0.0f, 360.0f);
                ImGui::SliderFloat("Pitch: ", &camera->getPitch(), -89.0f, 89.0f);
                ImGui::SliderFloat("Zoom Level: ", &camera->getZoomLevel(), camera->getMinZoomLevel(), camera->getMaxZoomLevel());
            }

        }

        void directionalLightProperties(const Core::Ref<Scene::DirectionalLightComponent>& directionalLight) {

            if (ImGui::CollapsingHeader("Directional Light")) {
                ImGui::ColorEdit3("Color: ", &directionalLight->m_color.x);
                ImGui::DragFloat3("Direction: ", &directionalLight->m_direction.x);
            }

        }

    };

}