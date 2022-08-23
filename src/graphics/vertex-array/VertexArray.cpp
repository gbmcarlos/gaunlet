#include "graphics/vertex-array/VertexArray.h"

#include "core/render/RenderCommand.h"

namespace engine::Graphics {

    VertexArray::VertexArray() {
        Core::RenderCommand::createVertexArray(m_rendererId);
    }
    VertexArray::~VertexArray() {
        Core::RenderCommand::deleteVertexArray(m_rendererId);
    }

    void VertexArray::addBuffer(const BufferLayout& bufferLayout) {

        auto& elements = bufferLayout.getElements();
        for (unsigned int i = 0; i < elements.size(); i++) {
            auto& element = elements[i];
            Core::RenderCommand::addVertexArrayAttribute(
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