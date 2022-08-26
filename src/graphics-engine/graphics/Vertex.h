#pragma once

#include "graphics-engine/pch.h"

namespace engine::Scene {

    struct Vertex {

        Vertex(glm::vec4 position, glm::vec3 normal, glm::vec2 textureCoordinates) :
            m_position(position),
            m_normal(normal),
            m_textureCoordinates(textureCoordinates),
            m_entityIndex(0)
        {}

        glm::vec4 m_position;
        glm::vec3 m_normal;
        glm::vec2 m_textureCoordinates;
        unsigned int m_entityIndex;

    };

}