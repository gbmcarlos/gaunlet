#include "Renderer.h"

#include "RenderCommand.h"

namespace engine {

    Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData{OrthographicCamera::getDefaultViewProjectionMatrix()};
    Renderer::BatchData* Renderer::m_batchData = nullptr;

    void Renderer::beginScene(const std::shared_ptr<OrthographicCamera>& orthographicCamera) {
        m_sceneData->m_viewProjectionMatrix = orthographicCamera->getViewProjectionMatrix();
    }

    void Renderer::beginBatch(const std::shared_ptr<Shader> &shader) {

        Renderer::m_batchData = new BatchData(shader);

    }

    void Renderer::submit(const Mesh& mesh, const glm::mat4& transform) {

        auto vertices = mesh.getVertices();
        auto indices = mesh.getIndices();

        // Transform the vertices
        for (auto& vertex : vertices) {
            vertex.m_position = transform * vertex.m_position;
        }

        // Transform the indices
        for (auto& index : indices) {
            index += m_batchData->m_vertices.size();
        }

        m_batchData->m_vertices.insert(m_batchData->m_vertices.end(), vertices.begin(), vertices.end());
        m_batchData->m_indices.insert(m_batchData->m_indices.end(), indices.begin(), indices.end());

    }

    void Renderer::submitBatch() {

        engine::BufferLayout layout = {
                {"a_position", 4, engine::LayoutElementType::Float},
                {"a_color", 4, engine::LayoutElementType::Float},
        };

        std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(layout, (const void*) m_batchData->m_vertices.data(), sizeof(Vertex) * m_batchData->m_vertices.size());

        std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>((unsigned int*) m_batchData->m_indices.data(), m_batchData->m_indices.size());

        std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
        vertexArray->addBuffer(vertexBuffer, indexBuffer);

        submit(m_batchData->m_shader, vertexArray);

    }

    void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {

        shader->bind();
        shader->setUniformMat4f("u_viewProjection", m_sceneData->m_viewProjectionMatrix);
        shader->setUniform4f("u_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        vertexArray->bind();
        vertexArray->getIndexBuffer()->bind();

        RenderCommand::drawIndexedTriangles(vertexArray);

    }

}