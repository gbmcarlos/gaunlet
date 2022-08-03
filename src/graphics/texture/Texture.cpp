#include "Texture.h"

#include "../../core/render/RenderCommand.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace engine {

    Texture::Texture(const std::string &path) {

        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!data) {
            throw std::runtime_error("Failed to load texture file");
        }

        TextureFormat internalFormat;
        TextureFormat dataFormat;

        if (channels == 3) {
            internalFormat = TextureFormat::RGB;
            dataFormat = TextureFormat::RGB;
        } else if (channels == 4) {
            internalFormat = TextureFormat::RGBA;
            dataFormat = TextureFormat::RGBA;
        } else {
            throw std::runtime_error("Unknown texture data format");
        }

        RenderCommand::loadTexture(m_rendererId, internalFormat, dataFormat,  (unsigned int) width, (unsigned int) height, data);

        stbi_image_free(data);

        m_width = width;
        m_height = height;

    }

    Texture::Texture(TextureFormat internalFormat, TextureFormat dataFormat, unsigned int width, unsigned int height, void* data) {

        RenderCommand::loadTexture(m_rendererId, internalFormat, dataFormat, (unsigned int) width, (unsigned int) height, data);

        m_width = width;
        m_height = height;

    }

    Texture::~Texture() {
        RenderCommand::deleteTexture(m_rendererId);
    }

    void Texture::bind(unsigned int slot) {
        RenderCommand::bindTexture(m_rendererId, slot);
    }

}