#pragma once

#include <memory>

#include "../shader/Shader.h"
#include "../buffer/VertexArray.h"
#include "../camera/OrthographicCamera.h"
#include "../mesh/Mesh.h"

namespace engine {

    class Renderer {

    public:
        Renderer() = delete;
        Renderer(Renderer const&) = delete;
        void operator=(Renderer const&)  = delete;

    public:

        static void beginScene(const std::shared_ptr<OrthographicCamera>& orthographicCamera);
        static void endScene();

        static void beginBatch(const std::shared_ptr<Shader>& shader);
        static void submit(const Mesh& mesh, const glm::mat4& transform);
        static void flushBatch();

        static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    private:

        struct SceneData {
            glm::mat4 m_viewProjectionMatrix;
        };

        static SceneData* m_sceneData;

        struct BatchData {

            std::shared_ptr<Shader> m_shader;

            std::vector<Vertex> m_vertices;
            std::vector<int> m_indices;

            explicit BatchData(std::shared_ptr<Shader> shader) : m_shader(shader) {
                m_vertices = std::vector<Vertex> {};
                m_indices = std::vector<int> {};
            }
        };

        static BatchData* m_batchData;

    };

}