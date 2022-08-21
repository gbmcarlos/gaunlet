#include "ForwardRenderer.h"

namespace engine {

    void ForwardRenderer::renderPolygons(const std::vector<PolygonVertex>& polygonVertices, const std::vector<unsigned int> &indices, const std::vector<Ref<Texture>>& textures, const Ref<Shader>& shader, const Ref<Framebuffer>& framebuffer) {

        // Create a layout, based on the structure of PolygonVertex
        static BufferLayout polygonVerticesLayout = {
            {"a_position", 4, engine::PrimitiveDataType::Float},
            {"a_normal", 3, engine::PrimitiveDataType::Float},
            {"a_textureCoordinates", 2, engine::PrimitiveDataType::Float},
            {"a_entityIndex", 1, engine::PrimitiveDataType::UInt},
        };

        renderTriangles<PolygonVertex>(
            polygonVerticesLayout,
            polygonVertices,
            indices,
            textures,
            shader,
            framebuffer
        );

    }

    void ForwardRenderer::renderCircles(const std::vector<CircleVertex>& circleVertices, const std::vector<unsigned int> &indices, const std::vector<Ref<Texture>>& textures, const Ref<Shader>& shader, const Ref<Framebuffer>& framebuffer) {

        // Create a layout, based on the structure of CircleVertex
        static BufferLayout circleVerticesLayout = {
            {"a_position", 4, engine::PrimitiveDataType::Float},
            {"a_localCoordinates", 2, engine::PrimitiveDataType::Float},
            {"a_textureCoordinates", 2, engine::PrimitiveDataType::Float},
            {"a_entityIndex", 1, engine::PrimitiveDataType::UInt},
        };

        renderTriangles<CircleVertex>(
            circleVerticesLayout,
            circleVertices,
            indices,
            textures,
            shader,
            framebuffer
        );

    }

    template<typename T>
    void ForwardRenderer::renderTriangles(BufferLayout& bufferLayout, const std::vector<T>& vertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures, const Ref<Shader>& shader, const Ref<Framebuffer>& framebuffer) {

        // Create the vertex and index buffers
        VertexBuffer vertexBuffer((const void*) vertices.data(), sizeof(T) * vertices.size());
        IndexBuffer indexBuffer((unsigned int*) indices.data(), indices.size());

        // Bind them together into a vertex array
        VertexArray vertexArray;
        vertexArray.addBuffer(bufferLayout);

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