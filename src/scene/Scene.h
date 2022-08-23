#pragma once

#include "../core/application/TimeStep.h"
#include "../graphics/framebuffer/Framebuffer.h"
#include "camera/OrthographicCamera.h"
#include "entity/GraphicsComponents.h"
#include "entity/PhysicsComponents.h"
#include "entity/LightingComponents.h"
#include "physics/PhysicsWorld.h"

#include <entt/entt.hpp>

namespace engine::Scene {

    class Entity;

    class Scene {

        friend class Entity;

    public:
        Scene();
        ~Scene();

        Entity createEntity();

        const Core::Ref<PhysicsWorld>& enablePhysics(glm::vec2 gravity);
        void start();
        void play();
        void pause();
        void togglePlay();

        void update(Core::TimeStep timeStep);
        void render(const Core::Ref<Camera>& camera, const Core::Ref<Graphics::Framebuffer>& framebuffer = nullptr);
        void render(const Core::Ref<Camera>& camera, const DirectionalLightComponent& directionalLight, const Core::Ref<Graphics::Framebuffer>& framebuffer = nullptr);
        void stop();

    private:

        bool m_playing = false;
        entt::registry m_registry;
        Core::Ref<Camera> m_camera = nullptr;
        Core::Ref<PhysicsWorld> m_physicsWorld = nullptr;

        void initPhysics();
        void simulatePhysics(Core::TimeStep timeStep);

        void initScripts();
        void runScripts(Core::TimeStep timeStep);
        void destroyScripts();

        void renderPolygons();
        void renderCircles();

    };

}