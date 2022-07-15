#include "Renderer.h"

#include "RenderCommand.h"

namespace engine {

    Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData{OrthographicCamera::getDefaultViewProjectionMatrix()};

    void Renderer::beginScene(const std::shared_ptr<OrthographicCamera>& orthographicCamera) {
        m_sceneData->m_viewProjectionMatrix = orthographicCamera->getViewProjectionMatrix();
    }

    void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {

        shader->bind();
        shader->setUniformMat4f("u_viewProjection", m_sceneData->m_viewProjectionMatrix);

        vertexArray->bind();
        vertexArray->getIndexBuffer()->bind();

        RenderCommand::drawIndexedTriangles(vertexArray);

    }

}