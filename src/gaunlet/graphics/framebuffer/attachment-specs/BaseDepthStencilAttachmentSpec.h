#pragma once

#include "gaunlet/graphics/framebuffer/DepthStencilAttachmentSpec.h"

namespace gaunlet::Graphics {

    class BaseDepthStencilAttachmentSpec : public DepthStencilAttachmentSpec {

    public:

        BaseDepthStencilAttachmentSpec(float depthClearValue, int stencilClearValue)
            : m_depthClearValue(depthClearValue), m_stencilClearValue(stencilClearValue) {}

        static Core::Ref<BaseDepthStencilAttachmentSpec> create(float depthClearValue, int stencilClearValue) {

            return Core::CreateRef<BaseDepthStencilAttachmentSpec>(
                depthClearValue, stencilClearValue
            );

        }

        float getDepthClearValue() override {
            return m_depthClearValue;
        }

        int getStencilClearValue() override {
            return m_stencilClearValue;
        }

        std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat, Core::PrimitiveDataType> getTextureParameters() override {
            return {Core::TextureInternalFormat::Depth_24_Stencil_8, Core::TextureExternalFormat::DepthStencil, Core::PrimitiveDataType::UInt24_8};
        }

    private:

        float m_depthClearValue;
        int m_stencilClearValue;

    };

}