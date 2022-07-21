#pragma once

#include "../shader/Shader.h"
#include "../shader/ShaderLibrary.h"
#include "../camera/OrthographicCamera.h"
#include "../buffer/VertexArray.h"
#include "../mesh/Mesh.h"
#include "../texture/Texture.h"

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

        static void submit(const Mesh& mesh, const glm::mat4& transform, glm::vec4 color);
        static void submit(const Mesh& mesh, const glm::mat4& transform, glm::vec4 color, std::shared_ptr<Texture> texture);
        static void flush();

        static void submitTriangles(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    private:

        static void loadDefaultShaders();
        static void loadDefaultWhiteTexture();
        static bool shouldFlush(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture>& texture);

        struct RendererStorage {

            static const unsigned int m_maxVertices = 5;
            std::vector<Vertex> m_vertices = {};

            static const unsigned int m_maxIndices = m_maxVertices * 2;
            std::vector<unsigned int> m_indices = {};

            static const unsigned int m_maxTextures = 10;
            std::vector<std::shared_ptr<Texture> > m_textures = {};
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