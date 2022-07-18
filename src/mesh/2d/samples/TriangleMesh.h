#pragma once

#include "../../Mesh.h"

namespace engine {

    class TriangleMesh : public Mesh {

    public:

        TriangleMesh() {

            m_vertices.push_back(Vertex{{-0.5f, -0.5f, 1.0f, 1.0f}, {0.8f, 0.0f, 0.0f, 1.0f}});
            m_vertices.push_back(Vertex{{0.5f, -0.5f, 1.0f, 1.0f}, {0.0f, 0.8f, 0.0f, 1.0f}});
            m_vertices.push_back(Vertex{{0.0f, sqrt(3)/2 - 0.5, 1.0f, 1.0f}, {0.0f, 0.0f, 0.8f, 1.0f}});

            m_indices = std::vector<unsigned int>{0, 1, 2};

        }

    };

}