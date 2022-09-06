#pragma once

#include "gaunlet/graphics/texture/TextureCubeMap.h"
#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    class SimpleSkyboxCubeMap : public Graphics::TextureCubeMap {
    public:
        SimpleSkyboxCubeMap() : Graphics::TextureCubeMap(std::vector<const char*>{
            ASSETS_PATH"/skyboxes/sky/right.bmp",
            ASSETS_PATH"/skyboxes/sky/left.bmp",
            ASSETS_PATH"/skyboxes/sky/top.bmp",
            ASSETS_PATH"/skyboxes/sky/bottom.bmp",
            ASSETS_PATH"/skyboxes/sky/front.bmp",
            ASSETS_PATH"/skyboxes/sky/back.bmp"
        }) {}
    };

}