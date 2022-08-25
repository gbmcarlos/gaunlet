#include "graphics-engine/graphics/texture/TextureImage2D.h"

#include "graphics-engine/core/render/RenderCommand.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace engine::Graphics {

    TextureImage2D::TextureImage2D(const std::string &path) {

        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!data) {
            throw std::runtime_error("Failed to load texture file");
        }

        Core::TextureDataFormat internalFormat;
        Core::TextureDataFormat dataFormat;

        if (channels == 3) {
            internalFormat = Core::TextureDataFormat::RGB;
            dataFormat = Core::TextureDataFormat::RGB;
        } else if (channels == 4) {
            internalFormat = Core::TextureDataFormat::RGBA;
            dataFormat = Core::TextureDataFormat::RGBA;
        } else {
            throw std::runtime_error("Unknown texture data format");
        }

        Core::RenderCommand::loadTexture(m_rendererId, Core::TextureType::Image2D, internalFormat, dataFormat,  (unsigned int) width, (unsigned int) height, data);

        stbi_image_free(data);

        m_width = width;
        m_height = height;

    }

    TextureImage2D::TextureImage2D(Core::TextureDataFormat internalFormat, Core::TextureDataFormat dataFormat, unsigned int width, unsigned int height, void* data) {

        Core::RenderCommand::loadTexture(m_rendererId, Core::TextureType::Image2D, internalFormat, dataFormat, (unsigned int) width, (unsigned int) height, data);

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