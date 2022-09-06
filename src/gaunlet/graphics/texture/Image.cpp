#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace gaunlet::Graphics {

    Image::Image(const char *path, bool flipVertically) {

        stbi_set_flip_vertically_on_load(flipVertically);
        m_data = (void*)stbi_load(path, &m_width, &m_height, &m_channels, 0);

        if (!m_data) {
            throw std::runtime_error("Failed to load texture file");
        }

    }

    Image::~Image() {
        stbi_image_free((stbi_uc*)m_data);
    }

}