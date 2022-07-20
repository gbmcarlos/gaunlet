#pragma once

#include "graphics/mesh/Mesh.h"

namespace engine {

    class SquareMesh : public Mesh {

    public:

        SquareMesh() {

            m_vertices.push_back(Vertex{{-0.5f, -0.5f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}});
            m_vertices.push_back(Vertex{{0.5f, -0.5f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}});
            m_vertices.push_back(Vertex{{0.5f, 0.5f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}});
            m_vertices.push_back(Vertex{{-0.5f, 0.5f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}});

            m_faceIndices = std::vector<unsigned int>{
                0, 1, 2,
                2, 3, 0
            };
            m_edgeIndices = std::vector<unsigned int>{
                0, 1,
                1, 2,
                2, 3,
                3, 0,
                2, 0
            };
            m_polygonIndices = std::vector<unsigned int>{
                0, 1,
                1, 2,
                2, 3,
                3, 0
            };

        }

    };

}