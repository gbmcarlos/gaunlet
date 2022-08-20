#include "VertexArray.h"

namespace engine {

    VertexArray::VertexArray() {
        RenderCommand::createVertexArray(m_rendererId);
    }
    VertexArray::~VertexArray() {
        RenderCommand::deleteVertexArray(m_rendererId);
    }

    void VertexArray::addBuffer(const BufferLayout& bufferLayout) {

        auto& elements = bufferLayout.getElements();
        for (unsigned int i = 0; i < elements.size(); i++) {
            auto& element = elements[i];
            RenderCommand::addVertexArrayAttribute(
                m_rendererId,
                i,
                element.m_count,
                element.m_type,
                element.m_normalized,
                bufferLayout.getStride(),
                element.m_offset
                );
        }

    }

}