#pragma once

#include "../core/application/TimeStep.h"
#include "entity/GraphicsComponents.h"
#include "entity/PhysicsComponents.h"
#include "physics/PhysicsWorld.h"

#include <entt/entt.hpp>

namespace engine {

    class Entity;

    class Scene {

        friend class Entity;

    public:
        Scene() = default;

        Entity createEntity();

        void start(glm::vec2 gravity);
        void onUpdate(TimeStep timeStep);
        void stop();

    private:

        entt::registry m_registry;
        PhysicsWorld m_physicsWorld;

        void initPhysics(glm::vec2 gravity);
        void simulatePhysics(TimeStep timeStep);

        void initScripts();
        void runScripts(TimeStep timeStep);
        void destroyScripts();

        void renderElements();
        void renderPolygons();
        void renderCircles();

    };

}