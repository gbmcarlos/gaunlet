#pragma once

#include "../core/render/RenderCommand.h"
#include "../texture/Texture.h"

#include <vector>
#include <memory>

namespace engine {

    struct FramebufferAttachmentSpecs {

        FramebufferAttachmentSpecs(TextureDataFormat textureDataFormat, TextureType textureType, FramebufferAttachmentType attachmentType, bool draw);

        TextureDataFormat m_textureDataFormat;
        TextureType m_textureType;
        FramebufferAttachmentType m_attachmentType;
        bool m_draw;

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
        inline std::vector<std::shared_ptr<Texture> >& getTextures() {return m_textures; }

    private:
        unsigned int m_width, m_height;
        std::vector<FramebufferAttachmentSpecs> m_attachmentSpecs;
        std::vector<std::shared_ptr<Texture> > m_textures;
        unsigned int m_lastViewportX0, m_lastViewportY0, m_lastViewportX1, m_lastViewportY1;
        unsigned int m_rendererId = 0;

        void recreate();

    };

}