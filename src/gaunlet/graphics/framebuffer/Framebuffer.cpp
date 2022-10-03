#include "gaunlet/graphics/framebuffer/Framebuffer.h"

#include "gaunlet/core/render/RenderCommand.h"
#include "gaunlet/graphics/texture/TextureImage2D.h"

namespace gaunlet::Graphics {

    FramebufferAttachmentSpec::FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, int clearColorValue)
        : m_attachmentType(attachmentType), m_dataFormat(framebufferDataFormat), m_clearColorIntValue(clearColorValue) {

        if (
                framebufferDataFormat != FramebufferDataFormat::R_Color
            &&  framebufferDataFormat != FramebufferDataFormat::R_Integer
            ) {
            throw std::runtime_error("Clear color type doesn't match the data type");
        }

    }

    FramebufferAttachmentSpec::FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, float clearColorValue)
        : m_attachmentType(attachmentType), m_dataFormat(framebufferDataFormat), m_clearColorFloatValue(clearColorValue) {

        if (
                framebufferDataFormat != FramebufferDataFormat::R_HDR
            &&  framebufferDataFormat != FramebufferDataFormat::R_Float
            ) {
            throw std::runtime_error("Clear color type doesn't match the data type");
        }

    }

    FramebufferAttachmentSpec::FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, const glm::vec2& clearColorValue)
        : m_attachmentType(attachmentType), m_dataFormat(framebufferDataFormat), m_clearColorVec2Value(clearColorValue) {

        if (
                framebufferDataFormat != FramebufferDataFormat::RG_Color
            &&  framebufferDataFormat != FramebufferDataFormat::RG_HDR
            &&  framebufferDataFormat != FramebufferDataFormat::RG_Integer
            &&  framebufferDataFormat != FramebufferDataFormat::RG_Float
            ) {
            throw std::runtime_error("Clear color type doesn't match the data type");
        }

    }

    FramebufferAttachmentSpec::FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, const glm::vec3& clearColorValue)
        : m_attachmentType(attachmentType), m_dataFormat(framebufferDataFormat), m_clearColorVec3Value(clearColorValue) {

        if (
                framebufferDataFormat != FramebufferDataFormat::RGB_Color
            &&  framebufferDataFormat != FramebufferDataFormat::RGB_HDR
            &&  framebufferDataFormat != FramebufferDataFormat::RGB_Integer
            &&  framebufferDataFormat != FramebufferDataFormat::RGB_Float
            ) {
            throw std::runtime_error("Clear color type doesn't match the data type");
        }

    }

    FramebufferAttachmentSpec::FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, const glm::vec4& clearColorValue)
        : m_attachmentType(attachmentType), m_dataFormat(framebufferDataFormat), m_clearColorVec4Value(clearColorValue) {

        if (
                framebufferDataFormat != FramebufferDataFormat::RGBA_Color
            &&  framebufferDataFormat != FramebufferDataFormat::RGBA_HDR
            &&  framebufferDataFormat != FramebufferDataFormat::RGBA_Integer
            &&  framebufferDataFormat != FramebufferDataFormat::RGBA_Float
            ) {
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
            } else if (attachmentSpec.m_attachmentType == Core::FramebufferAttachmentType::DepthStencil) {
                m_depthStencilAttachmentSpec = attachmentSpec;
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

        clearDepthStencilAttachment();

    }

    void Framebuffer::clearColorAttachment(unsigned int index) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[index];

        switch (colorAttachmentSpec.m_dataFormat) {
            // Red
                // Int
                case FramebufferDataFormat::R_Color:
                case FramebufferDataFormat::R_Integer:
                    Core::RenderCommand::clearColorAttachment(m_rendererId, index, Core::PrimitiveDataType::Int, &colorAttachmentSpec.m_clearColorIntValue);
                    break;
                // Float
                case FramebufferDataFormat::R_HDR:
                case FramebufferDataFormat::R_Float:
                    Core::RenderCommand::clearColorAttachment(m_rendererId, index, Core::PrimitiveDataType::Float, &colorAttachmentSpec.m_clearColorFloatValue);
                    break;
            // RG
                // Int
                case FramebufferDataFormat::RG_Color:
                case FramebufferDataFormat::RG_Integer:
                    Core::RenderCommand::clearColorAttachment(m_rendererId, index, Core::PrimitiveDataType::Int, &colorAttachmentSpec.m_clearColorVec2Value);
                    break;
                // Float
                case FramebufferDataFormat::RG_HDR:
                case FramebufferDataFormat::RG_Float:
                    Core::RenderCommand::clearColorAttachment(m_rendererId, index, Core::PrimitiveDataType::Float, &colorAttachmentSpec.m_clearColorVec2Value);
                    break;
            // RGB
                // Int
                case FramebufferDataFormat::RGB_Color:
                case FramebufferDataFormat::RGB_Integer:
                    Core::RenderCommand::clearColorAttachment(m_rendererId, index, Core::PrimitiveDataType::Int, &colorAttachmentSpec.m_clearColorVec2Value);
                    break;
                // Float
                case FramebufferDataFormat::RGB_HDR:
                case FramebufferDataFormat::RGB_Float:
                    Core::RenderCommand::clearColorAttachment(m_rendererId, index, Core::PrimitiveDataType::Float, &colorAttachmentSpec.m_clearColorVec2Value);
                    break;
            // RGBA
                // Int
                case FramebufferDataFormat::RGBA_Color:
                case FramebufferDataFormat::RGBA_Integer:
                    Core::RenderCommand::clearColorAttachment(m_rendererId, index, Core::PrimitiveDataType::Int, &colorAttachmentSpec.m_clearColorVec2Value);
                    break;
                // Float
                case FramebufferDataFormat::RGBA_HDR:
                case FramebufferDataFormat::RGBA_Float:
                    Core::RenderCommand::clearColorAttachment(m_rendererId, index, Core::PrimitiveDataType::Float, &colorAttachmentSpec.m_clearColorVec2Value);
                    break;
            default:
                throw std::runtime_error("Unsupported attachment data type for clear color");
        }

    }

    void Framebuffer::clearDepthStencilAttachment() {
        if (m_depthStencilAttachmentSpec.m_attachmentType != Core::FramebufferAttachmentType::None) {
            // Make sure to clear the whole stencil buffer
            Core::RenderCommand::setStencilOperation(true, Core::StencilOperation::Replace, Core::StencilOperation::Replace, Core::StencilOperation::Replace);
            Core::RenderCommand::clearDepthStencilAttachment(m_rendererId, 1.0f, 0);
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

    void Framebuffer::deAttachColor(unsigned int colorAttachmentIndex) {

        auto colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];

        Core::RenderCommand::framebufferAttach(
            m_rendererId,
            Core::TextureType::Image2D,
            colorAttachmentSpec.m_attachmentType,
            colorAttachmentIndex,
            0
        );

        m_colorAttachmentSpecs.erase(m_colorAttachmentSpecs.begin() + colorAttachmentIndex);
        m_textures.erase(m_textures.begin() + colorAttachmentIndex);

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

        // Create and attach the depth-stencil texture, if any
        if (m_depthStencilAttachmentSpec.m_attachmentType != Core::FramebufferAttachmentType::None) {
            attachDepthStencil(m_depthStencilAttachmentSpec);
            drawBuffers.emplace_back(-1);
        }

        setDrawBuffers(drawBuffers);

        // Check correctness and unbind
        Core::RenderCommand::checkFramebufferCompleteness(m_rendererId);
        Core::RenderCommand::unbindFramebuffer();

    }

    void Framebuffer::attachColor(FramebufferAttachmentSpec colorAttachmentSpec, unsigned int index) {

        auto [internalFormat, dataFormat] = convertFramebufferFormat(colorAttachmentSpec.m_dataFormat);

        Core::Ref<Texture> texture = Core::CreateRef<TextureImage2D>(
            internalFormat, dataFormat, Core::PrimitiveDataType::UByte,
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

    void Framebuffer::attachDepthStencil(FramebufferAttachmentSpec depthStencilAttachmentSpec) {

        // Create the buffer texture
        Core::Ref<Texture> texture = Core::CreateRef<TextureImage2D>(
            Core::TextureInternalFormat::Depth_24_Stencil_8, Core::TextureExternalFormat::DepthStencil, Core::PrimitiveDataType::UInt24_8,
            m_width, m_height,
            nullptr
        );
        m_textures.push_back(texture);

        // Attach the texture to the framebuffer
        Core::RenderCommand::framebufferAttach(
            m_rendererId,
            Core::TextureType::Image2D,
            m_depthStencilAttachmentSpec.m_attachmentType,
            0,
            texture->getRendererId()
        );

    }

    std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat> Framebuffer::convertFramebufferFormat(FramebufferDataFormat format) {

        switch (format) {
            // Color
                case FramebufferDataFormat::R_Color:        return {Core::TextureInternalFormat::Red_8_UNI, Core::TextureExternalFormat::Red};
                case FramebufferDataFormat::RG_Color:       return {Core::TextureInternalFormat::RG_8_UNI, Core::TextureExternalFormat::RG};
                case FramebufferDataFormat::RGB_Color:      return {Core::TextureInternalFormat::RGB_8_UNI, Core::TextureExternalFormat::RGB};
                case FramebufferDataFormat::RGBA_Color:     return {Core::TextureInternalFormat::RGBA_8_UNI, Core::TextureExternalFormat::RGBA};
            // HDR
                case FramebufferDataFormat::R_HDR:          return {Core::TextureInternalFormat::Red_16_F, Core::TextureExternalFormat::Red};
                case FramebufferDataFormat::RG_HDR:         return {Core::TextureInternalFormat::RG_16_F, Core::TextureExternalFormat::RG};
                case FramebufferDataFormat::RGB_HDR:        return {Core::TextureInternalFormat::RGB_16_F, Core::TextureExternalFormat::RGB};
                case FramebufferDataFormat::RGBA_HDR:       return {Core::TextureInternalFormat::RGBA_16_F, Core::TextureExternalFormat::RGBA};
            // Signed Integer
                case FramebufferDataFormat::R_Integer:      return {Core::TextureInternalFormat::Red_32_SI, Core::TextureExternalFormat::RedInteger};
                case FramebufferDataFormat::RG_Integer:     return {Core::TextureInternalFormat::RG_32_SI, Core::TextureExternalFormat::RGInteger};
                case FramebufferDataFormat::RGB_Integer:    return {Core::TextureInternalFormat::RGB_32_SI, Core::TextureExternalFormat::RGBInteger};
                case FramebufferDataFormat::RGBA_Integer:   return {Core::TextureInternalFormat::RGBA_32_SI, Core::TextureExternalFormat::RGBAInteger};
            // Float
                case FramebufferDataFormat::R_Float:        return {Core::TextureInternalFormat::Red_32_F, Core::TextureExternalFormat::Red};
                case FramebufferDataFormat::RG_Float:       return {Core::TextureInternalFormat::RG_32_F, Core::TextureExternalFormat::RG};
                case FramebufferDataFormat::RGB_Float:      return {Core::TextureInternalFormat::RGB_32_F, Core::TextureExternalFormat::RGB};
                case FramebufferDataFormat::RGBA_Float:     return {Core::TextureInternalFormat::RGBA_32_F, Core::TextureExternalFormat::RGBA};
            // Depth & Stencil
                case FramebufferDataFormat::DepthStencil:   return {Core::TextureInternalFormat::Depth_24_Stencil_8, Core::TextureExternalFormat::DepthStencil};
        }

    }

    template<>
    int Framebuffer::readPixel<int>(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];

        if (
                colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::R_Color
            &&  colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::R_Integer
            ) {
            throw std::runtime_error("Color attachment doesn't have INT data");
        }

        int data;
        Core::RenderCommand::readFramebuffer(
            m_rendererId,
            Core::FramebufferAttachmentType::Color, colorAttachmentIndex,
            Core::TextureExternalFormat::RedInteger, Core::PrimitiveDataType::Int,
            x, y, 1, 1,
            &data
        );

        return data;

    }

    template<>
    float Framebuffer::readPixel<float>(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];

        if (
                colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::R_HDR
            &&  colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::R_Float
            ) {
            throw std::runtime_error("Color attachment doesn't have float data");
        }

        float data;
        Core::RenderCommand::readFramebuffer(
            m_rendererId,
            Core::FramebufferAttachmentType::Color, colorAttachmentIndex,
            Core::TextureExternalFormat::Red, Core::PrimitiveDataType::Float,
            x, y, 1, 1,
            &data
        );

        return data;

    }

    template<>
    glm::vec2 Framebuffer::readPixel<glm::vec2>(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];

        if (
                colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::RG_HDR
            &&  colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::RG_Float
            ) {
            throw std::runtime_error("Color attachment doesn't have vec2 data");
        }

        glm::vec2 data;
        Core::RenderCommand::readFramebuffer(
            m_rendererId,
            Core::FramebufferAttachmentType::Color, colorAttachmentIndex,
            Core::TextureExternalFormat::RG, Core::PrimitiveDataType::Float,
            x, y, 1, 1,
            &data
        );

        return data;

    }

    template<>
    glm::vec3 Framebuffer::readPixel<glm::vec3>(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];

        if (
                colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::RGB_HDR
            &&  colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::RGB_Float
            ) {
            throw std::runtime_error("Color attachment doesn't have vec3 data");
        }

        glm::vec3 data;
        Core::RenderCommand::readFramebuffer(
            m_rendererId,
            Core::FramebufferAttachmentType::Color, colorAttachmentIndex,
            Core::TextureExternalFormat::RGB, Core::PrimitiveDataType::Float,
            x, y, 1, 1,
            &data
        );

        return data;

    }

    template<>
    glm::vec4 Framebuffer::readPixel<glm::vec4>(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];

        if (
                colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::RGBA_HDR
            &&  colorAttachmentSpec.m_dataFormat != FramebufferDataFormat::RGBA_Float
            ) {
            throw std::runtime_error("Color attachment doesn't have vec4 data");
        }

        glm::vec4 data;
        Core::RenderCommand::readFramebuffer(
            m_rendererId,
            Core::FramebufferAttachmentType::Color, colorAttachmentIndex,
            Core::TextureExternalFormat::RGBA, Core::PrimitiveDataType::Float,
            x, y, 1, 1,
            &data
        );

        return data;

    }

}