#pragma once

#include "gaunlet/scene/components/BasicComponents.h"
#include "gaunlet/scene/model/Model.h"

namespace gaunlet::Prefab::Components {

    struct ModelComponent : public Scene::ContentComponent {

        ModelComponent(const ModelComponent&) = default;
        explicit ModelComponent(Scene::Model model) : m_model(std::move(model)) {}

        std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() override {

            std::vector<Graphics::Vertex> vertices = {};
            std::vector<unsigned int> indices = {};

            // Get the vertices and indices of all the meshes of the model
            for (auto& mesh : m_model.getMeshes()) {
                vertices.insert(vertices.end(), mesh.getVertices().begin(), mesh.getVertices().end());
                indices.insert(indices.end(), mesh.getIndices().begin(), mesh.getIndices().end());
            }

            return {vertices, indices};

        }

        Scene::Model m_model;

    };

}