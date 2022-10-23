#pragma once

#include "gaunlet/scene/model/MeshGenerator.h"

namespace gaunlet::Prefab::MeshGenerators {

    class PlaneSphereGenerator : public Scene::MeshGenerator {

    public:

        PlaneSphereGenerator(float radius, unsigned int resolution, glm::vec2 leftBottomCorner, glm::vec2 rightTopCorner)
            : m_radius(radius), m_resolution(resolution), m_leftBottomCorner(leftBottomCorner), m_rightTopCorner(rightTopCorner) {}

        void onGuiRender() override {

            ImGui::SliderFloat("Radius", &m_radius, 1.0f, 50.0f);
            ImGui::SliderFloat("Start Longitude", &m_leftBottomCorner.x, -180.0f, 180.0f);
            ImGui::SliderFloat("End Longitude", &m_rightTopCorner.x, -180.0f, 180.0f);
            ImGui::SliderFloat("Start Latitude", &m_leftBottomCorner.y, -90.0f, 90.0f);
            ImGui::SliderFloat("End Latitude", &m_rightTopCorner.y, -90.0f, 90.0f);
            ImGui::SliderInt("Resolution", (int*) &m_resolution, 0, 63);

        }

        std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() override {

            return generate(
                m_radius,
                m_resolution,
                {1, 0, 0},
                {0, 1, 0},
                m_leftBottomCorner,
                m_rightTopCorner
            );

        }

    private:

        static std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> generate(float radius, unsigned int resolution, glm::vec3 longitudeAxis, glm::vec3 latitudeAxis, glm::vec2 leftBottomCorner, glm::vec2 rightTopCorner) {

            std::vector<Graphics::Vertex> vertices;
            std::vector<unsigned int> indices;

            float width = 2.0f;
            float height = 2.0f;
            float patchWidth = width / (float) resolution;
            float patchHeight = height / (float) resolution;
            float halfPatchWidth = patchWidth/2.0f;
            float halfPatchHeight = patchHeight / 2.0f;

            glm::vec3 planeLeftBottomCorner = glm::vec3(0) + (longitudeAxis * -width/2.0f) + (latitudeAxis * -height/2.0f);

            // For each row
            for (unsigned int i = 0; i < resolution; i++) {

                // For each cell in the row
                for (unsigned int j = 0; j < resolution; j++) {

                    unsigned int faceIndexOffset = vertices.size();

                    // Vertex position
                    glm::vec3 patchOrigin = planeLeftBottomCorner
                        + (longitudeAxis * halfPatchWidth) + (latitudeAxis * halfPatchHeight)
                        + ( longitudeAxis * (patchWidth * (float) j))
                        + ( latitudeAxis * (patchHeight * (float) i));

                    glm::vec3 patchLeftBottom    = (patchOrigin - (longitudeAxis * halfPatchWidth) - (latitudeAxis * halfPatchHeight));
                    glm::vec3 patchRightBottom   = (patchOrigin + (longitudeAxis * halfPatchWidth) - (latitudeAxis * halfPatchHeight));
                    glm::vec3 patchRightTop      = (patchOrigin + (longitudeAxis * halfPatchWidth) + (latitudeAxis * halfPatchHeight));
                    glm::vec3 patchLeftTop       = (patchOrigin - (longitudeAxis * halfPatchWidth) + (latitudeAxis * halfPatchHeight));

                    auto [sphereLeftBottom, leftBottomUV] = planePoint2Sphere(patchLeftBottom, longitudeAxis, latitudeAxis, width, leftBottomCorner, rightTopCorner);
                    auto [sphereRightBottom, rightBottomUV] = planePoint2Sphere(patchRightBottom, longitudeAxis, latitudeAxis, width, leftBottomCorner, rightTopCorner);
                    auto [sphereRightTop, rightTopUV] = planePoint2Sphere(patchRightTop, longitudeAxis, latitudeAxis, width, leftBottomCorner, rightTopCorner);
                    auto [sphereLeftTop, leftTopUV] = planePoint2Sphere(patchLeftTop, longitudeAxis, latitudeAxis, width, leftBottomCorner, rightTopCorner);

                    // Push the quad
                    vertices.emplace_back(glm::vec4(sphereLeftBottom * radius, 1), sphereLeftBottom, leftBottomUV);
                    vertices.emplace_back(glm::vec4(sphereRightBottom * radius, 1), sphereRightBottom, rightBottomUV);
                    vertices.emplace_back(glm::vec4(sphereRightTop * radius, 1), sphereRightTop, rightTopUV);
                    vertices.emplace_back(glm::vec4(sphereLeftTop * radius, 1), sphereLeftTop, leftTopUV);
                    indices.emplace_back(0 + faceIndexOffset);
                    indices.emplace_back(1 + faceIndexOffset);
                    indices.emplace_back(2 + faceIndexOffset);
                    indices.emplace_back(2 + faceIndexOffset);
                    indices.emplace_back(3 + faceIndexOffset);
                    indices.emplace_back(0 + faceIndexOffset);


                }

            }

            return {vertices, indices};

        }

