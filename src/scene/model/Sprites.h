#pragma once

#include "Model.h"

namespace engine {

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