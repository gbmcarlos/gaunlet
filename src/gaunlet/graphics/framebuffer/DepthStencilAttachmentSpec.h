#pragma once

#include "gaunlet/core/render/RenderApi.h"

namespace gaunlet::Graphics {

    class DepthStencilAttachmentSpec {

    public:

        virtual float getDepthClearValue() = 0;
        virtual int getStencilClearValue() = 0;
        virtual std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat, Core::PrimitiveDataType> getTextureParameters() = 0;

    };

}