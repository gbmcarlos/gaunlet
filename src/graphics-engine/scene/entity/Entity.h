#pragma once

#include "graphics-engine/scene/Scene.h"

namespace engine::Scene {

    class Entity {

    public:

        Entity() = default;
        Entity(entt::entity entityHandle, Scene* scene) : m_handle(entityHandle), m_scene(scene) {}

        int getId() {
            return (int) m_handle;
        }

        template<typename T>
        bool hasComponent() {
            return m_scene->m_registry.all_of<T>(m_handle);
        }

        template<typename T>
        T& getComponent() {
            if (!hasComponent<T>()) {
                throw std::runtime_error("Component not found");
            }
            return m_scene->m_registry.get<T>(m_handle);
        }

        template<typename T, typename... Args>
        T& addComponent(Args&&... args) {
            return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
        }

    private:
        entt::entity m_handle = entt::null;
        Scene* m_scene = nullptr;

    };

}