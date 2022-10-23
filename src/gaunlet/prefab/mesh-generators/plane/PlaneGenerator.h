#pragma once

#include "gaunlet/scene/model/MeshGenerator.h"

namespace gaunlet::Prefab::MeshGenerators {

    class PlaneGenerator : public Scene::MeshGenerator {

    public:

        PlaneGenerator(glm::vec3 origin, glm::vec3 normal, float width, float height, unsigned int subdivisions)
            : m_origin(origin), m_normal(normal), m_width(width), m_height(height), m_subdivisions(subdivisions) {}

        void onGuiRender() override {

            ImGui::DragFloat3("Origin", &m_origin.x, 1.0f, -100.0f, 100.0f);
            ImGui::DragFloat3("Normal", &m_normal.x, 0.2f, -1.0f, 1.0f);
            ImGui::SliderFloat("Width", &m_width, 1.0f, 50.0f);
            ImGui::SliderFloat("Height", &m_height, 1.0f, 50.0f);
            ImGui::SliderInt("Subdivisions", (int*) &m_subdivisions, 0, 100);

        }

        std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() override {

            std::vector<Graphics::Vertex> vertices;
            std::vector<unsigned int> indices;

            auto normal = glm::normalize(m_normal);

            glm::vec3 axisA = {normal.y, normal.z, normal.x};
            glm::vec3 axisB = glm::cross(normal, axisA);

            float patchWidth = m_width / (float) (m_subdivisions + 1);
            float patchHeight = m_height / (float) (m_subdivisions + 1);
            float halfPatchWidth = patchWidth/2;
            float halfPatchHeight = patchHeight/2;

            glm::vec3 leftBottomCorner = m_origin - (axisA * (m_width/2)) - (axisB * (m_height/2));
            glm::vec3 patchOrigin = leftBottomCorner + (axisA * halfPatchWidth) + (axisB * halfPatchHeight);

            float patchWidthUV = 1 / m_width;
            float patchHeightUV = 1 / m_height;
            float halfPatchWidthUV = patchWidthUV / 2;
            float halfPatchHeightUV = patchHeightUV / 2;

            glm::vec2 patchOriginUV = {+halfPatchWidthUV, +halfPatchHeightUV};

            // For each row
            for (unsigned int i = 0; i < m_subdivisions + 1; i++) {

                // For each cell in the row
                for (unsigned int j = 0; j < m_subdivisions + 1; j++) {

                    unsigned int indexOffset = vertices.size();

                    glm::vec3 leftBottom    = patchOrigin - (axisA * halfPatchWidth) - (axisB * halfPatchHeight);
                    glm::vec3 rightBottom   = patchOrigin + (axisA * halfPatchWidth) - (axisB * halfPatchHeight);
                    glm::vec3 rightTop      = patchOrigin + (axisA * halfPatchWidth) + (axisB * halfPatchHeight);
                    glm::vec3 leftTop       = patchOrigin - (axisA * halfPatchWidth) + (axisB * halfPatchHeight);

                    glm::vec2 leftBottomUV  = {-halfPatchWidthUV, -halfPatchHeightUV};
                    glm::vec2 rightBottomUV = {+halfPatchWidthUV, -halfPatchHeightUV};
                    glm::vec2 rightTopUV    = {+halfPatchWidthUV, +halfPatchHeightUV};
                    glm::vec2 leftTopUV     = {-halfPatchWidthUV, +halfPatchHeightUV};

                    vertices.emplace_back(glm::vec4(leftBottom, 1), normal, patchOriginUV + leftBottomUV);
                    vertices.emplace_back(glm::vec4(rightBottom, 1), normal, patchOriginUV + rightBottomUV);
                    vertices.emplace_back(glm::vec4(rightTop, 1), normal, patchOriginUV + rightTopUV);
                    vertices.emplace_back(glm::vec4(leftTop, 1), normal, patchOriginUV + leftTopUV);

                    indices.emplace_back(0 + indexOffset);
                    indices.emplace_back(1 + indexOffset);
                    indices.emplace_back(2 + indexOffset);
                    indices.emplace_back(2 + indexOffset);
                    indices.emplace_back(3 + indexOffset);
                    indices.emplace_back(0 + indexOffset);

                    // Move the coordinates to the next patch in the row, to the right
                    patchOrigin += axisA * patchWidth;
                    patchOriginUV += glm::vec2(patchWidthUV, 0);

                }

                // Move the coordinates to the first patch on the next row
                // First move up one patch, and then move it left N patches
                patchOrigin += axisB * patchHeight;
                patchOrigin += axisA * -(patchWidth * ((float) m_subdivisions + 1));
                patchOriginUV += glm::vec2(0, patchHeightUV);
                patchOriginUV += -glm::vec2(patchWidthUV * ((float) m_subdivisions + 1), 0);

            }

            return {vertices, indices};

        }


    private:

        glm::vec3 m_origin;
        glm::vec3 m_normal;
        float m_width;
        float m_height;
        unsigned int m_subdivisions;

    };

}