#include "VertexBuffer.h"

#include "../render/Renderer.h"

namespace engine {

    VertexBuffer::VertexBuffer(BufferLayout& layout, const void *data, unsigned int size)
        : m_dynamic(false), m_layout{layout} {
        Renderer::createVertexBuffer(m_rendererId, data, size);
    }

    VertexBuffer::VertexBuffer(BufferLayout& layout, unsigned int size)
            : m_dynamic(true), m_layout{layout} {
        Renderer::createVertexBuffer(m_rendererId, size);
    }

    VertexBuffer::~VertexBuffer() {
        Renderer::deleteBuffer(m_rendererId);
    }

    void VertexBuffer::setData(const void *data, unsigned int size) {

        if (!m_dynamic) {
            throw std::runtime_error("Can't set data for non-dynamic vertex buffer");
        }

        bind();
        Renderer::submitVertexBufferData(data, size);
    }

    void VertexBuffer::bind() {
        Renderer::bindVertexBuffer(m_rendererId);
    }
    void VertexBuffer::unbind() {
        Renderer::unbindVertexBuffer();
    }

}