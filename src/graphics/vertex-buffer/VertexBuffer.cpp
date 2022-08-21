#include "VertexBuffer.h"

namespace engine {

    VertexBuffer::VertexBuffer(const void *data, unsigned int size)
        : m_dynamic(false) {
        RenderCommand::createVertexBuffer(m_rendererId, data, size);
    }

    VertexBuffer::VertexBuffer(unsigned int size)
            : m_dynamic(true) {
        RenderCommand::createVertexBuffer(m_rendererId, size);
    }

    VertexBuffer::~VertexBuffer() {
        RenderCommand::deleteVertexBuffer(m_rendererId);
    }

    void VertexBuffer::setData(const void *data, unsigned int size) {

        if (!m_dynamic) {
            throw std::runtime_error("Can't set data for non-dynamic vertex buffer");
        }

        RenderCommand::submitVertexBufferData(m_rendererId, data, size);
    }

}