#pragma once

#include "../core/application/TimeStep.h"
#include "entity/PhysicsComponents.h"

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace engine {

    class Entity;

    class Scene {

        friend class Entity;

    public:
        Scene();
        ~Scene();

        Entity createEntity();

        void start();
        void onUpdate(TimeStep timeStep);

    private:

        entt::registry m_registry;
        b2World* m_physicsWorld;

        void initPhysics();
        void initScripts();

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