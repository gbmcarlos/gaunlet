#pragma once

#include "gaunlet/graphics/texture/Texture.h"

#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

    class TextureLibrary {

    public:

        TextureLibrary() = default;

        unsigned int add(const Core::Ref<Texture>& texture);
        unsigned int addOrGet(const Core::Ref<Texture>& texture);
        bool exists(const Core::Ref<Texture>& texture);
        Core::Ref<Texture> get(unsigned int index);
        inline const std::vector<Core::Ref<Texture>>& getAll() {return m_textures; }
        unsigned int count();
        void clear();

    private:
        std::vector<Core::Ref<Texture>> m_textures;

    };

}