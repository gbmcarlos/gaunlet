#pragma once

#include "../Mesh.h"

#include <utility>
#include "PolygonVertex.h"

namespace engine {

    class PolygonMesh : public Mesh<PolygonVertex> {

    public:

        PolygonMesh() = default;
        PolygonMesh(const PolygonMesh& other) : Mesh<PolygonVertex>(other.m_vertices, other.m_indices) {}
        PolygonMesh(std::vector<PolygonVertex> vertices, std::vector<unsigned int> indices)
            : Mesh<PolygonVertex>(std::move(vertices), std::move(indices)) {}

    };

}