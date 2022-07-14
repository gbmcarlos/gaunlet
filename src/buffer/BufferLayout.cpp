#include "BufferLayout.h"

namespace engine {

    BufferLayoutElement::BufferLayoutElement(std::string  name, unsigned int count, LayoutElementType type, bool normalized)
        :   name(std::move(name)),
            count(count),
            type(type),
            size(count * sizeof(layoutElementTypeToGLType(type))),
            offset(0),
            normalized(normalized) {}

    BufferLayout::BufferLayout(const std::initializer_list<BufferLayoutElement>& elements)
        : elements(elements) {
        calculateStrideAndOffsets();
    }

    void BufferLayout::calculateStrideAndOffsets() {
        stride = 0;
        unsigned int offset = 0;
        for (auto& element : elements) {
            element.offset += offset;
            offset += element.size;
            stride += element.size;
        }
    }

}