#pragma once

#include "../core/application/TimeStep.h"
#include "entt/entt.hpp"

namespace engine {

    class Entity;

    class Scene {

        friend class Entity;

    public:
        Scene() = default;
        ~Scene() = default;

        Entity createEntity();
        void onUpdate(TimeStep timeStep);

    private:
        entt::registry m_registry;

    };

}