#include "gaunlet/graphics/vertex-buffer/VertexBuffer.h"

#include "gaunlet/core/render/RenderCommand.h"

namespace gaunlet::Graphics {

    VertexBuffer::VertexBuffer(const void *data, unsigned int size)
        : m_dynamic(false) {
        Core::RenderCommand::createVertexBuffer(m_rendererId, data, size);
    }

    VertexBuffer::VertexBuffer(unsigned int size)
            : m_dynamic(true) {
        Core::RenderCommand::createVertexBuffer(m_rendererId, size);
    }

    VertexBuffer::~VertexBuffer() {
        Core::RenderCommand::deleteVertexBuffer(m_rendererId);
    }

    void VertexBuffer::setData(const void *data, unsigned int size) {

        if (!m_dynamic) {
            throw std::runtime_error("Can't set data for non-dynamic vertex buffer");
        }

        Core::RenderCommand::submitVertexBufferData(m_rendererId, data, size);
    }

}