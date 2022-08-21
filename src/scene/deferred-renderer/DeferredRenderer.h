#pragma once

#include "batch/Batch.h"

#include "../../graphics/vertex-array/VertexArray.h"
#include "../../graphics/uniform-buffer/UniformBuffer.h"
#include "../../graphics/texture/Texture.h"
#include "../../graphics/framebuffer/Framebuffer.h"

#include "../../graphics/shader/Shader.h"
#include "../../graphics/shader/ShaderLibrary.h"

#include "../entity/GraphicsComponents.h"
#include "../entity/LightingComponents.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../pch.h"

namespace engine {

    class DeferredRenderer {

    public:
        DeferredRenderer() = delete;
        DeferredRenderer(DeferredRenderer const&) = delete;
        void operator=(DeferredRenderer const&)  = delete;

    public:

        static void init();

        static void beginScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const Ref<Framebuffer>& framebuffer, const DirectionalLightComponent& directionalLight);
        static void endScene();

        // Batched draw calls
        static void submit(int entityId, const ModelComponent& polygonComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);
        static void submit(int entityId, const CircleComponent& circleComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);

    private:

        // Flush batches
        static void flushPolygons();
        static void flushCircles();

        // Init loaders
        static void loadDefaultShaders();
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

            Batch<PolygonEntityProperties> m_polygonBatch;
            Batch<CircleEntityProperties> m_circleBatch;

            Ref<Framebuffer> m_framebuffer;

            // Shared
            Ref<Texture> m_whiteTexture = nullptr;
            ShaderLibrary m_shaderLibrary;
            Ref<UniformBuffer> m_scenePropertiesUniformBuffer = nullptr;
            Ref<UniformBuffer> m_polygonEntityPropertiesUniformBuffer = nullptr;
            Ref<UniformBuffer> m_circleEntityPropertiesUniformBuffer = nullptr;

        };

        static RendererStorage* m_rendererStorage;

    };

}