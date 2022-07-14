#include "VertexArray.h"

#include <GL/glew.h>

namespace engine {

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &rendererId);
    };
    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &rendererId);
    };

    void VertexArray::bind() const {
        glBindVertexArray(rendererId);
    }

    void VertexArray::unbind() const {
        glBindVertexArray(0);
    }

    void VertexArray::addBuffer(VertexBuffer &vertexBuffer) const {

        bind();
        vertexBuffer.bind();

        const auto& elements = vertexBuffer.getBufferLayout().getElements();
        for (unsigned int i = 0; i < elements.size(); i++) {
            const auto& element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(
                    i,
                    element.count,
                    BufferLayoutElement::layoutElementTypeToGLType(element.type),
                    element.normalized ? GL_TRUE : GL_FALSE,
                    vertexBuffer.getBufferLayout().getStride(),
                    (const void*) element.offset
                );
        }

    }

}