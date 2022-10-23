#pragma once

#include "gaunlet/editor/panel/Panel.h"
#include "gaunlet/editor/workspace/Workspace.h"
#include "gaunlet/scene/components/BasicComponents.h"

#include "experimental/iterator"

namespace gaunlet::Prefab::GuiPanels {

    class EntityComponentsPanel : public Editor::GuiPanel {

    public:

        void onGuiRender() override {

            auto selectedSceneEntity = getWorkspace()->getSelectedSceneEntity();
            if (selectedSceneEntity) {

                ImGui::SliderFloat("Slider Speed: ", &m_sliderSpeed, 0.1f, 10.0f);

                sections(selectedSceneEntity);

            }

        }

    protected:

        float m_sliderSpeed = 1.0f;

        virtual void sections(Scene::Entity entity) {

            // name, tags, transform, model, circle, material, terrain

            ImGui::Text("Name: %s", entity.hasName() ? entity.getName() : "");

            tagComponents(entity);

            if (entity.hasComponent<Scene::TransformComponent>()) {
                transformComponentProperties(entity.getComponent<Scene::TransformComponent>());
            }

            if (entity.hasComponent<Components::ModelComponent>()) {
                modelComponentProperties(entity.getComponent<Components::ModelComponent>());
            }

            if (entity.hasComponent<Components::CircleComponent>()) {
                circleComponentProperties(entity.getComponent<Components::CircleComponent>());
            }

            if (entity.hasComponent<Scene::MaterialComponent>()) {
                materialComponentProperties(entity.getComponent<Scene::MaterialComponent>());
            }

        }

    private:

        void tagComponents(Scene::Entity entity) {

            std::vector<std::string> tags;

            if (entity.hasComponent<Editor::ModelOutlineTag>()) {
                tags.emplace_back("ModelOutline");
            }

            if (entity.hasComponent<Editor::WireframeModelTag>()) {
                tags.emplace_back("WireframeModel");
            }

            std::ostringstream outstream;
            std::copy(
                tags.begin(),
                tags.end(),
                std::experimental::make_ostream_joiner(outstream, ", ")
            );

            ImGui::Text("Tags: %s", outstream.str().c_str());

        }

        void transformComponentProperties(Scene::TransformComponent& component) {

            if (ImGui::CollapsingHeader("Transform Component")) {
                ImGui::DragFloat3("Translation: ", &component.m_translation.x, m_sliderSpeed);
                ImGui::DragFloat3("Rotation: ", &component.m_rotation.x, m_sliderSpeed);
                ImGui::DragFloat3("Scale: ", &component.m_scale.x, m_sliderSpeed);
            }

        }

        void modelComponentProperties(Components::ModelComponent& model) {

            if (ImGui::CollapsingHeader("Model Component")) {

                if (model.getMeshGenerator()) {
                    model.getMeshGenerator()->onGuiRender();
                } else {
                    ImGui::Text("Vertices: %lu", model.m_model.m_meshes[0].getVertices().size());
                    ImGui::Text("Indices: %lu", model.m_model.m_meshes[0].getIndices().size());
                }

            }

        }

        void circleComponentProperties(Components::CircleComponent& circle) {

            if (ImGui::CollapsingHeader("Circle Component")) {
                ImGui::SliderFloat("Thickness: ", &circle.m_thickness, 0.01f, 1.0f);
                ImGui::SliderFloat("Fade: ", &circle.m_fade, 0.1f, 1.0f);
            }

        }

        void materialComponentProperties(Scene::MaterialComponent& material) {

            if (ImGui::CollapsingHeader("Material Component")) {
                ImGui::ColorPicker4("Color: ", &material.m_color.x);
                ImGui::Text("Has Texture: %s", material.m_texture != nullptr ? "Yes" : "No");
            }

        }

    };

}