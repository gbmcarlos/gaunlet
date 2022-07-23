#pragma once

#include "PolygonVertex.h"

#include <vector>

namespace engine {

    class PolygonMesh {

    public:

        PolygonMesh() : m_vertices({}), m_indices({}) {}

        inline std::vector<PolygonVertex> getVertices() const {return m_vertices;}
        inline std::vector<unsigned int> getIndices() const {return m_indices;}

    protected:
        std::vector<PolygonVertex> m_vertices = {};
        std::vector<unsigned int> m_indices = {};

    };

}