#pragma once

#include "../../Mesh.h"

namespace engine {

    class SquareMesh : public Mesh {

    public:

        SquareMesh() {

            m_vertices.push_back(Vertex{{-0.5f, -0.5f, 1.0f, 1.0f}, {0.0f, 0.0f}, 0, {1.0f, 1.0f, 1.0f, 1.0f}});
            m_vertices.push_back(Vertex{{0.5f, -0.5f, 1.0f, 1.0f}, {1.0f, 0.0f}, 0, {1.0f, 1.0f, 1.0f, 1.0f}});
            m_vertices.push_back(Vertex{{0.5f, 0.5f, 1.0f, 1.0f}, {1.0f, 1.0f}, 0, {1.0f, 1.0f, 1.0f, 1.0f}});
            m_vertices.push_back(Vertex{{-0.5f, 0.5f, 1.0f, 1.0f}, {0.0f, 1.0f}, 0, {1.0f, 1.0f, 1.0f, 1.0f}});

            m_indices = std::vector<unsigned int>{
                0, 1, 2,
                2, 3, 0
            };

        }

    };

}