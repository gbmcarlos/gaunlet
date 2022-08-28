#include "gaunlet/graphics/framebuffer/Framebuffer.h"

#include "gaunlet/core/render/RenderCommand.h"
#include "gaunlet/graphics/texture/TextureImage2D.h"

namespace gaunlet::Graphics {

    FramebufferAttachmentSpec::FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, const glm::vec4& clearColorValue)
        : m_attachmentType(attachmentType), m_dataFormat(framebufferDataFormat), m_clearColorVec4Value(clearColorValue) {

        if (framebufferDataFormat != FramebufferDataFormat::RGBA) {
            throw std::runtime_error("Clear color type doesn't match the data type");
        }

    }

    FramebufferAttachmentSpec::FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, int clearColorValue)
        : m_attachmentType(attachmentType), m_dataFormat(framebufferDataFormat), m_clearColorIntValue(clearColorValue) {

        if (framebufferDataFormat != FramebufferDataFormat::Integer) {
            throw std::runtime_error("Clear color type doesn't match the data type");
        }

    }

    FramebufferAttachmentSpec::FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat)
        : m_attachmentType(attachmentType), m_dataFormat(framebufferDataFormat) {
    }

    Framebuffer::Framebuffer(const std::initializer_list<FramebufferAttachmentSpec>& attachmentSpecs, unsigned int width, unsigned int height) : m_width(width), m_height(height) {

        for (auto& attachmentSpec : attachmentSpecs) {
            if (attachmentSpec.m_attachmentType == Core::FramebufferAttachmentType::Color) {
                m_colorAttachmentSpecs.emplace_back(attachmentSpec);
            } else if (attachmentSpec.m_attachmentType == Core::FramebufferAttachmentType::Depth) {
                m_depthAttachmentSpec = attachmentSpec;
            }
        }

        recreate();
    }

    Framebuffer::~Framebuffer() {
        if (m_rendererId) {
            Core::RenderCommand::deleteFramebuffer(m_rendererId);
            for (auto& texture : m_textures) {
                unsigned int textureRendererId = texture->getRendererId();
                Core::RenderCommand::deleteTexture(textureRendererId);
            }
            m_textures.clear();
        }
    }

    void Framebuffer::bind() {

        Core::RenderCommand::getViewport(m_lastViewportX0, m_lastViewportY0, m_lastViewportX1, m_lastViewportY1);
        Core::RenderCommand::bindFramebuffer(m_rendererId);
        Core::RenderCommand::setViewport(0, 0, m_width, m_height);
    }

    void Framebuffer::unbind() {
        Core::RenderCommand::unbindFramebuffer();
        Core::RenderCommand::setViewport(m_lastViewportX0, m_lastViewportY0, m_lastViewportX1, m_lastViewportY1);
    }

    void Framebuffer::clear() {

        for (unsigned int i = 0; i < m_colorAttachmentSpecs.size(); i++) {
            clearColorAttachment(i);
        }

        clearDepthAttachment();

    }

    void Framebuffer::clearColorAttachment(unsigned int index) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[index];

        switch (colorAttachmentSpec.m_dataFormat) {
            case FramebufferDataFormat::RGBA:
                Core::RenderCommand::clearColorAttachment(m_rendererId, index, Core::PrimitiveDataType::Float, glm::value_ptr(colorAttachmentSpec.m_clearColorVec4Value));
                break;
            case FramebufferDataFormat::Integer:
                Core::RenderCommand::clearColorAttachment(m_rendererId, index, Core::PrimitiveDataType::Int, &colorAttachmentSpec.m_clearColorIntValue);
                break;
            default:
                throw std::runtime_error("Unsupported attachment data type for clear color");
        }

    }

    void Framebuffer::clearDepthAttachment() {
        if (m_depthAttachmentSpec.m_attachmentType != Core::FramebufferAttachmentType::None) {
            Core::RenderCommand::clearDepthAttachment(m_rendererId);
        }
    }

    void Framebuffer::resize(unsigned int width, unsigned int height) {
        m_width = width;
        m_height = height;
        recreate();
        Core::RenderCommand::setViewport(0, 0, m_width, m_height);
    }

    void Framebuffer::setDrawBuffers(const std::vector<int>& drawBuffers) {

        Core::RenderCommand::setDrawBuffers(m_rendererId, drawBuffers);

    }

    int Framebuffer::readPixel(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];

        if (colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::Integer) {
            throw std::runtime_error("Color attachment doesn't have INT data");
        }

        int data;
        Core::RenderCommand::readFramebuffer(
            m_rendererId,
            Core::FramebufferAttachmentType::Color, colorAttachmentIndex,
            Core::TextureDataFormat::RedInteger, Core::PrimitiveDataType::Int,
            x, y, 1, 1,
            &data
        );

        return data;

    }

    void Framebuffer::recreate() {

        if (m_rendererId) {
            Core::RenderCommand::deleteFramebuffer(m_rendererId);
            for (auto& texture : m_textures) {
                unsigned int textureRendererId = texture->getRendererId();
                Core::RenderCommand::deleteTexture(textureRendererId);
            }
            m_textures.clear();
        }

        // Create the framebuffer and bind it
        Core::RenderCommand::createFramebuffer(m_rendererId);
        Core::RenderCommand::bindFramebuffer(m_rendererId);

        std::vector<int> drawBuffers = {};

        // Create and attach the color textures
        for (unsigned int i = 0; i < m_colorAttachmentSpecs.size(); i++) {

            auto& colorAttachmentSpec = m_colorAttachmentSpecs[i];
            attachColor(colorAttachmentSpec, i);
            drawBuffers.emplace_back(i);

        }

        // Create and attach the depth texture, if any
        if (m_depthAttachmentSpec.m_attachmentType != Core::FramebufferAttachmentType::None) {

            attachDepth(m_depthAttachmentSpec);
            drawBuffers.emplace_back(-1);

        }

        setDrawBuffers(drawBuffers);

        // Check correctness and unbind
        Core::RenderCommand::checkFramebufferCompleteness(m_rendererId);
        Core::RenderCommand::unbindFramebuffer();

    }

    void Framebuffer::attachColor(FramebufferAttachmentSpec colorAttachmentSpec, unsigned int index) {

        Core::TextureDataFormat internalFormat;
        Core::TextureDataFormat format;

        switch (colorAttachmentSpec.m_dataFormat) {
            case FramebufferDataFormat::RGBA:
                internalFormat = Core::TextureDataFormat::RGBA;
                format = Core::TextureDataFormat::RGBA;
                break;
            case FramebufferDataFormat::Integer:
                internalFormat = Core::TextureDataFormat::RedInteger32;
                format = Core::TextureDataFormat::RedInteger;
                break;
            case FramebufferDataFormat::Depth:
                throw std::runtime_error("Invalid framebuffer data format");
        }

        Core::Ref<Texture> texture = Core::CreateRef<TextureImage2D>(
            internalFormat, format,
            m_width, m_height,
            nullptr
        );
        m_textures.push_back(texture);

        // Attach the texture to the framebuffer
        Core::RenderCommand::framebufferAttach(
            m_rendererId,
            Core::TextureType::Image2D,
            colorAttachmentSpec.m_attachmentType,
            index,
            texture->getRendererId()
        );

    }

    void Framebuffer::attachDepth(FramebufferAttachmentSpec depthAttachmentSpec) {

        // Create the buffer texture
        Core::Ref<Texture> texture = Core::CreateRef<TextureImage2D>(
            Core::TextureDataFormat::Depth, Core::TextureDataFormat::Depth,
            m_width, m_height,
            nullptr
        );
        m_textures.push_back(texture);

        // Attach the texture to the framebuffer
        Core::RenderCommand::framebufferAttach(
            m_rendererId,
            Core::TextureType::Image2D,
            m_depthAttachmentSpec.m_attachmentType,
            0,
            texture->getRendererId()
        );

    }

}