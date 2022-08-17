#pragma once

#include "../pch.h"

namespace engine {

    template <typename T>
    class Mesh {

    public:

        Mesh() : m_vertices({}), m_indices() {}
        Mesh(std::vector<T> vertices, std::vector<unsigned int> indices) : m_vertices(vertices), m_indices(std::move(indices)) {}
        inline const std::vector<T>& getVertices() const {return m_vertices;}
        inline const std::vector<unsigned int>& getIndices() const {return m_indices;}

    protected:
        std::vector<T> m_vertices = {};
        std::vector<unsigned int> m_indices = {};

    };

}