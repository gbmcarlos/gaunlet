#pragma once

#include "gaunlet/core/render/RenderApi.h"

namespace gaunlet::Graphics {

    class ColorAttachmentSpec {

    public:

        enum class Type {
            TYPE_UNI, TYPE_SNI,
            TYPE_SI, TYPE_UI,
            TYPE_F
        };

        enum class Channels {
            CHANNELS_1, CHANNELS_2, CHANNELS_3, CHANNELS_4
        };

        enum class Size {
            SIZE_2, SIZE_4, SIZE_5, SIZE_8,
            SIZE_10, SIZE_12, SIZE_16, SIZE_32
        };

        virtual Core::PrimitiveDataType getClearDataType() = 0;
        virtual void* getClearValue() = 0;
        virtual std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat, Core::PrimitiveDataType> getTextureParameters() = 0;

        virtual std::tuple<Core::TextureExternalFormat, Core::PrimitiveDataType> getReadIntParameters() = 0;
        virtual std::tuple<Core::TextureExternalFormat, Core::PrimitiveDataType> getReadFloatParameters() = 0;
        virtual std::tuple<Core::TextureExternalFormat, Core::PrimitiveDataType> getReadVec2Parameters() = 0;
        virtual std::tuple<Core::TextureExternalFormat, Core::PrimitiveDataType> getReadVec3Parameters() = 0;
        virtual std::tuple<Core::TextureExternalFormat, Core::PrimitiveDataType> getReadVec4Parameters() = 0;

    };

}