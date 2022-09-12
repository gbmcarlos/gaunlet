#pragma once

#include "gaunlet/pch.h"

namespace gaunlet::Prefab::Basic2DRenderPipeline {

    struct SceneProperties {

        SceneProperties(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
            : m_viewMatrix(viewMatrix), m_projectionMatrix(projectionMatrix) {}

        glm::mat4 m_viewMatrix;
        glm::mat4 m_projectionMatrix;

    };

    struct ModelEntityProperties {

        ModelEntityProperties(const glm::mat4& transform, glm::vec4 color)
            : m_transform(transform), m_color(color), m_textureIndex(0) {}

        // The order of these properties is optimized to minimize required padding when using this data in a uniform buffer
        glm::mat4 m_transform;
        glm::vec4 m_color;
        unsigned int m_textureIndex; glm::vec3 pad3 = {};

    };

    struct CircleEntityProperties {

        CircleEntityProperties(const glm::mat4& transform, glm::vec4 color, float thickness, float fade)
            : m_transform(transform), m_color(color), m_textureIndex(0), m_thickness(thickness), m_fade(fade) {}

        // The order of these properties is optimized to minimize required padding when using this data in a uniform buffer
        glm::mat4 m_transform;
        glm::vec4 m_color;
        unsigned int m_textureIndex;
        float m_thickness;
        float m_fade; float pad1;

    };

}