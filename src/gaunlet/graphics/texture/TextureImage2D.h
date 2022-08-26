#pragma once

#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/core/render/RenderApi.h"

namespace gaunlet::Graphics {

    class TextureImage2D : public Texture {

    public:
        TextureImage2D() = default;
        explicit TextureImage2D(const std::string& path);
        TextureImage2D(Core::TextureDataFormat internalFormat, Core::TextureDataFormat dataFormat, unsigned int width, unsigned int height, void* data);
        ~TextureImage2D();

        void activate(unsigned int slot) override;

    };

}