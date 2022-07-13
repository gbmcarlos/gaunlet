#include "VertexBufferLayout.h"

#include <GL/glew.h>

namespace engine {

    VertexBufferLayout::VertexBufferLayout()
        : stride(0) {}

    VertexBufferLayout::~VertexBufferLayout() {}

    void VertexBufferLayout::pushFloat(unsigned int count) {
        elements.push_back({
            GL_FLOAT,
            count,
            false
        });
        stride += count* VertexBufferLayoutElement::getSizeOfType(GL_FLOAT);
    }

}