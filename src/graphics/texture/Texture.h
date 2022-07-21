#pragma once

#include <string>

namespace engine {

    class Texture {

    public:
        Texture(const std::string& path);
        Texture(unsigned int width, unsigned int height, void* data);
        ~Texture();

        void bind(unsigned int slot = 0);

    private:
        unsigned int m_rendererId;

        unsigned int m_width;
        unsigned int m_height;

    };

}