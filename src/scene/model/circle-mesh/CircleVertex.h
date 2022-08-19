#pragma once

#include "glm/glm.hpp"

namespace engine {

    struct CircleVertex {

        CircleVertex(glm::vec4 position, glm::vec2 localCoordinates, glm::vec2 textureCoordinates) :
            m_position(position),
            m_localCoordinates(localCoordinates),
            m_textureCoordinates(textureCoordinates),
            m_entityIndex(0)
        {}

        glm::vec4 m_position;
        glm::vec2 m_localCoordinates;
        glm::vec2 m_textureCoordinates;
        unsigned int m_entityIndex;

    };

}