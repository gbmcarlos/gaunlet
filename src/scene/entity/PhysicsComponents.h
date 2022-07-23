#pragma once

#include "glm/glm.hpp"

namespace engine {

    struct RigidBodyComponent {

        enum class Type {
            Static, Kinematic, Dynamic
        };

        RigidBodyComponent() = default;
        RigidBodyComponent(const RigidBodyComponent&) = default;

        RigidBodyComponent(Type type) : m_type(type) {}

        Type m_type = Type::Static;
        void* m_runtimeBody = nullptr;

    };

    struct BoxColliderComponent {

        BoxColliderComponent() = default;
        BoxColliderComponent(float density) : m_density(density) {}
        BoxColliderComponent(glm::vec2 padding) : m_padding(padding) {}
        BoxColliderComponent(float density, glm::vec2 padding) : m_density(density), m_padding(padding) {}
        BoxColliderComponent(const BoxColliderComponent&) = default;

        glm::vec2 m_size = {0.5f, 0.5f};
        float m_density = 1.0f;
        glm::vec2 m_padding = {-0.01f, -0.01f};

        void* m_runtimeFixture = nullptr;

    };

}