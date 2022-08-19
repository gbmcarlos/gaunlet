#pragma once

#include "Texture.h"

#include "../pch.h"

namespace engine {

    class TextureLibrary {

    public:

        TextureLibrary() = default;

        unsigned int add(const std::shared_ptr<Texture>& texture);
        unsigned int addOrGet(const std::shared_ptr<Texture>& texture);
        bool exists(const std::shared_ptr<Texture>& texture);
        std::shared_ptr<Texture> get(unsigned int index);
        inline const std::vector<std::shared_ptr<Texture>>& getAll() {return m_textures; }
        unsigned int count();
        void clear();

    private:
        std::vector<std::shared_ptr<Texture>> m_textures;

    };

}