#pragma once

namespace engine {

    class VertexBuffer {

    private:
        unsigned int rendererId;

    public:
        VertexBuffer(const void* data, unsigned int size);
        VertexBuffer(unsigned int size);
        ~VertexBuffer();
        void setData(const void* data, unsigned int size);
        void bind() const;
        void unbind() const;
    };

}