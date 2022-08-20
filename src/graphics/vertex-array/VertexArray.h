#pragma once

#include "../vertex-buffer/VertexBuffer.h"
#include "../index-buffer/IndexBuffer.h"

namespace engine {

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