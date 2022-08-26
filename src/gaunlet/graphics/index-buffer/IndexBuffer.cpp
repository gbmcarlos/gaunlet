#include "gaunlet/graphics/index-buffer/IndexBuffer.h"

#include "gaunlet/core/render/RenderCommand.h"

namespace gaunlet::Graphics {

    IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count)
        : m_count(count), m_rendererId(0) {
        Core::RenderCommand::createIndexBuffer(m_rendererId, data, count);
    }

    IndexBuffer::~IndexBuffer() {
        Core::RenderCommand::deleteIndexBuffer(m_rendererId);
    }

}