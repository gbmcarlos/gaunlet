#pragma once

#include "gaunlet/core/render/RenderApi.h"
#include "gaunlet/graphics/texture/TextureImage2D.h"
#include "gaunlet/graphics/framebuffer/attachment-specs/ColorAttachmentSpecFactory.h"
#include "gaunlet/graphics/framebuffer/ColorAttachmentSpec.h"
#include "gaunlet/graphics/framebuffer/DepthStencilAttachmentSpec.h"

#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

    class Framebuffer {

    public:
        Framebuffer(unsigned int width, unsigned int height);
        ~Framebuffer();

        template<typename T>
        const Core::Ref<TextureImage2D>& addColorAttachment(ColorAttachmentSpec::Channels channels, ColorAttachmentSpec::Type type, ColorAttachmentSpec::Size size, T clearColor);
        const Core::Ref<TextureImage2D>& addColorAttachment(const Core::Ref<ColorAttachmentSpec>& colorAttachmentSpec);
        const Core::Ref<TextureImage2D>& setDepthStencilAttachment(float depthClearValue, int stencilClearValue);
        void resize(unsigned int width, unsigned int height);
        void recreate();

        void bind();
        void unbind();
        void clear();
        void clearColorAttachment(unsigned int index);
        void clearDepthStencilAttachment();
        void setDrawBuffers(const std::vector<int>& drawBuffers);
        void deAttachColor(unsigned int colorAttachmentIndex);

        inline unsigned int getRendererId() const {return m_rendererId; }
        inline unsigned int getWidth() {return m_width; }
        inline unsigned int getHeight() {return m_height; }
        const Core::Ref<TextureImage2D>& getColorAttachment(unsigned int index) {return m_textures.at(index); }

        template<typename T>
        T readPixel(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y);

    private:
        unsigned int m_width, m_height;
        unsigned int m_lastViewportX0, m_lastViewportY0, m_lastViewportX1, m_lastViewportY1;
        unsigned int m_rendererId = 0;

        std::vector<Core::Ref<ColorAttachmentSpec>> m_colorAttachmentSpecs = {};
        Core::Ref<DepthStencilAttachmentSpec> m_depthStencilAttachmentSpec = nullptr;

        std::vector<Core::Ref<TextureImage2D>> m_textures;

        void destroyFramebuffer();
        const Core::Ref<TextureImage2D>& createColorAttachment(const Core::Ref<ColorAttachmentSpec>& colorAttachmentSpec, unsigned int index);
        const Core::Ref<TextureImage2D>& createDepthStencilAttachment(const Core::Ref<DepthStencilAttachmentSpec>& depthStencilAttachmentSpec);

    };

    template<typename T>
    const Core::Ref<TextureImage2D>& Framebuffer::addColorAttachment(ColorAttachmentSpec::Channels channels, ColorAttachmentSpec::Type type, ColorAttachmentSpec::Size size, T clearColor) {

        auto colorAttachmentSpec = ColorAttachmentSpecFactory::create<T>(channels, type, size, clearColor);
        return addColorAttachment(colorAttachmentSpec);

    }

}