#include "IndexBuffer.h"

#include <GL/glew.h>

namespace engine {

    IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
        : count(count), rendererId(0) {
        glGenBuffers(1, &rendererId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }
    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &rendererId);
    }

    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
    }
    void IndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}