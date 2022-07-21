#pragma once

#include "glm/glm.hpp"

namespace engine {

    struct Vertex {

        glm::vec4 m_position;
        glm::vec2 m_textureCoordinates;
        int m_textureIndex;
        glm::vec4 m_color;

    };

}