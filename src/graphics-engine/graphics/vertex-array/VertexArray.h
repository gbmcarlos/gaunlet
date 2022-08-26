#pragma once

#include "graphics-engine/graphics/vertex-buffer/VertexBuffer.h"
#include "graphics-engine/graphics/index-buffer/IndexBuffer.h"

namespace engine::Graphics {

    class VertexArray {
    private:
        unsigned int m_rendererId;

    public:
        VertexArray();
        ~VertexArray();
        void addBuffer(const BufferLayout& bufferLayout);
        inline unsigned int getRendererId() const {return m_rendererId; }
    };

}