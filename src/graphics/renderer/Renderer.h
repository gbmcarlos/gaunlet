#pragma once

#include "../shader/Shader.h"
#include "../camera/OrthographicCamera.h"
#include "../buffer/VertexArray.h"
#include "../mesh/Mesh.h"

#include <memory>

namespace engine {

    enum class MeshElementType {
        Face, Edge, Polygon
    };

    class Renderer {

    public:
        Renderer() = delete;
        Renderer(Renderer const&) = delete;
        void operator=(Renderer const&)  = delete;

    public:

        static void beginScene(const std::shared_ptr<OrthographicCamera>& orthographicCamera);
        static void endScene();

        static void beginBatch(const std::shared_ptr<Shader>& shader, MeshElementType meshElementType);
        static void submit(const Mesh& mesh, const glm::mat4& transform);
        static void flushBatch();

        static void submitTriangles(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
        static void submitLines(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
        static void submitPolygons(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    private:

        static std::vector<unsigned int> getMeshVertices(const Mesh& mesh, MeshElementType meshElementType);

        struct SceneData {
            glm::mat4 m_viewProjectionMatrix;
        };

        static SceneData* m_sceneData;

        struct BatchData {

            MeshElementType m_meshElementType;

            std::shared_ptr<Shader> m_shader;

            std::vector<Vertex> m_vertices;
            std::vector<int> m_indices;

            explicit BatchData(std::shared_ptr<Shader> shader, MeshElementType meshElementType) : m_shader(shader), m_meshElementType(meshElementType) {
                m_vertices = std::vector<Vertex> {};
                m_indices = std::vector<int> {};
            }
        };

        static BatchData* m_batchData;

    };

}