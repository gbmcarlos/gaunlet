#include "VertexBuffer.h"

namespace engine {

    VertexBuffer::VertexBuffer(BufferLayout& layout, const void *data, unsigned int size)
        : m_dynamic(false), m_layout{layout} {
        RenderCommand::createVertexBuffer(m_rendererId, data, size);
    }

    VertexBuffer::VertexBuffer(BufferLayout& layout, unsigned int size)
            : m_dynamic(true), m_layout{layout} {
        RenderCommand::createVertexBuffer(m_rendererId, size);
    }

    VertexBuffer::~VertexBuffer() {
        RenderCommand::deleteBuffer(m_rendererId);
    }

    void VertexBuffer::setData(const void *data, unsigned int size) {

        if (!m_dynamic) {
            throw std::runtime_error("Can't set data for non-dynamic vertex buffer");
        }

        bind();
        RenderCommand::submitVertexBufferData(data, size);
    }

    void VertexBuffer::bind() {
        RenderCommand::bindVertexBuffer(m_rendererId);
    }
    void VertexBuffer::unbind() {
        RenderCommand::unbindVertexBuffer();
    }

}