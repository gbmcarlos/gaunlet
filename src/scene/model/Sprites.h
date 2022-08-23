#pragma once

#include "scene/model/Model.h"

namespace engine::Scene {

    class Triangle2DModel : public Model {
    public:
        Triangle2DModel() : Model(ASSETS_PATH"/models/triangle-2d.obj") {}
    };

    class Square2DModel : public Model {
    public:
        Square2DModel() : Model(ASSETS_PATH"/models/square-2d.obj") {}
    };

    class CubeModel : public Model {
    public:
        CubeModel() : Model(ASSETS_PATH"/models/cube.obj") {}
    };

}