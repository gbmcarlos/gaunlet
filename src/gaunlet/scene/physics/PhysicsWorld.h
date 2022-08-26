#pragma once

#include "gaunlet/core/application/TimeStep.h"
#include "gaunlet/scene/entity/GraphicsComponents.h"
#include "gaunlet/scene/entity/PhysicsComponents.h"

#include <box2d/box2d.h>

namespace gaunlet::Scene {

    class PhysicsWorld {

    public:

        constexpr static float FIXED_TIMESTEP = 1.f / 60.f;

        explicit PhysicsWorld(glm::vec2 gravity);
        ~PhysicsWorld();

        void setContactListener(b2ContactListener* contactListener);

        void createRigidBody(RigidBodyComponent& rigidBody, TransformComponent transform);
        void createBoxFixture(b2Body* body, BoxColliderComponent& boxCollider, TransformComponent transform);
        void createCircleFixture(b2Body* body, CircleColliderComponent& circleCollider, TransformComponent transform);

        void simulatePhysics(Core::TimeStep timeStep);
        void updateBody(RigidBodyComponent& rigidBody, TransformComponent& transform);

        void destroyPhysics();

    private:

        b2World* m_physicsWorld = nullptr;
        float m_fixedTimeStepAccumulator = 0;

        inline static b2BodyType convertRigidBodyType(RigidBodyComponent::Type type) {

            switch (type) {
                case RigidBodyComponent::Type::Static: return b2_staticBody;
                case RigidBodyComponent::Type::Kinematic: return b2_kinematicBody;
                case RigidBodyComponent::Type::Dynamic: return b2_dynamicBody;
            }

        }

    };

}