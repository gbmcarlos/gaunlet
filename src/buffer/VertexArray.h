#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace engine {

    class VertexArray {
    private:
        unsigned int rendererId;
    public:
        VertexArray();
        ~VertexArray();
        void bind() const;
        void unbind() const;
        void addBuffer(VertexBuffer& vertexBuffer, VertexBufferLayout& vertexBufferLayout);
    };

}