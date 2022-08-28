#pragma once

#include "gaunlet/graphics/vertex-buffer/BufferLayout.h"
#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

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

        static BufferLayout getBufferLayout() {
            return {
                {"a_position", 4, Core::PrimitiveDataType::Float},
                {"a_normal", 3, Core::PrimitiveDataType::Float},
                {"a_textureCoordinates", 2, Core::PrimitiveDataType::Float},
                {"a_entityIndex", 1, Core::PrimitiveDataType::UInt},
            };
        }

    };

}