#pragma once

#include "gaunlet/graphics/texture/TextureCubeMap.h"
#include "gaunlet/pch.h"

namespace gaunlet::Prefab::Skyboxes {

    class SimpleSkyboxCubeMap : public Graphics::TextureCubeMap {
    public:
        SimpleSkyboxCubeMap() : Graphics::TextureCubeMap(std::vector<const char*>{
            GL_PREFABS_PATH"/skyboxes/sky/right.bmp",
            GL_PREFABS_PATH"/skyboxes/sky/left.bmp",
            GL_PREFABS_PATH"/skyboxes/sky/top.bmp",
            GL_PREFABS_PATH"/skyboxes/sky/bottom.bmp",
            GL_PREFABS_PATH"/skyboxes/sky/front.bmp",
            GL_PREFABS_PATH"/skyboxes/sky/back.bmp"
        }) {}
    };

}