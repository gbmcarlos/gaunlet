#pragma once

#include "Texture.h"

namespace engine {

    class TextureImage2D : public Texture {

    public:
        TextureImage2D() = default;
        explicit TextureImage2D(const std::string& path);
        TextureImage2D(TextureDataFormat internalFormat, TextureDataFormat dataFormat, unsigned int width, unsigned int height, void* data);
        ~TextureImage2D();

        void activate(unsigned int slot) override;

    };

}