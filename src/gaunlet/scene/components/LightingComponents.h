#pragma once

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    struct DirectionalLightComponent {

        DirectionalLightComponent()
            : m_color(glm::vec3(1.0f, 1.0f, 1.0f)), m_direction(glm::vec3(0.0f, 0.0f, 0.0f)), m_ambientIntensity(1.0f), m_diffuseIntensity(1.0f) {}

        DirectionalLightComponent(glm::vec3 color, glm::vec3 direction, float ambientIntensity, float diffuseIntensity)
            : m_color(color), m_ambientIntensity(ambientIntensity), m_direction(direction), m_diffuseIntensity(diffuseIntensity) {}

        glm::vec3 m_color;
        glm::vec3 m_direction;
        float m_ambientIntensity;
        float m_diffuseIntensity;

    };

}