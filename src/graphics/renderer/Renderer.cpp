#include "Renderer.h"

#include <map>

namespace engine {

    Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData{OrthographicCamera::getDefaultViewProjectionMatrix()};
    Renderer::BatchData* Renderer::m_batchData = nullptr;
    ShaderLibrary Renderer::m_shaderLibrary = ShaderLibrary();

    void Renderer::init() {
        loadDefaultShaders();
    }

    void Renderer::beginScene(const std::shared_ptr<OrthographicCamera>& orthographicCamera) {
        m_sceneData->m_viewProjectionMatrix = orthographicCamera->getViewProjectionMatrix();
    }

    void Renderer::endScene() {
        m_sceneData = new SceneData{OrthographicCamera::getDefaultViewProjectionMatrix()};
    }

    void Renderer::beginBatch(MeshElementType meshElementType) {
        Renderer::m_batchData = new BatchData(meshElementType);
    }

    void Renderer::submit(const Mesh& mesh, const glm::mat4& transform, glm::vec4 color) {

        auto vertices = mesh.getVertices();

        auto indices = getMeshVertices(mesh, m_batchData->m_meshElementType);

        // Transform the vertices
        for (auto& vertex : vertices) {
            vertex.m_position = transform * vertex.m_position;
            vertex.m_color = color;
        }

        // Transform the indices
        for (auto& index : indices) {
            index += m_batchData->m_vertices.size();
        }

        m_batchData->m_vertices.insert(m_batchData->m_vertices.end(), vertices.begin(), vertices.end());
        m_batchData->m_indices.insert(m_batchData->m_indices.end(), indices.begin(), indices.end());

    }

    void Renderer::flushBatch() {

        engine::BufferLayout layout = {
                {"a_position", 4, engine::VertexBufferLayoutElementType::Float},
                {"a_color", 4, engine::VertexBufferLayoutElementType::Float},
        };

        std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(layout, (const void*) m_batchData->m_vertices.data(), sizeof(Vertex) * m_batchData->m_vertices.size());

        std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>((unsigned int*) m_batchData->m_indices.data(), m_batchData->m_indices.size());

        std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
        vertexArray->addBuffer(vertexBuffer, indexBuffer);

        if (m_batchData->m_meshElementType == MeshElementType::Face) {
            submitTriangles(m_shaderLibrary.get("main"), vertexArray);
        } else {
            submitLines(m_shaderLibrary.get("main"), vertexArray);
        }

        delete m_batchData;
        m_batchData = nullptr;

    }

    void Renderer::submitTriangles(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {

        shader->bind();
        shader->setUniformMat4f("u_viewProjection", m_sceneData->m_viewProjectionMatrix);

        vertexArray->bind();
        vertexArray->getIndexBuffer()->bind();

        RenderCommand::drawIndexedTriangles(vertexArray->getIndexBuffer()->getCount());

    }

    void Renderer::submitLines(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {

        shader->bind();
        shader->setUniformMat4f("u_viewProjection", m_sceneData->m_viewProjectionMatrix);

        vertexArray->bind();
        vertexArray->getIndexBuffer()->bind();

        RenderCommand::drawIndexedLines(vertexArray->getIndexBuffer()->getCount());

    }

    void Renderer::loadDefaultShaders() {

        std::map<engine::ShaderType, std::string> shaderSource {
            {engine::ShaderType::Vertex, ASSETS_PATH"/shaders/2d/polygon-vertex-shader.glsl"},
            {engine::ShaderType::Fragment, ASSETS_PATH"/shaders/2d/polygon-fragment-shader.glsl"}
        };
        m_shaderLibrary.load("main", shaderSource);

    }

    std::vector<unsigned int> Renderer::getMeshVertices(const Mesh &mesh, MeshElementType meshElementType) {

        if (meshElementType == MeshElementType::Face) {
            return mesh.getFaceIndices();
        } else if (meshElementType == MeshElementType::Edge) {
            return mesh.getEdgeIndices();
        } else if (meshElementType == MeshElementType::Polygon) {
            return mesh.getPolygonIndices();
        }

        throw std::runtime_error("Unsupported mesh element type");

    }

}