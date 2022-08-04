#pragma once

#include "Texture.h"

namespace engine {

    class TextureImage2D : public Texture {

    public:
        explicit TextureImage2D(const std::string& path);
        TextureImage2D(TextureDataFormat internalFormat, TextureDataFormat dataFormat, unsigned int width, unsigned int height, void* data);
        ~TextureImage2D();

        void bind(unsigned int slot) override;

    private:
        unsigned int m_width;
        unsigned int m_height;

    };

}