#pragma once

#include "gaunlet/core/render/RenderApi.h"

namespace gaunlet::Graphics {

    class TextureLibrary;

    class Texture {

        friend TextureLibrary;

    public:
        virtual void activate(unsigned int slot) = 0;

        inline unsigned int getRendererId() {return m_rendererId; }
        inline Core::TextureExternalFormat getFormat() {return m_format; }
        inline unsigned int getWidth() {return m_width; }
        inline unsigned int getHeight() {return m_height; }

    protected:

        unsigned int m_rendererId = 0;
        Core::TextureExternalFormat m_format = Core::TextureExternalFormat::RGBA;
        unsigned int m_width = 0;
        unsigned int m_height = 0;

        Core::TextureInternalFormat convertToInternalFormat(int channels) {

            if (channels == 3) {
                return Core::TextureInternalFormat::RGB_8_UNI;
            } else if (channels == 4) {
                return Core::TextureInternalFormat::RGBA_8_UNI;
            }

            throw std::runtime_error("Unknown texture data format");

        }

        Core::TextureExternalFormat convertToExternalFormat(int channels) {

            if (channels == 3) {
                return Core::TextureExternalFormat::RGB;
            } else if (channels == 4) {
                return Core::TextureExternalFormat::RGBA;
            }

            throw std::runtime_error("Unknown texture data format");

        }

    };

}