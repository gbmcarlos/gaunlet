#include "ForwardRenderer.h"

#include "../../graphics/vertex-buffer/VertexBuffer.h"
#include "../../graphics/index-buffer/IndexBuffer.h"
#include "../../graphics/vertex-array/VertexArray.h"

namespace engine {

    void ForwardRenderer::renderMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures, const Ref<Shader>& shader, const Ref<Framebuffer>& framebuffer) {

        // Create a layout, based on the structure of PolygonVertex
        static BufferLayout vertexLayout = {
            {"a_position", 4, engine::PrimitiveDataType::Float},
            {"a_normal", 3, engine::PrimitiveDataType::Float},
            {"a_textureCoordinates", 2, engine::PrimitiveDataType::Float},
            {"a_entityIndex", 1, engine::PrimitiveDataType::UInt},
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
        RenderCommand::drawIndexedTriangles(
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