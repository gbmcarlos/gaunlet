#pragma once

#include "Vertex.h"

#include <vector>

namespace engine {

    class Mesh {

    public:

        Mesh() : m_vertices({}), m_indices({}) {}

        inline std::vector<Vertex> getVertices() const {return m_vertices;}
        inline std::vector<unsigned int> getFaceIndices() const {return m_indices;}

    protected:
        std::vector<Vertex> m_vertices = {};
        std::vector<unsigned int> m_indices = {};

    };

}