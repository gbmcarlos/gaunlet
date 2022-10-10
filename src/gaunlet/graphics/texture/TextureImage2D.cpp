#include "gaunlet/graphics/texture/TextureImage2D.h"

#include "gaunlet/core/render/RenderCommand.h"

namespace gaunlet::Graphics {

    TextureImage2D::TextureImage2D(const std::string &path) {

        Image image(path.c_str(), true);

        Core::RenderCommand::loadTextureImage2d(
            m_rendererId,
            convertToInternalFormat(image.getChannels()), convertToExternalFormat(image.getChannels()), Core::PrimitiveDataType::UByte,
            (unsigned int) image.getWidth(), (unsigned int) image.getHeight(),
            image.getData()
        );

        m_format = convertToExternalFormat(image.getChannels());
        m_width = image.getWidth();
        m_height = image.getHeight();

    }

    TextureImage2D::TextureImage2D(Core::TextureInternalFormat internalFormat, Core::TextureExternalFormat dataFormat, Core::PrimitiveDataType dataType, unsigned int width, unsigned int height, void* data) {

        Core::RenderCommand::loadTextureImage2d(
            m_rendererId,
            internalFormat, dataFormat, dataType,
            (unsigned int) width, (unsigned int) height, data
        );

        m_format = dataFormat;
        m_width = width;
        m_height = height;

    }

    TextureImage2D::TextureImage2D(const std::string &path, Core::TextureFilteringParameterValue filteringParameterValue, Core::TextureWrappingParameterValue wrappingParameterValue)
        : TextureImage2D(path) {
        setFilteringParameters(filteringParameterValue);
        setWrappingParameters(wrappingParameterValue);
    }

    TextureImage2D::~TextureImage2D() {
        Core::RenderCommand::deleteTexture(m_rendererId);
    }

    void TextureImage2D::activate(unsigned int slot) {
        Core::RenderCommand::activateTexture(m_rendererId, Core::TextureType::Image2D, slot);
    }

    void TextureImage2D::setFilteringParameters(Core::TextureFilteringParameterValue value) {
        setFilteringParameters(value, value);
    }

    void TextureImage2D::setFilteringParameters(Core::TextureFilteringParameterValue minifyingValue, Core::TextureFilteringParameterValue magnifyingValue) {

        Core::RenderCommand::setTexturedFilteringParameter(
            m_rendererId, Core::TextureType::Image2D, Core::TextureFilteringParameter::Minifying,
            minifyingValue
        );

        Core::RenderCommand::setTexturedFilteringParameter(
            m_rendererId, Core::TextureType::Image2D, Core::TextureFilteringParameter::Magnifying,
            magnifyingValue
        );

    }

    void TextureImage2D::setWrappingParameters(Core::TextureWrappingParameterValue value) {
        setWrappingParameters(value, value);
    }

    void TextureImage2D::setWrappingParameters(Core::TextureWrappingParameterValue sValue, Core::TextureWrappingParameterValue tValue) {

        Core::RenderCommand::setTexturedWrappingParameter(
            m_rendererId, Core::TextureType::Image2D, Core::TextureWrappingParameter::S,
            sValue
        );

        Core::RenderCommand::setTexturedWrappingParameter(
            m_rendererId, Core::TextureType::Image2D, Core::TextureWrappingParameter::T,
            tValue
        );

    }

}