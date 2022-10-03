#include "TextureCubeMap.h"

#include "gaunlet/core/render/RenderCommand.h"

#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

    TextureCubeMap::TextureCubeMap(const std::vector<const char *>& filePaths) {

        // Load the images
        std::vector<Core::Scope<Image>> images;
        for (auto& path : filePaths) {
            images.emplace_back(Core::CreateScope<Image>(path, false));
        }

        m_format = convertToExternalFormat(images[0]->getChannels());

        // Prepare the properties
        std::vector<void*> imagesData;
        for (auto& image : images) {
            imagesData.push_back(image->getData());
        }

        int width = images[0]->getWidth();
        int height = images[0]->getHeight();
        Core::TextureInternalFormat internalFormat = convertToInternalFormat(images[0]->getChannels());
        Core::TextureExternalFormat dataFormat = convertToExternalFormat(images[0]->getChannels());

        Core::RenderCommand::loadTextureCubeMap(
            m_rendererId,
            internalFormat, dataFormat,
            width, height,
            imagesData
        );

    }

    TextureCubeMap::~TextureCubeMap() {
        Core::RenderCommand::deleteTexture(m_rendererId);
    }

    void TextureCubeMap::activate(unsigned int slot) {
        Core::RenderCommand::activateTexture(m_rendererId, Core::TextureType::CubeMap, slot);
    }

}