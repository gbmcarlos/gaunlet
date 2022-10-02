#pragma once

#include "gaunlet/core/render/RenderApi.h"

namespace gaunlet::Graphics {

    class TextureLibrary;

    class Texture {

        friend TextureLibrary;

    public:
        virtual void activate(unsigned int slot) = 0;

        inline unsigned int getRendererId() {return m_rendererId; }
        inline Core::TextureDataFormat getFormat() {return m_format; }
        inline unsigned int getWidth() {return m_width; }
        inline unsigned int getHeight() {return m_height; }

    protected:

        unsigned int m_rendererId = 0;
        Core::TextureDataFormat m_format = Core::TextureDataFormat::RGBA;
        unsigned int m_width = 0;
        unsigned int m_height = 0;

        Core::TextureDataFormat convertFormat(int channels) {

            if (channels == 3) {
                return Core::TextureDataFormat::RGB;
            } else if (channels == 4) {
                return Core::TextureDataFormat::RGBA;
            }

            throw std::runtime_error("Unknown texture data format");

        }

    };

}