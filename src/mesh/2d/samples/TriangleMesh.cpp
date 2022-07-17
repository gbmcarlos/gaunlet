#include "TriangleMesh.h"

namespace engine {

    TriangleMesh::TriangleMesh() {

        m_vertices.push_back(Vertex{{-0.5f, -0.5f, 1.0f, 1.0f}, {0.8f, 0.0f, 0.0f, 1.0f}});
        m_vertices.push_back(Vertex{{0.5f, -0.5f, 1.0f, 1.0f}, {0.0f, 0.8f, 0.0f, 1.0f}});
        m_vertices.push_back(Vertex{{0.0f, 0.5f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.8f, 1.0f}});

        m_indices = std::vector<unsigned int>{0, 1, 2};

    }

}