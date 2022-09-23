#include "gaunlet/graphics/render-pass/SimpleRenderPass.h"

#include "gaunlet/graphics/vertex-buffer/VertexBuffer.h"
#include "gaunlet/graphics/index-buffer/IndexBuffer.h"
#include "gaunlet/graphics/vertex-array/VertexArray.h"
#include "gaunlet/core/render/RenderCommand.h"

namespace gaunlet::Graphics {

    void SimpleRenderPass::renderIndexedVertices(const std::vector<gaunlet::Graphics::Vertex> &vertices, const std::vector<unsigned int> &indices, const Core::Ref<Graphics::Shader> &shader, RenderMode mode) {

        // Create a layout, based on the structure of PolygonVertex
        static gaunlet::Graphics::BufferLayout vertexLayout = gaunlet::Graphics::Vertex::getBufferLayout();

        // Create the vertex and index buffers
        gaunlet::Graphics::VertexBuffer vertexBuffer((const void *) vertices.data(),
                                                     sizeof(Vertex) * vertices.size());
        gaunlet::Graphics::IndexBuffer indexBuffer((unsigned int *) indices.data(), indices.size());

        // Bind them together into a vertex array
        gaunlet::Graphics::VertexArray vertexArray;
        vertexArray.addBuffer(vertexLayout);

        if (mode == RenderMode::Triangle) {
            Core::RenderCommand::drawIndexedTriangles(
                vertexBuffer.getRendererId(),
                indexBuffer.getRendererId(),
                vertexArray.getRendererId(),
                shader->getRendererId(),
                indexBuffer.getCount()
            );
        } else if (mode == RenderMode::Quad) {
            Core::RenderCommand::drawIndexedQuads(
                vertexBuffer.getRendererId(),
                indexBuffer.getRendererId(),
                vertexArray.getRendererId(),
                shader->getRendererId(),
                vertices.size()
            );
        }

    }

}
