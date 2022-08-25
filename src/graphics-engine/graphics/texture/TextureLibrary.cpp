#include "graphics-engine/graphics/texture/TextureLibrary.h"

namespace engine::Graphics {

    unsigned int TextureLibrary::add(const Core::Ref<Texture>& texture) {

        if (exists(texture)) {
            throw std::runtime_error("Texture already exists in the library");
        }

        unsigned int index = m_textures.size();

        m_textures.emplace_back(texture);

        return index;

    }

    unsigned int TextureLibrary::addOrGet(const Core::Ref<Texture> &texture) {

        auto iterator = std::find(m_textures.begin(), m_textures.end(), texture);

        if (iterator == m_textures.end()) {
            return add(texture);
        } else {
            return (unsigned int) std::distance(m_textures.begin(), iterator);
        }

    }

    bool TextureLibrary::exists(const Core::Ref<Texture>& texture) {

        auto iterator = std::find(m_textures.begin(), m_textures.end(), texture);

        return iterator != m_textures.end();

    }

    Core::Ref<Texture> TextureLibrary::get(unsigned int index) {

        if (index >= m_textures.size()) {
            throw std::runtime_error("Out of bounds texture");
        }

        return m_textures[index];

    }

    unsigned int TextureLibrary::count() {
        return m_textures.size();
    }

    void TextureLibrary::clear() {
        m_textures.clear();
    }

}