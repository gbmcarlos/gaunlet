#pragma once

#include "glm/glm.hpp"

namespace engine {

    struct Vertex {

        Vertex(glm::vec4 position, glm::vec2 textureCoordinates) : m_position(position), m_textureCoordinates(textureCoordinates), m_textureIndex(0), m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) {}

        glm::vec4 m_position;
        glm::vec2 m_textureCoordinates;
        int m_textureIndex;
        glm::vec4 m_color;

    };

}