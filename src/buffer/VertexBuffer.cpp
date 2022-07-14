#include "VertexBuffer.h"

#include "../render/Renderer.h"

namespace engine {

    VertexBuffer::VertexBuffer(BufferLayout& layout, const void *data, unsigned int size)
        : dynamic(false), layout{std::move(layout)} {
        Renderer::createVertexBuffer(rendererId, data, size);
    }

    VertexBuffer::VertexBuffer(BufferLayout& layout, unsigned int size)
            : dynamic(true), layout{std::move(layout)} {
        Renderer::createVertexBuffer(rendererId, size);
    }

    VertexBuffer::~VertexBuffer() {
        Renderer::deleteBuffer(rendererId);
    }

    void VertexBuffer::setData(const void *data, unsigned int size) {

        if (!dynamic) {
            throw std::runtime_error("Can't set data for non-dynamic vertex buffer");
        }

        bind();
        Renderer::submitVertexBufferData(data, size);
    }

    void VertexBuffer::bind() {
        Renderer::bindVertexBuffer(rendererId);
    }
    void VertexBuffer::unbind() {
        Renderer::unbindVertexBuffer();
    }

}