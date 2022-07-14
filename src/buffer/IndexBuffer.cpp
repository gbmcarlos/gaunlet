#include "IndexBuffer.h"

#include "../render/Renderer.h"

namespace engine {

    IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count)
        : count(count), rendererId(0) {
        Renderer::createIndexBuffer(rendererId, data, count);
    }
    IndexBuffer::~IndexBuffer() {
        Renderer::deleteBuffer(rendererId);
    }

    void IndexBuffer::bind() {
        Renderer::bindIndexBuffer(rendererId);
    }
    void IndexBuffer::unbind() {
        Renderer::unbindIndexBuffer();
    }

}