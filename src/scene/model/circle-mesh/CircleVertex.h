#pragma once

#include "glm/glm.hpp"

namespace engine {

    struct CircleVertex {

        CircleVertex(glm::vec4 position, glm::vec2 localCoordinates, glm::vec2 textureCoordinates) :
            m_position(position),
            m_localCoordinates(localCoordinates),
            m_thickness(0.5f),
            m_fade(0.01f),
            m_textureCoordinates(textureCoordinates),
            m_textureIndex(0),
            m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
        {}

        glm::vec4 m_position;
        glm::vec2 m_localCoordinates;
        float m_thickness;
        float m_fade;
        glm::vec2 m_textureCoordinates;
        int m_textureIndex;
        glm::vec4 m_color;

    };

}