#include "gaunlet/graphics/framebuffer/Framebuffer.h"

#include "gaunlet/core/render/RenderCommand.h"
#include "gaunlet/graphics/texture/TextureImage2D.h"
#include "gaunlet/graphics/framebuffer/attachment-specs/BaseDepthStencilAttachmentSpec.h"

namespace gaunlet::Graphics {

    Framebuffer::Framebuffer(unsigned int width, unsigned int height)
        : m_width(width), m_height(height) {
        Core::RenderCommand::createFramebuffer(m_rendererId);
    }

    Framebuffer::~Framebuffer() {
        destroyFramebuffer();
    }

    const Core::Ref<TextureImage2D>& Framebuffer::addColorAttachment(const Core::Ref<ColorAttachmentSpec>& colorAttachmentSpec) {

        unsigned int attachmentIndex = m_colorAttachmentSpecs.size();
        m_colorAttachmentSpecs.emplace_back(colorAttachmentSpec);
        return createColorAttachment(colorAttachmentSpec, attachmentIndex);

    }

    const Core::Ref<TextureImage2D>& Framebuffer::setDepthStencilAttachment(float depthClearValue, int stencilClearValue) {

        m_depthStencilAttachmentSpec = BaseDepthStencilAttachmentSpec::create(depthClearValue, stencilClearValue);
        return createDepthStencilAttachment(m_depthStencilAttachmentSpec);

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

        Core::RenderCommand::clearColorAttachment(
            m_rendererId, index,
            colorAttachmentSpec->getClearDataType(),
            colorAttachmentSpec->getClearValue()
        );

    }

    void Framebuffer::clearDepthStencilAttachment() {

        if (m_depthStencilAttachmentSpec) {
            // Make sure to clear the whole stencil buffer
            Core::RenderCommand::setStencilOperation(true, Core::StencilOperation::Replace, Core::StencilOperation::Replace, Core::StencilOperation::Replace);
            Core::RenderCommand::clearDepthStencilAttachment(
                m_rendererId,
                m_depthStencilAttachmentSpec->getDepthClearValue(),
                m_depthStencilAttachmentSpec->getStencilClearValue()
            );
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
            Core::FramebufferAttachmentType::Color,
            colorAttachmentIndex,
            0
        );

        m_colorAttachmentSpecs.erase(m_colorAttachmentSpecs.begin() + colorAttachmentIndex);
        m_textures.erase(m_textures.begin() + colorAttachmentIndex);

    }

    void Framebuffer::recreate() {

        if (m_rendererId) {
            destroyFramebuffer();
            Core::RenderCommand::createFramebuffer(m_rendererId);
        }

        std::vector<int> drawBuffers = {};

        // Create and attach the color textures
        for (unsigned int i = 0; i < m_colorAttachmentSpecs.size(); i++) {

            auto& colorAttachmentSpec = m_colorAttachmentSpecs[i];
            createColorAttachment(colorAttachmentSpec, i);
            drawBuffers.emplace_back(i);

        }

        // Create and attach the depth-stencil texture, if any
        if (m_depthStencilAttachmentSpec) {
            createDepthStencilAttachment(m_depthStencilAttachmentSpec);
            drawBuffers.emplace_back(-1);
        }

        setDrawBuffers(drawBuffers);

        // Check correctness and unbind
        Core::RenderCommand::checkFramebufferCompleteness(m_rendererId);
        Core::RenderCommand::unbindFramebuffer();

    }

    void Framebuffer::destroyFramebuffer() {

        // Delete the framebuffer
        Core::RenderCommand::deleteFramebuffer(m_rendererId);
        // Clear the list of attachment textures (each texture will delete itself)
        m_textures.clear();

    }

