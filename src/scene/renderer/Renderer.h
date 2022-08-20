#pragma once

#include "batch/Batch.h"

#include "../../graphics/vertex-array/VertexArray.h"
#include "../../graphics/uniform-buffer/UniformBuffer.h"
#include "../../graphics/texture/Texture.h"
#include "../../graphics/framebuffer/Framebuffer.h"

#include "../../graphics/shader/Shader.h"
#include "../../graphics/shader/ShaderLibrary.h"

#include "../entity/GraphicsComponents.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../pch.h"

namespace engine {

    class Renderer {

    public:
        Renderer() = delete;
        Renderer(Renderer const&) = delete;
        void operator=(Renderer const&)  = delete;

    public:

        static void init();

        static void beginScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const Ref<Framebuffer>& framebuffer);
        static void endScene();

        // Batched draw calls
        static void submit(int entityId, const PolygonModelComponent& polygonComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);
        static void submit(int entityId, const CircleModelComponent& circleComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);

    private:

        // Flush batches
        static void flushPolygons();
        static void flushCircles();

        // Non-batched, direct draw calls
        static void renderPolygons(const std::vector<PolygonVertex>& polygonVertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures, const std::vector<PolygonEntityProperties>& entityProperties, const Ref<Shader>& shader);
        static void renderCircles(const std::vector<CircleVertex>& circleVertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures, const std::vector<CircleEntityProperties>& entityProperties, const Ref<Shader>& shader);

        // Init loaders
        static void loadDefaultShaders();
        static void loadDefaultWhiteTexture();

        struct RendererStorage {

            Batch<PolygonVertex, PolygonEntityProperties> m_polygonBatch;
            Batch<CircleVertex, CircleEntityProperties> m_circleBatch;

            Ref<Framebuffer> m_framebuffer;

            // Shared
            Ref<Texture> m_whiteTexture = nullptr;
            ShaderLibrary m_shaderLibrary;
            Ref<UniformBuffer> m_sceneMatricesUniformBuffer = nullptr;
            Ref<UniformBuffer> m_polygonEntityPropertiesUniformBuffer = nullptr;
            Ref<UniformBuffer> m_circleEntityPropertiesUniformBuffer = nullptr;

        };

        static RendererStorage* m_rendererStorage;

    };

}