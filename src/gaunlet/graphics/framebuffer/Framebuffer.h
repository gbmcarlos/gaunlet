#pragma once

#include "gaunlet/core/render/RenderApi.h"
#include "gaunlet/graphics/texture/Texture.h"
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
        void addColorAttachment(ColorAttachmentSpec::Channels channels, ColorAttachmentSpec::Type type, ColorAttachmentSpec::Size size, T clearColor);
        void setDepthStencilAttachment(float depthClearValue, int stencilClearValue);
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
        inline Core::Ref<Texture>& getColorAttachment(unsigned int index) {return m_textures[index]; }

        template<typename T>
        T readPixel(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y);

    private:
        unsigned int m_width, m_height;
        unsigned int m_lastViewportX0, m_lastViewportY0, m_lastViewportX1, m_lastViewportY1;
        unsigned int m_rendererId = 0;

        std::vector<Core::Ref<ColorAttachmentSpec>> m_colorAttachmentSpecs = {};
        Core::Ref<DepthStencilAttachmentSpec> m_depthStencilAttachmentSpec = nullptr;

        std::vector<Core::Ref<Texture>> m_textures;

        void attachColor(const Core::Ref<ColorAttachmentSpec>& colorAttachmentSpec, unsigned int index);
        void attachDepthStencil(const Core::Ref<DepthStencilAttachmentSpec>& depthStencilAttachmentSpec);

    };

    template<typename T>
    void Framebuffer::addColorAttachment(ColorAttachmentSpec::Channels channels, ColorAttachmentSpec::Type type, ColorAttachmentSpec::Size size, T clearColor) {

        m_colorAttachmentSpecs.emplace_back(
            ColorAttachmentSpecFactory::create<T>(channels, type, size, clearColor)
        );

    }

}