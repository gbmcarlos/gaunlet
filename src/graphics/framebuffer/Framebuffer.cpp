#include "Framebuffer.h"

#include "../texture/TextureImage2D.h"

namespace engine {

    FramebufferAttachmentSpec::FramebufferAttachmentSpec(FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, const glm::vec4& clearColorValue)
        : m_attachmentType(attachmentType), m_dataFormat(framebufferDataFormat), m_clearColorVec4Value(clearColorValue) {

        if (framebufferDataFormat != FramebufferDataFormat::RGBA) {
            throw std::runtime_error("Clear color type doesn't match the data type");
        }

    }

    FramebufferAttachmentSpec::FramebufferAttachmentSpec(FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, int clearColorValue)
        : m_attachmentType(attachmentType), m_dataFormat(framebufferDataFormat), m_clearColorIntValue(clearColorValue) {

        if (framebufferDataFormat != FramebufferDataFormat::Integer) {
            throw std::runtime_error("Clear color type doesn't match the data type");
        }

    }

    FramebufferAttachmentSpec::FramebufferAttachmentSpec(FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat)
        : m_attachmentType(attachmentType), m_dataFormat(framebufferDataFormat) {
    }

    Framebuffer::Framebuffer(const std::initializer_list<FramebufferAttachmentSpec>& attachmentSpecs, unsigned int width, unsigned int height) : m_width(width), m_height(height) {

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

    void Framebuffer::clear() {

        for (unsigned int i = 0; i < m_colorAttachmentSpecs.size(); i++) {

            auto& colorAttachmentSpec = m_colorAttachmentSpecs[i];

            switch (colorAttachmentSpec.m_dataFormat) {
                case FramebufferDataFormat::RGBA:
                    RenderCommand::clearColorAttachment(m_rendererId, i, PrimitiveDataType::Float, glm::value_ptr(colorAttachmentSpec.m_clearColorVec4Value));
                    break;
                case FramebufferDataFormat::Integer:
                    RenderCommand::clearColorAttachment(m_rendererId, i, PrimitiveDataType::Int, &colorAttachmentSpec.m_clearColorIntValue);
                    break;
                default:
                    throw std::runtime_error("Unsupported attachment data type for clear color");
            }

        }

        if (m_depthAttachmentSpec.m_attachmentType != FramebufferAttachmentType::None) {
            RenderCommand::clearDepthAttachment(m_rendererId);
        }

    }

    void Framebuffer::resize(unsigned int width, unsigned int height) {
        m_width = width;
        m_height = height;
        recreate();
        RenderCommand::setViewport(0, 0, m_width, m_height);
    }

    int Framebuffer::readPixel(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];

        if (colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::Integer) {
            throw std::runtime_error("Color attachment doesn't have INT data");
        }

        int data;
        RenderCommand::readFramebuffer(
            m_rendererId,
            FramebufferAttachmentType::Color, colorAttachmentIndex,
            TextureDataFormat::RedInteger, PrimitiveDataType::Int,
            x, y, 1, 1,
            &data
        );

        return data;

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
            attachColor(colorAttachmentSpec, i);
            drawBuffers.emplace_back(FramebufferAttachmentType::Color);

        }

        // Create and attach the depth texture, if any
        if (m_depthAttachmentSpec.m_attachmentType != FramebufferAttachmentType::None) {

            attachDepth(m_depthAttachmentSpec);
            drawBuffers.emplace_back(FramebufferAttachmentType::None);

        }

        // If there are more than one color attachment, tell opengl about them
        if (m_colorAttachmentSpecs.size() > 1) {
            RenderCommand::setDrawBuffers(m_rendererId, drawBuffers);
        }

        // Check correctness and unbind
        RenderCommand::checkFramebufferCompleteness(m_rendererId);
        RenderCommand::unbindFramebuffer();

    }

    void Framebuffer::attachColor(FramebufferAttachmentSpec colorAttachmentSpec, unsigned int index) {

        TextureDataFormat internalFormat;
        TextureDataFormat format;

        switch (colorAttachmentSpec.m_dataFormat) {
            case FramebufferDataFormat::RGBA:
                internalFormat = TextureDataFormat::RGBA;
                format = TextureDataFormat::RGBA;
                break;
            case FramebufferDataFormat::Integer:
                internalFormat = TextureDataFormat::RedInteger32;
                format = TextureDataFormat::RedInteger;
                break;
            case FramebufferDataFormat::Depth:
                throw std::runtime_error("Invalid framebuffer data format");
        }

        std::shared_ptr<Texture> texture = std::make_shared<TextureImage2D>(
            internalFormat, format,
            m_width, m_height,
            nullptr
        );
        m_textures.push_back(texture);

        // Attach the texture to the framebuffer
        RenderCommand::framebufferAttach(
            m_rendererId,
            engine::TextureType::Image2D,
            colorAttachmentSpec.m_attachmentType,
            index,
            texture->getRendererId()
        );

    }

    void Framebuffer::attachDepth(FramebufferAttachmentSpec depthAttachmentSpec) {

        // Create the buffer texture
        std::shared_ptr<Texture> texture = std::make_shared<TextureImage2D>(
            TextureDataFormat::Depth, TextureDataFormat::Depth,
            m_width, m_height,
            nullptr
        );
        m_textures.push_back(texture);

        // Attach the texture to the framebuffer
        RenderCommand::framebufferAttach(
            m_rendererId,
            engine::TextureType::Image2D,
            m_depthAttachmentSpec.m_attachmentType,
            0,
            texture->getRendererId()
        );

    }

}