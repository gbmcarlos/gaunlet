#pragma once

#include "../../polygon-mesh/PolygonMesh.h"

namespace engine {

    class TriangleMesh : public PolygonMesh {

    public:

        TriangleMesh() {

            float triangleHeight = sqrt(3)/2;

            m_vertices.push_back(PolygonVertex{{-0.5f, -0.5f, 0.0f, 1.0f}, {0.0f, 0.0f}});
            m_vertices.push_back(PolygonVertex{{0.5f, -0.5f, 0.0f, 1.0f}, {1.0f, 0.0f}});
            m_vertices.push_back(PolygonVertex{{0.0f, triangleHeight - 0.5, 0.0f, 1.0f}, {0.5f, triangleHeight}});

            m_indices = std::vector<unsigned int>{
                0, 1, 2
            };

        }

    };

}