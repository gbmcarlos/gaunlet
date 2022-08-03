#pragma once

#include "../../core/render/RenderCommand.h"
#include <string>

namespace engine {

    class Texture {

        enum class Format {
            RGB, RGBA
        };

    public:
        Texture(const std::string& path);
        Texture(TextureFormat internalFormat, TextureFormat dataFormat, unsigned int width, unsigned int height, void* data);
        ~Texture();

        void bind(unsigned int slot = 0);

    private:
        unsigned int m_rendererId;

        unsigned int m_width;
        unsigned int m_height;

    };

}