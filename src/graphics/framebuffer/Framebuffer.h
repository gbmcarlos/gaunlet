#pragma once

#include "../core/render/RenderCommand.h"
#include "../texture/Texture.h"

#include <vector>
#include <memory>

namespace engine {

    struct FramebufferAttachmentSpecs {

        FramebufferAttachmentSpecs(TextureDataFormat textureDataFormat, TextureType textureType, float width, float height, FramebufferAttachmentType attachmentType, bool draw);

        float m_width, m_height;
        TextureDataFormat m_textureDataFormat;
        TextureType m_textureType;
        FramebufferAttachmentType m_attachmentType;
        bool m_draw;

    };

    class Framebuffer {

    public:
        Framebuffer(const std::initializer_list<FramebufferAttachmentSpecs>& attachmentSpecs);
        ~Framebuffer();
        void bind();
        void unbind();

    private:
        std::vector<FramebufferAttachmentSpecs> m_attachmentSpecs;
        std::vector<std::shared_ptr<Texture> > m_textures;
        unsigned int m_rendererId = 0;

    };

}