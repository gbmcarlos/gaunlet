#pragma once

namespace engine::Graphics {

    class IndexBuffer {

    private:
        unsigned int m_rendererId;
        unsigned int m_count;
    public:
        IndexBuffer(unsigned int* data, unsigned int count);
        ~IndexBuffer();
        inline unsigned int getRendererId() const {return m_rendererId; }
        inline unsigned int getCount() const {return m_count;}
    };

}