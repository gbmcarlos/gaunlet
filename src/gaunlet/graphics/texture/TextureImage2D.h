#pragma once

#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/graphics/texture/Image.h"
#include "gaunlet/core/render/RenderApi.h"

namespace gaunlet::Graphics {

    class TextureImage2D : public Texture {

    public:
        explicit TextureImage2D(const std::string& path);
        TextureImage2D(Core::TextureInternalFormat internalFormat, Core::TextureExternalFormat dataFormat, Core::PrimitiveDataType dataType, unsigned int width, unsigned int height, void* data);
        ~TextureImage2D();

        void activate(unsigned int slot) override;

    };

}