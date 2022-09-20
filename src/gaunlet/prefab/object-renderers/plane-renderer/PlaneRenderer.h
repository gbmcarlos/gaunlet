#pragma once

#include "gaunlet/scene/renderer/ObjectRenderer.h"
#include "gaunlet/prefab/object-renderers/model-renderer/ModelRenderer.h"
#include "gaunlet/graphics/renderer/DirectRenderer.h"
#include "gaunlet/scene/components/TerrainComponents.h"
#include "gaunlet/scene/camera/Camera.h"

#include "gaunlet/pch.h"

namespace gaunlet::Prefab::ObjectRenderers {

    struct PlaneQuadEntityProperties {

        PlaneQuadEntityProperties() = default;

        float m_leftSizeFactor;
        float m_rightSizeFactor;
        float m_bottomSizeFactor;
        float m_topSizeFactor;
        unsigned int m_textureIndex; // For compatibility with the batch renderer
        glm::vec3 m_pad3;

    };

    struct CameraFrustumPlane {
        glm::vec3 m_normal;
        float distance;
    };

    struct CameraFrustum {

        CameraFrustumPlane m_nearPlane;
        CameraFrustumPlane m_farPlane;
        CameraFrustumPlane m_leftPlane;
        CameraFrustumPlane m_rightPlane;
        CameraFrustumPlane m_bottomPlane;
        CameraFrustumPlane m_topPlane;

    };

    class PlaneRenderer {

    public:

        PlaneRenderer(unsigned int propertySetsUniformBufferBindingPoint, unsigned int cameraFrustumUniformBufferBindingPoint)
            : m_renderer({100000, 600000, 10, 100}) {

            // Create a uniform buffer that will contain the properties of every object, and will be linked to the shader
            m_propertySetsUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
                "EntityPropertySets",
                propertySetsUniformBufferBindingPoint,
                sizeof (PlaneQuadEntityProperties) * m_renderer.getBatchParameters().m_maxPropertySets
            );

            // Create a uniform buffer that will contain the information about the camera's frustum
            m_frustumUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
                "CameraFrustum",
                cameraFrustumUniformBufferBindingPoint,
                sizeof (CameraFrustum)
            );

            loadShaders();
        }

        void render(Scene::Entity entity, const Core::Ref<Graphics::Shader>& shader) {

            auto& planeComponent = entity.getComponent<Scene::PlaneComponent>();
            auto hierarchicalTransform = getHierarchicalTransform(entity);

            m_shaderLibrary.get("plane-faces")
                ->setUniformMat4f("u_modelTransform", hierarchicalTransform)
                ->setUniform1f("u_targetTessellationLevel", planeComponent.m_targetTessellationLevel)
                ->setUniform1f("u_tessellationLevelSlope", planeComponent.m_tessellationLevelSlope)
                ->setUniform1f("u_maxHeight", planeComponent.m_maxHeight)
                ->setUniform1i("u_entityId", entity.getId());

            Scene::Frustum originalFrustum = planeComponent.m_camera->getFrustum();

            CameraFrustum cameraFrustum{
                {originalFrustum.m_nearPlane.m_normal, originalFrustum.m_nearPlane.m_distance},
                {originalFrustum.m_farPlane.m_normal, originalFrustum.m_farPlane.m_distance},
                {originalFrustum.m_leftPlane.m_normal, originalFrustum.m_leftPlane.m_distance},
                {originalFrustum.m_rightPlane.m_normal, originalFrustum.m_rightPlane.m_distance},
                {originalFrustum.m_bottomPlane.m_normal, originalFrustum.m_bottomPlane.m_distance},
                {originalFrustum.m_topPlane.m_normal, originalFrustum.m_topPlane.m_distance},
            };

            m_frustumUniformBuffer->setData(
                (const void*) &cameraFrustum,
                sizeof(CameraFrustum)
            );

            for (auto& quad : planeComponent.getContent()) {

                PlaneQuadEntityProperties planeQuadEntityProperties{};

                bool batched = m_renderer.submitIndexedTriangles(
                    quad.m_vertices,
                    quad.m_indices,
                    planeComponent.m_heightmap,
                    planeQuadEntityProperties
                );

                if (!batched) {
                    m_renderer.flush(shader, Graphics::RenderMode::Quad);
                    m_renderer.submitIndexedTriangles(
                        quad.m_vertices,
                        quad.m_indices,
                        planeComponent.m_heightmap,
                        planeQuadEntityProperties
                    );
                }

            }

            m_renderer.flush(shader, Graphics::RenderMode::Quad);

        }

        static unsigned int getUniformBufferCount() {return 2; }

        inline Graphics::ShaderLibrary& getShaders() {return m_shaderLibrary; }

    protected:

        Graphics::BatchedRenderer<PlaneQuadEntityProperties> m_renderer;
        Core::Ref<Graphics::UniformBuffer> m_propertySetsUniformBuffer = nullptr;
        Core::Ref<Graphics::UniformBuffer> m_frustumUniformBuffer = nullptr;

    private:

        glm::mat4 getHierarchicalTransform(Scene::Entity entity) {

            if (!entity.hasComponent<Scene::TransformComponent>()) {
                return glm::mat4(1);
            }

            glm::mat4 result = entity.getComponent<Scene::TransformComponent>().getTransformationMatrix();
            Scene::Entity current = entity;

            while (true) {

                auto parent = current.getParent();

                if (!parent || !parent.hasComponent<Scene::TransformComponent>()) {
                    break;
                }

                // Multiply with the parent's transform and move to the next generation
                result = parent.getComponent<Scene::TransformComponent>().getTransformationMatrix() * result;
                current = parent;

            }

            return result;

        }

        void loadShaders() {

            std::map<Core::ShaderType, std::string> facesSources {
                {Core::ShaderType::Vertex, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/vertex.glsl"},
                {Core::ShaderType::TessellationControl, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/tessellation-control.glsl"},
                {Core::ShaderType::TessellationEvaluation, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/tessellation-evaluation.glsl"},
                {Core::ShaderType::Fragment, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/fragment.glsl"}
            };

            auto facesShader = m_shaderLibrary.load("plane-faces", facesSources);

            // Set a single "skybox" texture
            facesShader->setUniform1i("heightmap", 1);

            facesShader->linkUniformBuffer(m_frustumUniformBuffer);

        }

        Graphics::ShaderLibrary m_shaderLibrary;

    };

}