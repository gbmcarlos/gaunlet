#pragma once

#include "../pch.h"

namespace engine::Scene {

    struct DirectionalLightComponent {

        DirectionalLightComponent()
            : m_color(glm::vec3(1.0f, 1.0f, 1.0f)), m_position(glm::vec3(0.0f, 0.0f, 0.0f)), m_ambientIntensity(1.0f), m_diffuseIntensity(1.0f) {}

        DirectionalLightComponent(glm::vec3 color, glm::vec3 position, float ambientIntensity, float diffuseIntensity)
            : m_color(color), m_ambientIntensity(ambientIntensity), m_position(position), m_diffuseIntensity(diffuseIntensity) {}

        glm::vec3 m_color;
        glm::vec3 m_position;
        float m_ambientIntensity;
        float m_diffuseIntensity;

    };

}