    const Core::Ref<TextureImage2D>& Framebuffer::createColorAttachment(const Core::Ref<ColorAttachmentSpec>& colorAttachmentSpec, unsigned int index) {

        auto [internalFormat, externalFormat, dataType] = colorAttachmentSpec->getTextureParameters();

        Core::Ref<TextureImage2D> texture = Core::CreateRef<TextureImage2D>(
            internalFormat, externalFormat, dataType,
            m_width, m_height,
            nullptr
        );
        texture->setFilteringParameters(gaunlet::Core::TextureFilteringParameterValue::Linear);
        texture->setWrappingParameters(gaunlet::Core::TextureWrappingParameterValue::Repeat);
        m_textures.push_back(texture);

        // Attach the texture to the framebuffer
        Core::RenderCommand::framebufferAttach(
            m_rendererId,
            Core::TextureType::Image2D,
            Core::FramebufferAttachmentType::Color,
            index,
            texture->getRendererId()
        );

        return m_textures.back();

    }

    const Core::Ref<TextureImage2D>& Framebuffer::createDepthStencilAttachment(const Core::Ref<DepthStencilAttachmentSpec>& depthStencilAttachmentSpec) {

        auto [internalFormat, externalFormat, dataType] = depthStencilAttachmentSpec->getTextureParameters();

        // Create the buffer texture
        Core::Ref<TextureImage2D> texture = Core::CreateRef<TextureImage2D>(
            internalFormat, externalFormat, dataType,
            m_width, m_height,
            nullptr
        );
        m_textures.push_back(texture);

        // Attach the texture to the framebuffer
        Core::RenderCommand::framebufferAttach(
            m_rendererId,
            Core::TextureType::Image2D,
            Core::FramebufferAttachmentType::DepthStencil,
            0,
            texture->getRendererId()
        );

        return m_textures.back();

    }

    template<>
    int Framebuffer::readPixel<int>(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];
        auto [externalFormat, dataType] = colorAttachmentSpec->getReadIntParameters();

        int data;
        Core::RenderCommand::readFramebuffer(
            m_rendererId,
            Core::FramebufferAttachmentType::Color, colorAttachmentIndex,
            externalFormat, dataType,
            x, y, 1, 1,
            &data
        );

        return data;

    }

    template<>
    float Framebuffer::readPixel<float>(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];
        auto [externalFormat, dataType] = colorAttachmentSpec->getReadFloatParameters();

        float data;
        Core::RenderCommand::readFramebuffer(
            m_rendererId,
            Core::FramebufferAttachmentType::Color, colorAttachmentIndex,
            externalFormat, dataType,
            x, y, 1, 1,
            &data
        );

        return data;

    }

    template<>
    glm::vec2 Framebuffer::readPixel<glm::vec2>(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];
        auto [externalFormat, dataType] = colorAttachmentSpec->getReadVec2Parameters();

        glm::vec2 data;
        Core::RenderCommand::readFramebuffer(
            m_rendererId,
            Core::FramebufferAttachmentType::Color, colorAttachmentIndex,
            externalFormat, dataType,
            x, y, 1, 1,
            &data
        );

        return data;

    }

    template<>
    glm::vec3 Framebuffer::readPixel<glm::vec3>(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];
        auto [externalFormat, dataType] = colorAttachmentSpec->getReadVec3Parameters();

        glm::vec3 data;
        Core::RenderCommand::readFramebuffer(
            m_rendererId,
            Core::FramebufferAttachmentType::Color, colorAttachmentIndex,
            externalFormat, dataType,
            x, y, 1, 1,
            &data
        );

        return data;

    }

    template<>
    glm::vec4 Framebuffer::readPixel<glm::vec4>(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y) {

        auto& colorAttachmentSpec = m_colorAttachmentSpecs[colorAttachmentIndex];
        auto [externalFormat, dataType] = colorAttachmentSpec->getReadVec4Parameters();

        glm::vec4 data;
        Core::RenderCommand::readFramebuffer(
            m_rendererId,
            Core::FramebufferAttachmentType::Color, colorAttachmentIndex,
            externalFormat, dataType,
            x, y, 1, 1,
            &data
        );

        return data;

    }

}