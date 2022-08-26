#include "gaunlet/graphics/forward-renderer/ForwardRenderer.h"

#include "gaunlet/graphics/vertex-buffer/VertexBuffer.h"
#include "gaunlet/graphics/index-buffer/IndexBuffer.h"
#include "gaunlet/graphics/vertex-array/VertexArray.h"
#include "gaunlet/core/render/RenderCommand.h"

namespace engine::Graphics {

    void ForwardRenderer::renderMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Core::Ref<Graphics::Texture>>& textures, const Core::Ref<Graphics::Shader>& shader, const Core::Ref<Graphics::Framebuffer>& framebuffer) {

        // Create a layout, based on the structure of PolygonVertex
        static BufferLayout vertexLayout = {
            {"a_position", 4, Core::PrimitiveDataType::Float},
            {"a_normal", 3, Core::PrimitiveDataType::Float},
            {"a_textureCoordinates", 2, Core::PrimitiveDataType::Float},
            {"a_entityIndex", 1, Core::PrimitiveDataType::UInt},
        };

        // Create the vertex and index buffers
        VertexBuffer vertexBuffer((const void*) vertices.data(), sizeof(Vertex) * vertices.size());
        IndexBuffer indexBuffer((unsigned int*) indices.data(), indices.size());

        // Bind them together into a vertex array
        VertexArray vertexArray;
        vertexArray.addBuffer(vertexLayout);

        // Bind all the textures
        for (unsigned int i = 0; i < textures.size(); i++) {
            textures[i]->activate(i);
        }

        if (framebuffer != nullptr) {
            framebuffer->bind();
        }

        // Render the polygons as triangles
        Core::RenderCommand::drawIndexedTriangles(
            vertexBuffer.getRendererId(),
            indexBuffer.getRendererId(),
            vertexArray.getRendererId(),
            shader->getRendererId(),
            indexBuffer.getCount()
        );

        if (framebuffer != nullptr) {
            framebuffer->unbind();
        }

    }

}