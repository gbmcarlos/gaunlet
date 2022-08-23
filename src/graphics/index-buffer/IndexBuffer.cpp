#include "graphics/index-buffer/IndexBuffer.h"

#include "core/render/RenderCommand.h"

namespace engine::Graphics {

    IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count)
        : m_count(count), m_rendererId(0) {
        Core::RenderCommand::createIndexBuffer(m_rendererId, data, count);
    }

    IndexBuffer::~IndexBuffer() {
        Core::RenderCommand::deleteIndexBuffer(m_rendererId);
    }

}