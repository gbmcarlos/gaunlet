#pragma once

#include "gaunlet/graphics/vertex-buffer/BufferLayout.h"

namespace gaunlet::Graphics {

    class VertexBuffer {

    private:
        unsigned int m_rendererId;
        bool m_dynamic;
    public:
        VertexBuffer(const void* data, unsigned int size);
        explicit VertexBuffer(unsigned int size);
        ~VertexBuffer();
        void setData(const void* data, unsigned int size);
        inline unsigned int getRendererId() const {return m_rendererId; }
    };

}