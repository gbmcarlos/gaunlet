#include "gaunlet/graphics/vertex-buffer/BufferLayout.h"

#include "gaunlet/core/render/RenderCommand.h"

namespace gaunlet::Graphics {

    BufferLayoutElement::BufferLayoutElement(std::string  name, unsigned int count, Core::PrimitiveDataType type, bool normalized)
        : m_name(std::move(name)),
          m_count(count),
          m_type(type),
          m_size(count * Core::RenderCommand::sizeOfLayoutElementType(type)),
          m_offset(0),
          m_normalized(normalized) {}

    BufferLayout::BufferLayout(const std::initializer_list<BufferLayoutElement>& elements)
        : m_elements(elements) {
        calculateStrideAndOffsets();
    }

    void BufferLayout::calculateStrideAndOffsets() {
        m_stride = 0;
        unsigned int offset = 0;
        for (auto& element : m_elements) {
            element.m_offset += offset;
            offset += element.m_size;
            m_stride += element.m_size;
        }
    }

}