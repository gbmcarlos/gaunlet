#include "VertexArray.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "../render/RenderCommand.h"

namespace engine {

    VertexArray::VertexArray() {
        RenderCommand::createVertexArray(m_rendererId);
        RenderCommand::unbindVertexArray();
    }
    VertexArray::~VertexArray() {
        RenderCommand::deleteVertexArray(m_rendererId);
    }

    void VertexArray::bind() {
        RenderCommand::bindVertexArray(m_rendererId);
    }

    void VertexArray::unbind() {
        RenderCommand::unbindVertexArray();
    }

    void VertexArray::addBuffer(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer) {

        bind();

        vertexBuffer->bind();
        auto& elements = vertexBuffer->getBufferLayout().getElements();
        for (unsigned int i = 0; i < elements.size(); i++) {
            auto& element = elements[i];
            RenderCommand::addVertexArrayAttribute(
                    i,
                    element.m_count,
                    BufferLayoutElement::layoutElementTypeToGLType(element.m_type),
                    element.m_normalized,
                    vertexBuffer->getBufferLayout().getStride(),
                    element.m_offset
                );
        }

        indexBuffer->bind();

    }

}