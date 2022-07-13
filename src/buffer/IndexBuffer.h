#pragma once

namespace engine {

    class IndexBuffer {

    private:
        unsigned int rendererId;

    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();
        unsigned int count;
        void bind() const;
        void unbind() const;
    };

}