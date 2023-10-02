#pragma once

#include <utility>

#include "gaunlet/scene/model/MeshGenerator.h"
#include "gaunlet/scene/camera/Camera.h"

namespace gaunlet::Prefab::MeshGenerators {

    class PlaneSphereGenerator : public Scene::MeshGenerator {

    public:

        PlaneSphereGenerator(float radius, unsigned int resolution, Core::Ref<Scene::Camera>  camera)
            : m_radius(radius), m_resolution(resolution), m_camera(std::move(camera)) {}

        void onGuiRender() override {

            ImGui::SliderFloat("Radius", &m_radius, 1.0f, 50.0f);
            ImGui::SliderFloat("Start Longitude", &m_leftBottomCorner.x, -180.0f, 180.0f);
            ImGui::SliderFloat("End Longitude", &m_rightTopCorner.x, -180.0f, 180.0f);
            ImGui::SliderFloat("Start Latitude", &m_leftBottomCorner.y, -90.0f, 90.0f);
            ImGui::SliderFloat("End Latitude", &m_rightTopCorner.y, -90.0f, 90.0f);
            ImGui::SliderInt("Resolution", (int*) &m_resolution, 0, 63);

        }

        std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() override {

            auto cameraPosition = glm::normalize(m_camera->getPosition()) * m_radius;

            glm::vec3 longitudeAxis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), cameraPosition));
            glm::vec3 latitudeAxis = glm::normalize(glm::cross(longitudeAxis, cameraPosition));

            Context context(
                m_radius,
                m_resolution,
                longitudeAxis,
                latitudeAxis,
                m_leftBottomCorner,
                m_rightTopCorner
            );

            return generate(context);

        }

    private:

        struct Context {

            Context(float radius, unsigned int resolution, glm::vec3 longitudeAxis, glm::vec3 latitudeAxis, glm::vec2 leftBottomCorner, glm::vec2 rightTopCorner)
                : m_radius(radius), m_resolution(resolution), m_longitudeAxis(longitudeAxis), m_latitudeAxis(latitudeAxis) {
                m_frontAxis = glm::cross(m_longitudeAxis, m_latitudeAxis);
                m_longitudeStart = glm::radians(glm::max(leftBottomCorner.x, -180.0f));
                m_longitudeEnd = glm::radians(glm::min(rightTopCorner.x, 180.0f));
                m_latitudeStart = glm::radians(glm::max(leftBottomCorner.y, -89.999f));
                m_latitudeEnd = glm::radians(glm::min(rightTopCorner.y, 89.999f));
                m_longitudeShift = glm::radians(90.0f);
                m_basisMatrix = glm::transpose(glm::mat3(m_longitudeAxis, m_latitudeAxis, glm::cross(m_longitudeAxis, m_latitudeAxis)));
                m_pi = glm::pi<float>();
            }

            float m_radius;
            unsigned int m_resolution;
            glm::vec3 m_longitudeAxis;
            glm::vec3 m_latitudeAxis;
            glm::vec3 m_frontAxis;
            float m_longitudeStart;
            float m_longitudeEnd;
            float m_latitudeStart;
            float m_latitudeEnd;
            float m_longitudeShift;
            glm::mat3 m_basisMatrix;
            float m_pi;

        };

        static std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> generate(Context& context) {

            std::vector<Graphics::Vertex> vertices;
            std::vector<unsigned int> indices;

            float width = 2.0f;
            float height = 2.0f;
            float patchWidth = width / (float) context.m_resolution;
            float patchHeight = height / (float) context.m_resolution;
            float halfPatchWidth = patchWidth/2.0f;
            float halfPatchHeight = patchHeight / 2.0f;

            glm::vec3 planeLeftBottomCorner = glm::vec3(0) + (context.m_longitudeAxis * -width/2.0f) + (context.m_latitudeAxis * -height/2.0f);

            // For each row
            for (unsigned int i = 0; i < context.m_resolution; i++) {

                // For each cell in the row
                for (unsigned int j = 0; j < context.m_resolution; j++) {

                    unsigned int faceIndexOffset = vertices.size();

                    // Vertex position
                    glm::vec3 patchOrigin = planeLeftBottomCorner
                        + (context.m_longitudeAxis * halfPatchWidth) + (context.m_latitudeAxis * halfPatchHeight)
                        + ( context.m_longitudeAxis * (patchWidth * (float) j))
                        + ( context.m_latitudeAxis * (patchHeight * (float) i));

                    glm::vec3 patchLeftBottom    = (patchOrigin - (context.m_longitudeAxis * halfPatchWidth) - (context.m_latitudeAxis * halfPatchHeight));
                    glm::vec3 patchRightBottom   = (patchOrigin + (context.m_longitudeAxis * halfPatchWidth) - (context.m_latitudeAxis * halfPatchHeight));
                    glm::vec3 patchRightTop      = (patchOrigin + (context.m_longitudeAxis * halfPatchWidth) + (context.m_latitudeAxis * halfPatchHeight));
                    glm::vec3 patchLeftTop       = (patchOrigin - (context.m_longitudeAxis * halfPatchWidth) + (context.m_latitudeAxis * halfPatchHeight));

                    auto [sphereLeftBottom, leftBottomUV] = planePoint2Sphere(patchLeftBottom, false, context);
                    auto [sphereRightBottom, rightBottomUV] = planePoint2Sphere(patchRightBottom, true, context);
                    auto [sphereRightTop, rightTopUV] = planePoint2Sphere(patchRightTop, true, context);
                    auto [sphereLeftTop, leftTopUV] = planePoint2Sphere(patchLeftTop, false, context);

                    // If the quad crosses the seam, make the right side be over 1
                    if ((rightBottomUV.x - leftBottomUV.x) < 0.0f) {
//                        rightBottomUV.x = 1.0f;
                        rightBottomUV.x += 1.0f;
                    }

                    if ((rightTopUV.x - leftTopUV.x) < 0.0f) {
//                        rightTopUV.x = 1.0f;
                        rightTopUV.x += 1.0f;
                    }

                    // Push the quad
                    vertices.emplace_back(glm::vec4(sphereLeftBottom * context.m_radius, 1), sphereLeftBottom, leftBottomUV);
                    vertices.emplace_back(glm::vec4(sphereRightBottom * context.m_radius, 1), sphereRightBottom, rightBottomUV);
                    vertices.emplace_back(glm::vec4(sphereRightTop * context.m_radius, 1), sphereRightTop, rightTopUV);
                    vertices.emplace_back(glm::vec4(sphereLeftTop * context.m_radius, 1), sphereLeftTop, leftTopUV);
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

        static std::tuple<glm::vec3, glm::vec2> planePoint2Sphere(glm::vec3 planePoint, bool rightSidePoint, Context& context) {

            float pi = glm::pi<float>();

            // Sphere point
            glm::vec3 spherePoint = planePoint2SpherePoint(planePoint, context);

            // UV (Equirectangular projection)
            float u = (std::atan2(spherePoint.x, spherePoint.z) / pi + 1) / 2.0f;
            float v = std::asin(spherePoint.y) / pi + 0.5;

            // If we've reached the seam, and this point is on the left side of the patch, make sure it gets 0 as U coordinate
            if ((u > 0.999f || u < 0.001f) && !rightSidePoint) {
//                u = 0.0f;
            }

            // If we've reached the seam, and this point is on the right side of the patch, make sure it gets 1 as U coordinate
            if ((u > 0.999f || u < 0.001f) && rightSidePoint) {
//                u = 1.0f;
            }

            return {
                spherePoint,
                {u, v}
            };

        }

        static glm::vec3 planePoint2SpherePoint(glm::vec3 p, Context& context) {

            // [0, 1]
            float u = (glm::dot(p, context.m_longitudeAxis) + 1.0f) / 2.0f;
            float v = (glm::dot(p, context.m_latitudeAxis) + 1.0f) / 2.0f;

            // Scale to specifications
            // start + ([u|v] * (end - start))
            float lon = context.m_longitudeStart + (u * (context.m_longitudeEnd - context.m_longitudeStart));
            float lat = context.m_latitudeStart + (v * (context.m_latitudeEnd - context.m_latitudeStart));

            // Shift everything, so the plane starts at 0 and ends at 1 (horizontally)
            lon += context.m_longitudeShift;

            float x = glm::cos(lat) * glm::cos(lon);
            float y = glm::sin(lat);
            float z = glm::cos(lat) * glm::sin(lon);

            return glm::vec3(-x, y, z) * context.m_basisMatrix;

        }

        float m_radius;
        unsigned int m_resolution;
        Core::Ref<Scene::Camera> m_camera;
        glm::vec2 m_leftBottomCorner = {-90.0f, -90.0f};
        glm::vec2 m_rightTopCorner = {90.0f, 90.0f};

    };

}