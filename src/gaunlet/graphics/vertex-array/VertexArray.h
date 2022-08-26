#pragma once

#include "gaunlet/graphics/vertex-buffer/VertexBuffer.h"
#include "gaunlet/graphics/index-buffer/IndexBuffer.h"

namespace gaunlet::Graphics {

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