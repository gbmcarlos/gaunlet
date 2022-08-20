#pragma once

#include "BufferLayout.h"

namespace engine {

    class VertexBuffer {

    private:
        unsigned int m_rendererId;
        bool m_dynamic;
        BufferLayout m_layout;
    public:
        VertexBuffer(BufferLayout& layout, const void* data, unsigned int size);
        VertexBuffer(BufferLayout& layout, unsigned int size);
        ~VertexBuffer();
        void setData(const void* data, unsigned int size);
        inline unsigned int getRendererId() const {return m_rendererId; }
        inline BufferLayout& getBufferLayout() {return m_layout;}
    };

}