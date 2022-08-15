#pragma once

#include "../../polygon-mesh/PolygonMesh.h"

namespace engine {

    class CubeMesh : public PolygonMesh {

    public:

        CubeMesh() {

            // 0 - 3
            m_vertices.push_back(PolygonVertex{{-0.5f, -0.5f, 0.5f, 1.0f}, {0.25f, 0.33f}});
            m_vertices.push_back(PolygonVertex{{0.5f, -0.5f, 0.5f, 1.0f}, {0.50f, 0.33f}});
            m_vertices.push_back(PolygonVertex{{0.5f, 0.5f, 0.5f, 1.0f}, {0.50f, 0.66f}});
            m_vertices.push_back(PolygonVertex{{-0.5f, 0.5f, 0.5f, 1.0f}, {0.25f, 0.66f}});

            // 4 - 5
            m_vertices.push_back(PolygonVertex{{-0.5f, -0.5f, -0.5f, 1.0f}, {0.0f, 0.33f}});
            m_vertices.push_back(PolygonVertex{{-0.5f, 0.5f, -0.5f, 1.0f}, {0.0f, 0.66f}});

            // 6 - 7
            m_vertices.push_back(PolygonVertex{{-0.5f, -0.5f, -0.5f, 1.0f}, {0.25f, 0.0f}});
            m_vertices.push_back(PolygonVertex{{0.5f, -0.5f, -0.5f, 1.0f}, {0.5f, 0.0f}});

            // 8 - 9
            m_vertices.push_back(PolygonVertex{{0.5f, 0.5f, -0.5f, 1.0f}, {0.5f, 1.0f}});
            m_vertices.push_back(PolygonVertex{{-0.5f, 0.5f, -0.5f, 1.0f}, {0.25f, 1.0f}});

            // 10 - 13
            m_vertices.push_back(PolygonVertex{{0.5f, -0.5f, -0.5f, 1.0f}, {0.75f, 0.33f}});
            m_vertices.push_back(PolygonVertex{{-0.5f, -0.5f, -0.5f, 1.0f}, {1.0f, 0.33f}});
            m_vertices.push_back(PolygonVertex{{-0.5f, 0.5f, -0.5f, 1.0f}, {1.0f, 0.66f}});
            m_vertices.push_back(PolygonVertex{{0.5f, 0.5f, -0.5f, 1.0f}, {0.75f, 0.66f}});

            m_indices = std::vector<unsigned int>{
                // Front
                0, 1, 2,
                2, 3, 0,
                // Left
                4, 0, 3,
                3, 5, 4,
                // Bottom
                6, 7, 1,
                1, 0, 6,
                // Top
                3, 2, 8,
                8, 9, 3,
                // Right
                1, 10, 13,
                13, 2, 1,
                // Back
                10, 11, 12,
                12, 13, 10
            };

        }

    };

}