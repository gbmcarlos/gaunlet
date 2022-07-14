#pragma once

#include "VertexBuffer.h"
#include "BufferLayout.h"

namespace engine {

    class VertexArray {
    private:
        unsigned int rendererId;
    public:
        VertexArray();
        ~VertexArray();
        void bind();
        void unbind();
        void addBuffer(VertexBuffer& vertexBuffer);
    };

}