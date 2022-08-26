#pragma once

namespace gaunlet::Graphics {

    class TextureLibrary;

    class Texture {

        friend TextureLibrary;

    public:
        virtual void activate(unsigned int slot) = 0;

        inline unsigned int getRendererId() {return m_rendererId; }
        inline unsigned int getWidth() {return m_width; }
        inline unsigned int getHeight() {return m_height; }

    protected:
        unsigned int m_rendererId = 0;
        unsigned int m_width = 0;
        unsigned int m_height = 0;

    };

}