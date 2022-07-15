#pragma once

namespace engine {

    class IndexBuffer {

    private:
        unsigned int m_rendererId;
        unsigned int m_count;
    public:
        IndexBuffer(unsigned int* data, unsigned int count);
        ~IndexBuffer();
        void bind();
        void unbind();
        inline unsigned int getCount() {return m_count;}
    };

}