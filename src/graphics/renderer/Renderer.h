#pragma once

#include "../buffer/VertexArray.h"
#include "../texture/Texture.h"

#include "../shader/Shader.h"
#include "../shader/ShaderLibrary.h"

#include "../scene/Component.h"

#include "../camera/OrthographicCamera.h"

#include <array>
#include <memory>

namespace engine {

    class Renderer {

    public:
        Renderer() = delete;
        Renderer(Renderer const&) = delete;
        void operator=(Renderer const&)  = delete;

    public:

        static void init();

        static void beginScene(const std::shared_ptr<OrthographicCamera>& orthographicCamera);
        static void endScene();

        // Batched draw calls
        static void submit(const PolygonComponent& polygonComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);
        static void submit(const CircleComponent& circleComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent);

        static void flushPolygons();
        static void flushCircles();

        // Non-batched, direct draw calls
        static void submitTriangles(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
        static void submitCircles(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    private:

        static void loadDefaultShaders();
        static void loadDefaultWhiteTexture();

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
            std::shared_ptr<Texture> m_whiteTexture = nullptr;
            ShaderLibrary m_shaderLibrary;

            RendererStorage() = default;

        };

        static RendererStorage* m_rendererStorage;

        static ShaderLibrary m_shaderLibrary;

        struct SceneData {
            glm::mat4 m_viewProjectionMatrix;
        };

        static SceneData* m_sceneData;

    };

}