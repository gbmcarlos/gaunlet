#pragma once

#include "../Mesh.h"
#include "CircleVertex.h"

#include <vector>

namespace engine {

    class CircleMesh : public Mesh<CircleVertex> {

    public:

        CircleMesh();

    };

}