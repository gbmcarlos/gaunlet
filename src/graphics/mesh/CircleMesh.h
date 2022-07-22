#pragma once

#include "CircleVertex.h"

#include <vector>

namespace engine {

    class CircleMesh {

    public:

        CircleMesh() {

            m_vertices.push_back(CircleVertex{{-0.5f, -0.5f, 1.0f, 1.0f}, {-1.0f, -1.0f}, {0.0f, 0.0f}});
            m_vertices.push_back(CircleVertex{{0.5f, -0.5f, 1.0f, 1.0f}, {1.0f, -1.0f}, {1.0f, 0.0f}});
            m_vertices.push_back(CircleVertex{{0.5f, 0.5f, 1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}});
            m_vertices.push_back(CircleVertex{{-0.5f, 0.5f, 1.0f, 1.0f}, {-1.0f, 1.0f}, {0.0f, 1.0f}});

            m_indices = std::vector<unsigned int>{
                0, 1, 2,
                2, 3, 0
            };

        }

        inline std::vector<CircleVertex> getVertices() const {return m_vertices;}
        inline std::vector<unsigned int> getIndices() const {return m_indices;}

    protected:
        std::vector<CircleVertex> m_vertices = {};
        std::vector<unsigned int> m_indices = {};

    };

}