#pragma once

#include "gaunlet/graphics/texture/TextureCubeMap.h"
#include "gaunlet/pch.h"

namespace gaunlet::Prefab::Skyboxes {

    class SimpleSkyboxCubeMap : public Graphics::TextureCubeMap {
    public:
        SimpleSkyboxCubeMap() : Graphics::TextureCubeMap(std::vector<const char*>{
            PREFABS_PATH"/skyboxes/sky/right.bmp",
            PREFABS_PATH"/skyboxes/sky/left.bmp",
            PREFABS_PATH"/skyboxes/sky/top.bmp",
            PREFABS_PATH"/skyboxes/sky/bottom.bmp",
            PREFABS_PATH"/skyboxes/sky/front.bmp",
            PREFABS_PATH"/skyboxes/sky/back.bmp"
        }) {}
    };

}