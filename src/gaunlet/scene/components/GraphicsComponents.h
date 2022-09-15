#pragma once

#include "gaunlet/scene/model/Model.h"
#include "gaunlet/prefab/sprites//Sprites.h"
#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/graphics/texture/TextureCubeMap.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    struct TransformComponent {

        TransformComponent() : m_translation(glm::vec3(0.0f, 0.0f, 0.0f)), m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)), m_scale(glm::vec3(1.0f, 1.0f, 1.0f)) {}
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(glm::vec3 position,  glm::vec3 rotation, glm::vec3 scale) : m_translation(position), m_rotation(rotation), m_scale(scale) {}

        glm::vec3 m_translation;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        glm::mat4 getTransformationMatrix() const {

            glm::mat4 translationsMatrix = glm::translate(glm::mat4(1.0f), m_translation);

            glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), {1, 0, 0});
            glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), {0, 1, 0});
            glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), {0, 0, 1});

            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

            return translationsMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;
        }

    };

    struct ModelComponent {

        ModelComponent(const ModelComponent&) = default;
        explicit ModelComponent(Model model) : m_model(std::move(model)) {}

        std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() {

            std::vector<Graphics::Vertex> vertices = {};
            std::vector<unsigned int> indices = {};

            // Get the vertices and indices of all the meshes of the model
            for (auto& mesh : m_model.getMeshes()) {
                vertices.insert(vertices.end(), mesh.getVertices().begin(), mesh.getVertices().end());
                indices.insert(indices.end(), mesh.getIndices().begin(), mesh.getIndices().end());
            }

            return {vertices, indices};

        }

        Model m_model;

    };

    struct CircleComponent {

        CircleComponent(const CircleComponent&) = default;

        // A circle is drawn as a square (a single mesh), with a different shader
        CircleComponent() :
            CircleComponent(0.5, 0.01f) {}

        CircleComponent(float thickness) :
            CircleComponent(thickness, 0.01f) {}

        CircleComponent(float thickness, float fade) :
            m_mesh(Prefab::Sprites::Square2DModel().getMeshes()[0]),
            m_thickness(thickness),
            m_fade(fade) {}

        std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() {
            return {m_mesh.getVertices(), m_mesh.getIndices()};
        }

        Mesh m_mesh;
        float m_thickness;
        float m_fade;

    };

    struct MaterialComponent {

        MaterialComponent(const MaterialComponent&) = default;
        MaterialComponent() : m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), m_texture(nullptr) {}
        explicit MaterialComponent(glm::vec4 color) : m_color(color), m_texture(nullptr) {}
        explicit MaterialComponent(Core::Ref<Graphics::Texture> texture) : m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), m_texture(std::move(texture)) {}
        MaterialComponent(glm::vec4 color, Core::Ref<Graphics::Texture> texture) : m_color(color), m_texture(std::move(texture)) {}

        glm::vec4 m_color;
        Core::Ref<Graphics::Texture> m_texture;

    };

    struct SkyboxComponent {

        SkyboxComponent()
            : m_cubeMap(nullptr) {}
        explicit SkyboxComponent(Core::Ref<Graphics::TextureCubeMap> cubeMap)
            : m_cubeMap(std::move(cubeMap)) {}
        explicit SkyboxComponent(const std::vector<const char*>& filePaths)
            : m_cubeMap(Core::CreateRef<Graphics::TextureCubeMap>(filePaths)) {}

        Core::Ref<Graphics::TextureCubeMap> m_cubeMap = nullptr;

    };

    struct PlaneComponent {

        PlaneComponent(int patchCountX, int patchCountZ, float patchWidth, float patchDepth)
            : m_mesh(createMesh(patchCountX, patchCountZ, patchWidth, patchDepth)) {
        }

        std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() {
            return {m_mesh.getVertices(), m_mesh.getIndices()};
        }

        /*
         * Create a set of squares, on the XZ plane, in rows (X) and columns (Z)
         * Go from -totalWidth/2 (left) and -totalWidth/2 (far) to totalWidth/2 (right) and totalDepth/2 (near)
         * So, looking at the plane from above, go from the top right corner, toward the right and toward the bottom
         */
        static Mesh createMesh(unsigned int patchCountX, unsigned int patchCountZ, float patchWidth, float patchDepth) {

            std::vector<Graphics::Vertex> vertices;
            std::vector<unsigned int> indices;

            // The width and depth of the whole plane
            float totalWidth = patchWidth * (float) patchCountX;
            float totalDepth = patchDepth * (float) patchCountZ;

            // The width and depth (in UV coordinates) of a single patch
            float patchUVWidth = (1/totalWidth) * patchWidth;
            float patchUVDepth = (1/totalDepth) * patchDepth;

            unsigned int indexOffset = 0;

            // For each row, from left to right
            for (unsigned int i = 0; i < patchCountX; i++) {

                // For each column in the row, from bottom to top
                for (unsigned int j = 0; j < patchCountZ; j++) {

                    /*
                     * The 4 edges of the patch
                     * Calculate the position of the center of the patch, as if the center of the plane was at the left-bottom corner
                     * Then move the whole thing half-plane to the left and down, to keep the local center at the center of the plane
                     */
                    float left = ((float)i * patchWidth) - (totalWidth/2);
                    float bottom = - (((float)j * patchDepth) - (totalDepth/2)); // Negate it, because down in the Z axis is positive
                    float right = left + patchWidth;
                    float top = bottom - patchDepth; // Up in the Z axis is negative

                    // The UV coordinates of the 4 edges of the patch
                    float leftUV = (float) i * patchUVWidth;
                    float bottomUV = (float) j * patchUVDepth;
                    float rightUV = leftUV + patchUVWidth;
                    float topUV = bottomUV + patchUVDepth;

                    glm::vec3 normal = {0, 1, 0};

                    vertices.push_back({{left, 0, bottom, 1}, normal, {leftUV, bottomUV}});
                    vertices.push_back({{right, 0, bottom, 1}, normal, {rightUV, bottomUV}});
                    vertices.push_back({{right, 0, top, 1}, normal, {rightUV, topUV}});
                    vertices.push_back({{left, 0, top, 1}, normal, {leftUV, topUV}});

                    indices.push_back(0 + indexOffset);
                    indices.push_back(1 + indexOffset);
                    indices.push_back(2 + indexOffset);
                    indices.push_back(2 + indexOffset);
                    indices.push_back(3 + indexOffset);
                    indices.push_back(0 + indexOffset);

                    indexOffset += 4;

                }

            }

            return {vertices, indices};

        }

        Mesh m_mesh;

    };

}