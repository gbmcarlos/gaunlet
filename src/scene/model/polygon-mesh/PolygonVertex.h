#pragma once

#include "glm/glm.hpp"

namespace engine {

    struct PolygonVertex {

        PolygonVertex(glm::vec4 position, glm::vec4 normal, glm::vec2 textureCoordinates) :
            m_position(position),
            m_normal(normal),
            m_textureCoordinates(textureCoordinates),
            m_textureIndex(0),
            m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
        {}

        glm::vec4 m_position;
        glm::vec4 m_normal;
        glm::vec2 m_textureCoordinates;
        int m_textureIndex;
        glm::vec4 m_color;

    };

}