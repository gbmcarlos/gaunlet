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

        const Ref<PhysicsWorld>& enablePhysics(glm::vec2 gravity);
        void start();
        void play();
        void pause();
        void togglePlay();

        void update(TimeStep timeStep);
        void render(const Ref<Camera>& camera, const Ref<Framebuffer>& framebuffer = nullptr);
        void stop();

    private:

        bool m_playing = false;
        entt::registry m_registry;
        Ref<Camera> m_camera = nullptr;
        Ref<PhysicsWorld> m_physicsWorld = nullptr;

        void initPhysics();
        void simulatePhysics(TimeStep timeStep);

        void initScripts();
        void runScripts(TimeStep timeStep);
        void destroyScripts();

        void renderPolygons();
        void renderCircles();

    };

}