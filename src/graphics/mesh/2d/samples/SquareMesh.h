#pragma once

#include "../../PolygonMesh.h"

namespace engine {

    class SquareMesh : public PolygonMesh {

    public:

        SquareMesh() {

            m_vertices.push_back(PolygonVertex{{-0.5f, -0.5f, 1.0f, 1.0f}, {0.0f, 0.0f}});
            m_vertices.push_back(PolygonVertex{{0.5f, -0.5f, 1.0f, 1.0f}, {1.0f, 0.0f}});
            m_vertices.push_back(PolygonVertex{{0.5f, 0.5f, 1.0f, 1.0f}, {1.0f, 1.0f}});
            m_vertices.push_back(PolygonVertex{{-0.5f, 0.5f, 1.0f, 1.0f}, {0.0f, 1.0f}});

            m_indices = std::vector<unsigned int>{
                0, 1, 2,
                2, 3, 0
            };

        }

    };

}