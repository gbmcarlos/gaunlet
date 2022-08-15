#pragma once

#include "../../graphics/buffer/VertexArray.h"
#include "../../graphics/texture/Texture.h"

#include "../../graphics/shader/Shader.h"
#include "../../graphics/shader/ShaderLibrary.h"

#include "../entity/GraphicsComponents.h"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace engine {

    class Renderer {

    public:
        Renderer() = delete;
        Renderer(Renderer const&) = delete;
        void operator=(Renderer const&)  = delete;

    public:

        static void init();

        static void beginScene(glm::mat4 viewProjectionMatrix);
        static void endScene();

        // Batched draw calls
        static void submit(const PolygonModelComponent& polygonComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);
        static void submit(const CircleModelComponent& circleComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);

        // Flush batches
        static void flushPolygons();
        static void flushCircles();

        // Non-batched, direct draw calls
        static void submitTriangles(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
        static void submitCircles(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    private:

        // Init loaders
        static void loadDefaultShaders();
        static void loadDefaultWhiteTexture();

        // Internal batch checks
        static bool shouldFlushPolygon(const std::vector<PolygonVertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture>& texture);
        static bool shouldFlushCircles(const std::vector<CircleVertex>& vertices, const std::shared_ptr<Texture>& texture);

        struct RendererStorage {

            // Polygons
            static const unsigned int m_maxPolygonVertices = 100;
            std::vector<PolygonVertex> m_polygonVertices = {};

            static const unsigned int m_maxPolygonIndices = m_maxPolygonVertices * 3;
            std::vector<unsigned int> m_polygonIndices = {};

            static const unsigned int m_maxPolygonTextures = 10;
            std::vector<std::shared_ptr<Texture> > m_polygonTextures = {};

            // Circles
            static const unsigned int m_maxCircleVertices = 100;
            std::vector<CircleVertex> m_circleVertices = {};

            std::vector<unsigned int> m_circleIndices = {};

            static const unsigned int m_maxCircleTextures = 10;
            std::vector<std::shared_ptr<Texture> > m_circleTextures = {};

            // Shared
            glm::mat4 m_viewProjectionMatrix;
            std::shared_ptr<Texture> m_whiteTexture = nullptr;
            ShaderLibrary m_shaderLibrary;

            RendererStorage() : m_viewProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f)) {}

        };

        static RendererStorage* m_rendererStorage;

    };

}