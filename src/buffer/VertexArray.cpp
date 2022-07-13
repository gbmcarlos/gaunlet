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

    void VertexArray::addBuffer(VertexBuffer &vertexBuffer, VertexBufferLayout &vertexBufferLayout) {

        bind();
        vertexBuffer.bind();

        const auto& elements = vertexBufferLayout.elements;
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); i++) {
            const auto& element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(
                    i,
                    element.count,
                    element.type,
                    element.normalized ? GL_TRUE : GL_FALSE,
                    vertexBufferLayout.stride,
                    (const void*) offset
                );
            offset += element.count * VertexBufferLayoutElement::getSizeOfType(element.type);
        }

    }

}