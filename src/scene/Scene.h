#pragma once

#include "../core/application/TimeStep.h"
#include "../graphics/framebuffer/Framebuffer.h"
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

        const std::shared_ptr<PhysicsWorld>& enablePhysics(glm::vec2 gravity);
        void start();
        void play();
        void pause();
        void togglePlay();

        void update(TimeStep timeStep);
        void render(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Framebuffer>& framebuffer = nullptr);
        void stop();

    private:

        bool m_playing = false;
        entt::registry m_registry;
        std::shared_ptr<Camera> m_camera = nullptr;
        std::shared_ptr<PhysicsWorld> m_physicsWorld = nullptr;

        void initPhysics();
        void simulatePhysics(TimeStep timeStep);

        void initScripts();
        void runScripts(TimeStep timeStep);
        void destroyScripts();

        void renderPolygons();
        void renderCircles();

    };

}