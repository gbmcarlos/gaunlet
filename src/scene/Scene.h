#pragma once

#include "../core/application/TimeStep.h"
#include "entity/GraphicsComponents.h"
#include "entity/PhysicsComponents.h"

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace engine {

    class Entity;

    class Scene {

        friend class Entity;

    public:
        Scene() = default;
        ~Scene();

        Entity createEntity();

        void start(glm::vec2 gravity);
        void stop();
        void onUpdate(TimeStep timeStep);

    private:

        entt::registry m_registry;
        b2World* m_physicsWorld = nullptr;

        void initPhysics(glm::vec2 gravity);
        void createRigidBody(RigidBodyComponent& rigidBody, TransformComponent transform);
        void createBoxFixture(b2Body* body, BoxColliderComponent& boxCollider, TransformComponent transform);
        void createCircleFixture(b2Body* body, CircleColliderComponent& boxCollider, TransformComponent transform);

        void initScripts();

        void destroyPhysics();
        void destroyScripts();

        void runScripts(TimeStep timeStep);
        void simulatePhysics(TimeStep timeStep);
        void renderElements();

        void renderPolygons();
        void renderCircles();

        inline static b2BodyType convertRigidBodyType(RigidBodyComponent::Type type) {

            switch (type) {
                case RigidBodyComponent::Type::Static: return b2_staticBody;
                case RigidBodyComponent::Type::Kinematic: return b2_kinematicBody;
                case RigidBodyComponent::Type::Dynamic: return b2_dynamicBody;
            }

        }

    };

}