#include "Texture.h"

#include "../../core/render/RenderCommand.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace engine {

    Texture::Texture(const std::string &path) {

        int width, height, channels;
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!data) {
            throw std::runtime_error("Failed to load texture file");
        }

        RenderCommand::loadTexture(m_rendererId, (unsigned int) width, (unsigned int) height, data);

        stbi_image_free(data);

        m_width = width;
        m_height = height;

    }

    Texture::Texture(unsigned int width, unsigned int height, void* data) {

        RenderCommand::loadTexture(m_rendererId, (unsigned int) width, (unsigned int) height, data);

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