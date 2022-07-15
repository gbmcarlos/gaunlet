#include "IndexBuffer.h"

#include "../render/Renderer.h"

namespace engine {

    IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count)
        : m_count(count), m_rendererId(0) {
        Renderer::createIndexBuffer(m_rendererId, data, count);
    }
    IndexBuffer::~IndexBuffer() {
        Renderer::deleteBuffer(m_rendererId);
    }

    void IndexBuffer::bind() {
        Renderer::bindIndexBuffer(m_rendererId);
    }
    void IndexBuffer::unbind() {
        Renderer::unbindIndexBuffer();
    }

}