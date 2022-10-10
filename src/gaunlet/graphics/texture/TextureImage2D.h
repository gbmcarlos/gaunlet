#pragma once

#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/graphics/texture/Image.h"
#include "gaunlet/core/render/RenderApi.h"

namespace gaunlet::Graphics {

    class TextureImage2D : public Texture {

    public:
        explicit TextureImage2D(const std::string& path);
        explicit TextureImage2D(const std::string& path, Core::TextureFilteringParameterValue filteringParameterValue, Core::TextureWrappingParameterValue wrappingParameterValue);
        TextureImage2D(Core::TextureInternalFormat internalFormat, Core::TextureExternalFormat dataFormat, Core::PrimitiveDataType dataType, unsigned int width, unsigned int height, void* data);
        ~TextureImage2D();

        void activate(unsigned int slot) override;
        void setFilteringParameters(Core::TextureFilteringParameterValue value);
        void setFilteringParameters(Core::TextureFilteringParameterValue minifyingValue, Core::TextureFilteringParameterValue magnifyingValue);
        void setWrappingParameters(Core::TextureWrappingParameterValue value);
        void setWrappingParameters(Core::TextureWrappingParameterValue sValue, Core::TextureWrappingParameterValue tValue);

    };

}