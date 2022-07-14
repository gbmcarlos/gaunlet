#pragma once

namespace engine {

    class IndexBuffer {

    private:
        unsigned int rendererId;
        unsigned int count;
    public:
        IndexBuffer(unsigned int* data, unsigned int count);
        ~IndexBuffer();
        void bind();
        void unbind();
        inline unsigned int getCount() {return count;}
    };

}