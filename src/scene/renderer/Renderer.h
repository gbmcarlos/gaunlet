#pragma once

#include "batch/Batch.h"

#include "../../graphics/vertex-array/VertexArray.h"
#include "../../graphics/uniform-buffer/UniformBuffer.h"
#include "../../graphics/texture/Texture.h"

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

        static void beginScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
        static void endScene();

        // Batched draw calls
        static void submit(const unsigned int& entityId, const PolygonModelComponent& polygonComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);
        static void submit(const unsigned int& entityId, const CircleModelComponent& circleComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);

    private:

        // Flush batches
        static void flushPolygons();
        static void flushCircles();

        // Non-batched, direct draw calls
        static void submitPolygons(const std::vector<PolygonVertex>& polygonVertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures, const std::vector<PolygonEntityProperties>& entityProperties, const std::shared_ptr<Shader>& shader);
        static void submitCircles(const std::vector<CircleVertex>& circleVertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures, const std::vector<CircleEntityProperties>& entityProperties, const std::shared_ptr<Shader>& shader);

        // Init loaders
        static void loadDefaultShaders();
        static void loadDefaultWhiteTexture();

        struct RendererStorage {

            Batch<PolygonVertex, PolygonEntityProperties> m_polygonBatch;
            Batch<CircleVertex, CircleEntityProperties> m_circleBatch;

            // Shared
            std::shared_ptr<Texture> m_whiteTexture = nullptr;
            ShaderLibrary m_shaderLibrary;
            std::shared_ptr<UniformBuffer> m_sceneMatricesUniformBuffer = nullptr;
            std::shared_ptr<UniformBuffer> m_polygonEntityPropertiesUniformBuffer = nullptr;
            std::shared_ptr<UniformBuffer> m_circleEntityPropertiesUniformBuffer = nullptr;

        };

        static RendererStorage* m_rendererStorage;

    };

}