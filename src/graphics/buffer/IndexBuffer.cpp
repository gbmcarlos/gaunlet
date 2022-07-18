#include "IndexBuffer.h"

#include "../../core/render/RenderCommand.h"

namespace engine {

    IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count)
        : m_count(count), m_rendererId(0) {
        RenderCommand::createIndexBuffer(m_rendererId, data, count);
    }
    IndexBuffer::~IndexBuffer() {
        RenderCommand::deleteBuffer(m_rendererId);
    }

    void IndexBuffer::bind() {
        RenderCommand::bindIndexBuffer(m_rendererId);
    }
    void IndexBuffer::unbind() {
        RenderCommand::unbindIndexBuffer();
    }

}