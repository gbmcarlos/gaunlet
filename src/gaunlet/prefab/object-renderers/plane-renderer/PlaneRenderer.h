#pragma once

#include "gaunlet/scene/renderer/ObjectRenderer.h"
#include "gaunlet/prefab/object-renderers/model-renderer/ModelRenderer.h"
#include "gaunlet/graphics/render-pass/SimpleRenderPass.h"
#include "gaunlet/prefab/terrain/TerrainComponents.h"
#include "gaunlet/scene/camera/Camera.h"

#include "gaunlet/pch.h"

namespace gaunlet::Prefab::ObjectRenderers {

    struct PlaneQuadEntityProperties {

        PlaneQuadEntityProperties() = default;

        unsigned int m_quadPosition;
        float m_leftSizeFactor;
        float m_bottomSizeFactor;
        float m_rightSizeFactor;
        float m_topSizeFactor;
        unsigned int m_textureIndex; // For compatibility with the batch renderer
        glm::vec2 m_pad2;

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
            : m_renderer({100000, 600000, 10, 1000}) {

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

            auto& planeComponent = entity.getComponent<Prefab::Terrain::TerrainComponent>();

            // Global variables for the whole plane
            m_shaderLibrary.get("plane-faces")
                ->setUniform1f("u_triangleSize", planeComponent.m_triangleSize)
                ->setUniform1f("u_maxHeight", planeComponent.m_maxHeight)
                ->setUniform1i("u_entityId", entity.getId());

            // Global frustum data
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

                // Per-quad data
                PlaneQuadEntityProperties planeQuadEntityProperties{
                    quad.m_position,
                    quad.m_leftSizeRatio,
                    quad.m_bottomSizeRatio,
                    quad.m_rightSizeRatio,
                    quad.m_topSizeRatio
                };

                bool batched = m_renderer.submitIndexedTriangles(
                    quad.m_vertices,
                    quad.m_indices,
                    planeComponent.m_heightmap,
                    planeQuadEntityProperties
                );

                if (!batched) {
                    renderObjects(shader);
                    m_renderer.submitIndexedTriangles(
                        quad.m_vertices,
                        quad.m_indices,
                        planeComponent.m_heightmap,
                        planeQuadEntityProperties
                    );
                }

            }

            renderObjects(shader);

        }

        static unsigned int getUniformBufferCount() {return 2; }

        inline Graphics::ShaderLibrary& getShaders() {return m_shaderLibrary; }

    protected:

        void renderObjects(const Core::Ref<Graphics::Shader>& shader) {

            auto& entityPropertySets = m_renderer.getPropertySets();

            // Submit the entity properties to the uniform buffer
            m_propertySetsUniformBuffer->setData(
                (const void*) entityPropertySets.data(),
                sizeof(PlaneQuadEntityProperties) * entityPropertySets.size()
            );

            m_renderer.flush(shader, Graphics::RenderMode::Quad);

        }

        Graphics::BatchedRenderPass<PlaneQuadEntityProperties> m_renderer;
        Core::Ref<Graphics::UniformBuffer> m_propertySetsUniformBuffer = nullptr;
        Core::Ref<Graphics::UniformBuffer> m_frustumUniformBuffer = nullptr;

    private:

        void loadShaders() {

            std::map<Core::ShaderType, std::string> facesSources {
                {Core::ShaderType::Vertex, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/vertex.glsl"},
                {Core::ShaderType::TessellationControl, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/tessellation-control.glsl"},
                {Core::ShaderType::TessellationEvaluation, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/tessellation-evaluation.glsl"},
                {Core::ShaderType::Fragment, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/fragment.glsl"}
            };

            auto facesShader = m_shaderLibrary.load("plane-faces", facesSources);

            // Set a single "heighmap" texture (slot 0 is for the whiteTexture)
            facesShader->setUniform1i("heightmap", 1);

            facesShader->linkUniformBuffer(m_propertySetsUniformBuffer);
            facesShader->linkUniformBuffer(m_frustumUniformBuffer);

        }

        Graphics::ShaderLibrary m_shaderLibrary;

    };

}