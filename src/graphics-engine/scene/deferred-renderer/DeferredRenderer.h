#pragma once

#include "graphics-engine/scene/deferred-renderer/batch/Batch.h"

#include "graphics-engine/graphics/vertex-array/VertexArray.h"
#include "graphics-engine/graphics/uniform-buffer/UniformBuffer.h"
#include "graphics-engine/graphics/texture/Texture.h"
#include "graphics-engine/graphics/framebuffer/Framebuffer.h"

#include "graphics-engine/graphics/shader/Shader.h"
#include "graphics-engine/graphics/shader/ShaderLibrary.h"

#include "graphics-engine/scene/entity/GraphicsComponents.h"
#include "graphics-engine/scene/entity/LightingComponents.h"

#include "graphics-engine/pch.h"

namespace engine::Scene {

    class DeferredRenderer {

    public:
        DeferredRenderer() = delete;
        DeferredRenderer(DeferredRenderer const&) = delete;
        void operator=(DeferredRenderer const&)  = delete;

    public:

        static void init();

        static void beginScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const Core::Ref<Graphics::Framebuffer>& framebuffer, const DirectionalLightComponent& directionalLight);
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

            Core::Ref<Graphics::Framebuffer> m_framebuffer;

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