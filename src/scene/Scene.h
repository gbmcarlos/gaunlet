#pragma once

#include "../core/application/TimeStep.h"
#include "camera/OrthographicCamera.h"
#include "entity/GraphicsComponents.h"
#include "entity/PhysicsComponents.h"
#include "physics/PhysicsWorld.h"

#include <entt/entt.hpp>

namespace engine {

    class Entity;

    class Scene {

        friend class Entity;

    public:
        Scene();
        ~Scene();

        Entity createEntity();

        void enablePhysics(glm::vec2 gravity);
        void start(const std::shared_ptr<OrthographicCamera>& camera);
        void onUpdate(TimeStep timeStep);
        void stop();

    private:

        entt::registry m_registry;
        std::shared_ptr<OrthographicCamera> m_camera = nullptr;
        PhysicsWorld* m_physicsWorld = nullptr;

        void initPhysics();
        void simulatePhysics(TimeStep timeStep);

        void initScripts();
        void runScripts(TimeStep timeStep);
        void destroyScripts();

        void renderElements();
        void renderPolygons();
        void renderCircles();

    };

}