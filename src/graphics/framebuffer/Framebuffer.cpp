#include "Framebuffer.h"

#include "../texture/TextureImage2D.h"

namespace engine {

    FramebufferAttachmentSpecs::FramebufferAttachmentSpecs(TextureDataFormat textureDataFormat, TextureType textureType, FramebufferAttachmentType attachmentType)
        : m_textureDataFormat(textureDataFormat), m_textureType(textureType), m_attachmentType(attachmentType) {
    }

    Framebuffer::Framebuffer(const std::initializer_list<FramebufferAttachmentSpecs>& attachmentSpecs, unsigned int width, unsigned int height) : m_width(width), m_height(height) {

        for (auto& attachmentSpec : attachmentSpecs) {
            if (attachmentSpec.m_attachmentType == FramebufferAttachmentType::Color) {
                m_colorAttachmentSpecs.emplace_back(attachmentSpec);
            } else if (attachmentSpec.m_attachmentType == FramebufferAttachmentType::Depth) {
                m_depthAttachmentSpec = attachmentSpec;
            }
        }

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

        std::vector<FramebufferAttachmentType> drawBuffers = {};

        // Create and attach the color textures
        for (unsigned int i = 0; i < m_colorAttachmentSpecs.size(); i++) {
            auto& colorAttachmentSpec = m_colorAttachmentSpecs[i];
            // Create the buffer texture
            std::shared_ptr<Texture> texture = std::make_shared<TextureImage2D>(
                colorAttachmentSpec.m_textureDataFormat, colorAttachmentSpec.m_textureDataFormat,
                m_width, m_height,
                nullptr
            );
            m_textures.push_back(texture);

            // Attach the texture to the framebuffer
            RenderCommand::framebufferAttach(
                colorAttachmentSpec.m_textureType,
                colorAttachmentSpec.m_attachmentType,
                i,
                texture->getRendererId()
            );

            drawBuffers.emplace_back(FramebufferAttachmentType::Color);

        }

        // Create and attach the depth texture, if any
        if (m_depthAttachmentSpec.m_attachmentType != FramebufferAttachmentType::None) {

            // Create the buffer texture
            std::shared_ptr<Texture> texture = std::make_shared<TextureImage2D>(
                m_depthAttachmentSpec.m_textureDataFormat, m_depthAttachmentSpec.m_textureDataFormat,
                m_width, m_height,
                nullptr
            );
            m_textures.push_back(texture);

            // Attach the texture to the framebuffer
            RenderCommand::framebufferAttach(
                m_depthAttachmentSpec.m_textureType,
                m_depthAttachmentSpec.m_attachmentType,
                0,
                texture->getRendererId()
            );

            drawBuffers.emplace_back(FramebufferAttachmentType::None);

        }

        // If there are more than one color attachment, tell opengl about them
        if (m_colorAttachmentSpecs.size() > 1) {
            RenderCommand::setDrawBuffers(drawBuffers);
        }

        // Check correctness and unbind
        RenderCommand::checkFramebufferCompleteness(m_rendererId);
        RenderCommand::unbindFramebuffer();

    }

}