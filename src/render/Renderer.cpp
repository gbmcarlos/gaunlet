#include "Renderer.h"

#include "RenderCommand.h"

namespace engine {

//    void Renderer::beginScene(std::shared_ptr<OrthographicCamera> orthographicCamera) {
//
//    }

    void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {

        shader->bind();

        vertexArray->bind();
        vertexArray->getIndexBuffer()->bind();

        RenderCommand::drawIndexedTriangles(vertexArray);

    }

}