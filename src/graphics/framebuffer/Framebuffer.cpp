#include "Framebuffer.h"

#include "../texture/TextureImage2D.h"

namespace engine {

    FramebufferAttachmentSpecs::FramebufferAttachmentSpecs(TextureDataFormat textureDataFormat, TextureType textureType, float width, float height, FramebufferAttachmentType attachmentType, bool draw)
        : m_width(width), m_height(height), m_textureDataFormat(textureDataFormat), m_textureType(textureType), m_attachmentType(attachmentType), m_draw(draw) {
    }

    Framebuffer::Framebuffer(const std::initializer_list<FramebufferAttachmentSpecs>& attachmentSpecs) : m_attachmentSpecs(attachmentSpecs) {

        // Create the framebuffer and bind it
        RenderCommand::createFramebuffer(m_rendererId);
        RenderCommand::bindFramebuffer(m_rendererId);

        std::vector<FramebufferAttachmentType> drawBuffers;

        // Create a texture for each attachment, and attach it
        for (auto& attachment : m_attachmentSpecs) {

            // Create the buffer texture
            std::shared_ptr<Texture> texture = std::make_shared<TextureImage2D>(
                attachment.m_textureDataFormat, attachment.m_textureDataFormat,
                attachment.m_width, attachment.m_height,
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

        }

        // Set which buffers are for drawing
        RenderCommand::setDrawBuffers(drawBuffers);

        // Check correctness and unbind
        RenderCommand::checkFramebufferCompleteness(m_rendererId);
        RenderCommand::unbindFramebuffer(m_rendererId);

    }

    Framebuffer::~Framebuffer() {
        RenderCommand::deleteFramebuffer(m_rendererId);
    }

    void Framebuffer::bind() {
        RenderCommand::bindFramebuffer(m_rendererId);
    }

    void Framebuffer::unbind() {
        RenderCommand::bindFramebuffer(m_rendererId);
    }

}