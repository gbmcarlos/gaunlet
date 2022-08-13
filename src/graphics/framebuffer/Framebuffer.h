#pragma once

#include "../core/render/RenderCommand.h"
#include "../texture/Texture.h"

#include <vector>
#include <memory>

namespace engine {

    struct FramebufferAttachmentSpecs {

        FramebufferAttachmentSpecs(TextureDataFormat textureDataFormat, TextureType textureType, FramebufferAttachmentType attachmentType);

        TextureDataFormat m_textureDataFormat;
        TextureType m_textureType;
        FramebufferAttachmentType m_attachmentType;

    };

    class Framebuffer {

    public:
        Framebuffer(const std::initializer_list<FramebufferAttachmentSpecs>& attachmentSpecs, unsigned int width, unsigned int height);
        ~Framebuffer();

        void bind();
        void unbind();
        void resize(unsigned int width, unsigned int height);

        inline unsigned int getWidth() {return m_width; }
        inline unsigned int getHeight() {return m_height; }
        inline std::shared_ptr<Texture>& getColorAttachment(unsigned int index) {return m_textures[index]; }

    private:
        unsigned int m_width, m_height;
        unsigned int m_lastViewportX0, m_lastViewportY0, m_lastViewportX1, m_lastViewportY1;
        unsigned int m_rendererId = 0;

        std::vector<FramebufferAttachmentSpecs> m_colorAttachmentSpecs = {};
        FramebufferAttachmentSpecs m_depthAttachmentSpec = {TextureDataFormat::Depth, TextureType::Image2D, FramebufferAttachmentType::None};

        std::vector<std::shared_ptr<Texture> > m_textures;

        void recreate();

    };

}