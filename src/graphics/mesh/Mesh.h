#pragma once

#include "Vertex.h"

#include <vector>

namespace engine {

    class Mesh {

    public:

        Mesh() : m_vertices({}), m_faceIndices({}) {}

        inline std::vector<Vertex> getVertices() const {return m_vertices;}
        inline std::vector<unsigned int> getFaceIndices() const {return m_faceIndices;}
        inline std::vector<unsigned int> getEdgeIndices() const {return m_edgeIndices;}
        inline std::vector<unsigned int> getPolygonIndices() const {return m_polygonIndices;}

    protected:
        std::vector<Vertex> m_vertices = {};
        std::vector<unsigned int> m_faceIndices = {};
        std::vector<unsigned int> m_edgeIndices = {};
        std::vector<unsigned int> m_polygonIndices = {};

    };

}