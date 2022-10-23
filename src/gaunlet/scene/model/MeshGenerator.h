#pragma once

#include "gaunlet/graphics/Vertex.h"

namespace gaunlet::Scene {

    class MeshGenerator {

    public:

        virtual std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() = 0;
        virtual void onGuiRender() {}

    };

}