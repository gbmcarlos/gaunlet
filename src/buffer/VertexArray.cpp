#include "VertexArray.h"

#include "../render/Renderer.h"

namespace engine {

    VertexArray::VertexArray() {
        Renderer::createVertexArray(rendererId);
    }
    VertexArray::~VertexArray() {
        Renderer::deleteVertexArray(rendererId);
    }

    void VertexArray::bind() {
        Renderer::bindVertexArray(rendererId);
    }

    void VertexArray::unbind() {
        Renderer::unbindVertexArray();
    }

    void VertexArray::addBuffer(VertexBuffer &vertexBuffer) {

        bind();
        vertexBuffer.bind();

        const auto& elements = vertexBuffer.getBufferLayout().getElements();
        for (unsigned int i = 0; i < elements.size(); i++) {
            const auto& element = elements[i];
            Renderer::addVertexArrayAttribute(
                    i,
                    element.count,
                    BufferLayoutElement::layoutElementTypeToGLType(element.type),
                    element.normalized,
                    vertexBuffer.getBufferLayout().getStride(),
                    element.offset
                );
        }

    }

}