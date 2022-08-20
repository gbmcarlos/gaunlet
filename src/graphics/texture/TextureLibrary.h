#pragma once

#include "Texture.h"

#include "../pch.h"

namespace engine {

    class TextureLibrary {

    public:

        TextureLibrary() = default;

        unsigned int add(const Ref<Texture>& texture);
        unsigned int addOrGet(const Ref<Texture>& texture);
        bool exists(const Ref<Texture>& texture);
        Ref<Texture> get(unsigned int index);
        inline const std::vector<Ref<Texture>>& getAll() {return m_textures; }
        unsigned int count();
        void clear();

    private:
        std::vector<Ref<Texture>> m_textures;

    };

}