        static std::tuple<glm::vec3, glm::vec2> planePoint2Sphere(glm::vec3 planePoint, glm::vec3 longitudeAxis, glm::vec3 latitudeAxis, float planeSize, glm::vec2 leftBottomCorner, glm::vec2 rightTopCorner) {

            float pi = glm::pi<float>();

            // Sphere point
            glm::vec3 spherePoint = planePoint2SpherePoint(planePoint, longitudeAxis, latitudeAxis, planeSize, leftBottomCorner, rightTopCorner);

            // UV (Equirectangular projection)
            float u = (std::atan2(spherePoint.x, spherePoint.z) / pi + 1) / 2.0f;
            float v = std::asin(spherePoint.y) / pi + 0.5;

            float u2 = glm::dot(planePoint, longitudeAxis);

            // If the left edge of the plane meets the opposite side of the sphere, keep it as 0
            if (u2 == -1.0f && u == 1.0f) {
                u = 0.0f;
            }

            // If the right edge of the plane meets the opposite side of the sphere, keep it as 1
            if (u2 >= 1.0f && u < 0.001f) {
                u = 1.0f;
            }

            return {
                spherePoint,
                {u, v}
            };

        }

        static glm::vec3 planePoint2SpherePoint(glm::vec3 p, glm::vec3 longitudeAxis, glm::vec3 latitudeAxis, float planeSize, glm::vec2 leftBottomCorner, glm::vec2 rightTopCorner) {

            // [0, 1]
            float u = (glm::dot(p, longitudeAxis) + 1.0f) / planeSize;
            float v = (glm::dot(p, latitudeAxis) + 1.0f) / planeSize;

            // Scale to specifications
            // start + ([u|v] * (end - start))
            float lonStart = glm::radians(glm::max(leftBottomCorner.x, -180.0f));
            float lonEnd = glm::radians(glm::min(rightTopCorner.x, 180.0f));
            float lon = lonStart + (u * (lonEnd - lonStart));

            float latStart = glm::radians(glm::max(leftBottomCorner.y, -89.999f));
            float latEnd = glm::radians(glm::min(rightTopCorner.y, 89.999f));
            float lat = latStart + (v * (latEnd - latStart));

            // Shift everything 90 degrees to the right, so the plane starts at 0 and ends at 1 (horizontally)
            lon += glm::radians(90.0f);

            float x = glm::cos(lat) * glm::cos(lon);
            float y = glm::sin(lat);
            float z = glm::cos(lat) * glm::sin(lon);

            return {-x, y, z};

        }

        float m_radius;
        unsigned int m_resolution;
        glm::vec2 m_leftBottomCorner;
        glm::vec2 m_rightTopCorner;

    };

}