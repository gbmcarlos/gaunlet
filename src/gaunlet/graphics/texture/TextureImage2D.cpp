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

    TextureImage2D::~TextureImage2D() {
        Core::RenderCommand::deleteTexture(m_rendererId);
    }

    void TextureImage2D::activate(unsigned int slot) {
        Core::RenderCommand::activateTexture(m_rendererId, Core::TextureType::Image2D, slot);
    }

}