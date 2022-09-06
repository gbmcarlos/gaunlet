#pragma once

#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/graphics/texture/Image.h"

#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

    class TextureCubeMap : public Texture {

    public:

        explicit TextureCubeMap(const std::vector<const char*>& filePaths);
        ~TextureCubeMap();

        void activate(unsigned int slot) override;

    };

}