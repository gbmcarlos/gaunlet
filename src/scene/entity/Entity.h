#pragma once

#include "../Scene.h"

namespace engine {

    class Entity {

    public:

        Entity(entt::entity entityHandle, Scene* scene) : m_handle(entityHandle), m_scene(scene) {}

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
        entt::entity m_handle;
        Scene* m_scene;

    };

}