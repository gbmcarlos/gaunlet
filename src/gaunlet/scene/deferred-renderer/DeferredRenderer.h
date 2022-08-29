#pragma once

#include "gaunlet/scene/deferred-renderer/batch/Batch.h"

#include "gaunlet/graphics/vertex-array/VertexArray.h"
#include "gaunlet/graphics/uniform-buffer/UniformBuffer.h"
#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/graphics/Vertex.h"

#include "gaunlet/graphics/shader/Shader.h"
#include "gaunlet/graphics/shader/ShaderLibrary.h"

#include "gaunlet/scene/components/GraphicsComponents.h"
#include "gaunlet/scene/components/LightingComponents.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    enum class RenderMode {
        Faces, Wireframe
    };

    class DeferredRenderer {

    public:
        DeferredRenderer() = delete;
        DeferredRenderer(DeferredRenderer const&) = delete;
        void operator=(DeferredRenderer const&)  = delete;

    public:

        static void init();

        static void beginScene(RenderMode renderMode, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const DirectionalLightComponent& directionalLight);
        static void endScene();

        // Batched draw calls
        static void submit(int entityId, const ModelComponent& polygonComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);
        static void submit(int entityId, const CircleComponent& circleComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);

    private:

        // Flush batches
        static void flushPolygons();
        static void flushCircles();

        static void render(const std::vector<gaunlet::Graphics::Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Core::Ref<Graphics::Texture>>& textures, const Core::Ref<Graphics::Shader>& shader);

        // Init loaders
        static void loadShaders();
        static void loadPolygonShaders();
        static void loadCircleShaders();
        static void loadDefaultWhiteTexture();

        struct SceneProperties {

            SceneProperties(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 directionalLightColor, glm::vec3 directionalLightPosition, float directionalLightAmbientStrength, float directionalLightDiffuseIntensity)
                : m_viewMatrix(viewMatrix), m_projectionMatrix(projectionMatrix), m_directionalLightColor(directionalLightColor), m_directionalLightPosition(directionalLightPosition), m_directionalLightAmbientIntensity(directionalLightAmbientStrength), m_directionalLightDiffuseIntensity(directionalLightDiffuseIntensity) {}

            glm::mat4 m_viewMatrix;
            glm::mat4 m_projectionMatrix;

            // The order of these properties is optimized to minimize required padding when using this data in a uniform buffer
            glm::vec3 m_directionalLightColor;
            float m_directionalLightAmbientIntensity;
            glm::vec3 m_directionalLightPosition;
            float m_directionalLightDiffuseIntensity;

        };

        struct RendererStorage {

            RenderMode m_renderMode;

            Batch<PolygonEntityProperties> m_polygonBatch;
            Batch<CircleEntityProperties> m_circleBatch;

            // Shared
            Core::Ref<Graphics::Texture> m_whiteTexture = nullptr;
            Graphics::ShaderLibrary m_shaderLibrary;
            Core::Ref<Graphics::UniformBuffer> m_scenePropertiesUniformBuffer = nullptr;
            Core::Ref<Graphics::UniformBuffer> m_polygonEntityPropertiesUniformBuffer = nullptr;
            Core::Ref<Graphics::UniformBuffer> m_circleEntityPropertiesUniformBuffer = nullptr;

        };

        static RendererStorage* m_rendererStorage;

    };

}