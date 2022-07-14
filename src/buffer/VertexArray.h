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
        void bind() const;
        void unbind() const;
        void addBuffer(VertexBuffer& vertexBuffer) const;
    };

}