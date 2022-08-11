#include "Framebuffer.h"

#include "../texture/TextureImage2D.h"

namespace engine {

    FramebufferAttachmentSpecs::FramebufferAttachmentSpecs(TextureDataFormat textureDataFormat, TextureType textureType, FramebufferAttachmentType attachmentType, bool draw)
        : m_textureDataFormat(textureDataFormat), m_textureType(textureType), m_attachmentType(attachmentType), m_draw(draw) {
    }

    Framebuffer::Framebuffer(const std::initializer_list<FramebufferAttachmentSpecs>& attachmentSpecs, unsigned int width, unsigned int height) : m_attachmentSpecs(attachmentSpecs), m_width(width), m_height(height) {
        recreate();
    }

    Framebuffer::~Framebuffer() {
        if (m_rendererId) {
            RenderCommand::deleteFramebuffer(m_rendererId);
            for (auto& texture : m_textures) {
                unsigned int textureRendererId = texture->getRendererId();
                RenderCommand::deleteTexture(textureRendererId);
            }
            m_textures.clear();
        }
    }

    void Framebuffer::bind() {

        RenderCommand::getViewport(m_lastViewportX0, m_lastViewportY0, m_lastViewportX1, m_lastViewportY1);
        RenderCommand::bindFramebuffer(m_rendererId);
        RenderCommand::setViewport(0, 0, m_width, m_height);
    }

    void Framebuffer::unbind() {
        RenderCommand::unbindFramebuffer();
        RenderCommand::setViewport(m_lastViewportX0, m_lastViewportY0, m_lastViewportX1, m_lastViewportY1);
    }

    void Framebuffer::resize(unsigned int width, unsigned int height) {
        m_width = width;
        m_height = height;
        recreate();
        RenderCommand::setViewport(0, 0, m_width, m_height);
    }

    void Framebuffer::recreate() {

        if (m_rendererId) {
            RenderCommand::deleteFramebuffer(m_rendererId);
            for (auto& texture : m_textures) {
                unsigned int textureRendererId = texture->getRendererId();
                RenderCommand::deleteTexture(textureRendererId);
            }
            m_textures.clear();
        }

        // Create the framebuffer and bind it
        RenderCommand::createFramebuffer(m_rendererId);
        RenderCommand::bindFramebuffer(m_rendererId);

        std::vector<FramebufferAttachmentType> drawBuffers;

        // Create a texture for each attachment, and attach it
        for (auto& attachment : m_attachmentSpecs) {

            // Create the buffer texture
            std::shared_ptr<Texture> texture = std::make_shared<TextureImage2D>(
                attachment.m_textureDataFormat, attachment.m_textureDataFormat,
                m_width, m_height,
                nullptr
            );
            m_textures.push_back(texture);

            // Attach the texture to the framebuffer
            RenderCommand::framebufferAttach(
                attachment.m_textureType,
                attachment.m_attachmentType,
                texture->getRendererId()
            );

            if (attachment.m_draw) {
                drawBuffers.push_back(attachment.m_attachmentType);
            } else {
                drawBuffers.push_back(FramebufferAttachmentType::None);
            }

            RenderCommand::setDrawBuffers(drawBuffers);

        }

        // Check correctness and unbind
        RenderCommand::checkFramebufferCompleteness(m_rendererId);
        RenderCommand::unbindFramebuffer();

    }

}