#pragma once

#include "gaunlet/pch.h"
#include <box2d/box2d.h>

namespace engine::Scene {

    struct RigidBodyComponent {

        enum class Type {
            Static, Kinematic, Dynamic
        };

        RigidBodyComponent() = default;
        RigidBodyComponent(const RigidBodyComponent&) = default;

        RigidBodyComponent(Type type) : m_type(type) {}
        RigidBodyComponent(Type type, bool fixedRotation) : m_type(type), m_fixedRotation(fixedRotation) {}

        Type m_type = Type::Static;
        bool m_fixedRotation = false;

        b2Body* m_runtimeBody = nullptr;

    };

    struct BoxColliderComponent {

        BoxColliderComponent() = default;
        BoxColliderComponent(glm::vec2 padding, float density, float friction, float restitution, float restitutionThreshold) :
            m_padding(padding),
            m_density(density),
            m_friction(friction),
            m_restitution(restitution),
            m_restitutionThreshold(restitutionThreshold)
        {}
        BoxColliderComponent(const BoxColliderComponent&) = default;

        b2Body* getBody() {
            return m_runtimeFixture->GetBody();
        }

        glm::vec2 m_size = {0.5f, 0.5f}; // Always start as a square of 1x1, so it can scale regularly
        glm::vec2 m_padding = {-0.01f, -0.01f};
        float m_density = 1.0f;
        float m_friction = 0.0f;
        float m_restitution = 0.0f;
        float m_restitutionThreshold = 1.0f;

        b2Fixture* m_runtimeFixture = nullptr;

    };

    struct CircleColliderComponent {

        CircleColliderComponent() = default;
        CircleColliderComponent(float padding, float density, float friction, float restitution, float restitutionThreshold) :
            m_padding(padding),
            m_density(density),
            m_friction(friction),
            m_restitution(restitution),
            m_restitutionThreshold(restitutionThreshold)
        {}
        CircleColliderComponent(const CircleColliderComponent&) = default;

        b2Body* getBody() {
            return m_runtimeFixture->GetBody();
        }

        float m_radius = 0.5f; // Always start as a circle of radius 0.5 (diameter of 1), so it can scale regularly
        float m_padding = -0.01f;
        float m_density = 1.0f;
        float m_friction = 0.0f;
        float m_restitution = 0.0f;
        float m_restitutionThreshold = 1.0f;

        b2Fixture* m_runtimeFixture = nullptr;

    };

}