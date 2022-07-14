#pragma once

#include <vector>

#include "BufferLayout.h"

namespace engine {

    class VertexBuffer {

    private:
        unsigned int rendererId;
        bool dynamic;
        BufferLayout layout;
    public:
        VertexBuffer(BufferLayout& layout, const void* data, unsigned int size);
        VertexBuffer(BufferLayout& layout, unsigned int size);
        ~VertexBuffer();
        void setData(const void* data, unsigned int size);
        void bind() const;
        void unbind() const;
        inline BufferLayout getBufferLayout() {return layout;}
    };

}