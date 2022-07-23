#pragma once

#include "glm/glm.hpp"

namespace engine {

    struct SpeedComponent {

        SpeedComponent() = default;
        SpeedComponent(const SpeedComponent&) = default;
        SpeedComponent(glm::vec3 speed) : m_speed(speed) {}

        glm::vec3 m_speed;

    };

}