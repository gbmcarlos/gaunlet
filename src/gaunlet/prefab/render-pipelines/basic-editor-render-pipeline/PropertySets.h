#pragma once

#include "gaunlet/pch.h"

namespace gaunlet::Prefab::BasicEditorRenderPipeline {

    struct ModelEntityProperties {

        ModelEntityProperties(int entityId, const glm::mat4& transform, glm::vec4 color)
            : m_entityId(entityId), m_transform(transform), m_color(color), m_textureIndex(0) {}

        // The order of these properties is optimized to minimize required padding when using this data in a uniform buffer
        glm::mat4 m_transform;
        glm::vec4 m_color;
        unsigned int m_textureIndex;
        int m_entityId; glm::vec2 pad2 = {};

    };

    struct CircleEntityProperties {

        CircleEntityProperties(int entityId, const glm::mat4& transform, glm::vec4 color, float thickness, float fade)
            : m_entityId(entityId), m_transform(transform), m_color(color), m_textureIndex(0), m_thickness(thickness), m_fade(fade) {}

        // The order of these properties is optimized to minimize required padding when using this data in a uniform buffer
        glm::mat4 m_transform;
        glm::vec4 m_color;
        unsigned int m_textureIndex;
        int m_entityId;
        float m_thickness;
        float m_fade;

    };

}