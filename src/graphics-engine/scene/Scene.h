#pragma once

#include "graphics-engine/core/application/TimeStep.h"
#include "graphics-engine/graphics/framebuffer/Framebuffer.h"
#include "graphics-engine/scene/camera/OrthographicCamera.h"
#include "graphics-engine/scene/entity/GraphicsComponents.h"
#include "graphics-engine/scene/entity/PhysicsComponents.h"
#include "graphics-engine/scene/entity/LightingComponents.h"
#include "graphics-engine/scene/physics/PhysicsWorld.h"

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