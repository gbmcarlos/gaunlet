#pragma once

#include "gaunlet/scene/components/BasicComponents.h"
#include "gaunlet/scene/model/Model.h"
#include "gaunlet/scene/model/MeshGenerator.h"

namespace gaunlet::Prefab::Components {

    struct ModelComponent : public Scene::ContentComponent {

        ModelComponent(const ModelComponent&) = default;
        explicit ModelComponent(Scene::Model model) : m_model(std::move(model)) {}
        explicit ModelComponent(Core::Ref<Scene::MeshGenerator> meshGenerator) : m_meshGenerator(std::move(meshGenerator)) {}

        Core::Ref<Scene::MeshGenerator>& getMeshGenerator() {
            return m_meshGenerator;
        }

        std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() override {

            if (m_meshGenerator) {
                return m_meshGenerator->getContent();
            }

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
        Core::Ref<Scene::MeshGenerator> m_meshGenerator = nullptr;

    };

}