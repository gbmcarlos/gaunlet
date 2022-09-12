#pragma once

#include "gaunlet/scene/model/Model.h"

namespace gaunlet::Prefab::Sprites {

    class Triangle2DModel : public Scene::Model {
    public:
        Triangle2DModel() : Model(PREFABS_PATH"/sprites/models/triangle-2d.obj") {}
    };

    class Square2DModel : public Scene::Model {
    public:
        Square2DModel() : Model(PREFABS_PATH"/sprites/models/square-2d.obj") {}
    };

    class CubeModel : public Scene::Model {
    public:
        CubeModel() : Model(PREFABS_PATH"/sprites/models/cube.obj") {}
    };

}