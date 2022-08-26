#pragma once

#include "graphics-engine/graphics/Vertex.h"

#include "graphics-engine/pch.h"

namespace engine::Scene {

    class Mesh {

    public:

        Mesh() : m_vertices({}), m_indices() {}
        Mesh(std::vector<Graphics::Vertex> vertices, std::vector<unsigned int> indices) : m_vertices(std::move(vertices)), m_indices(std::move(indices)) {}
        inline const std::vector<Graphics::Vertex>& getVertices() const {return m_vertices;}
        inline const std::vector<unsigned int>& getIndices() const {return m_indices;}

    protected:
        std::vector<Graphics::Vertex> m_vertices = {};
        std::vector<unsigned int> m_indices = {};

    };

}