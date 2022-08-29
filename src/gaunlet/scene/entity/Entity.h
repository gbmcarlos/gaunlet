#pragma once

#include <entt/entt.hpp>

namespace gaunlet::Scene {

    // ENTITY DECLARATION

    class Registry; // Forward-declare the Registry, for the Entity

    struct RelationshipComponent {
        entt::entity m_parent = entt::null;
        std::vector<entt::entity> m_children = {};
    };

    class Entity {

    public:

        Entity();
        Entity(entt::entity entityHandle, Registry* registry);
        Entity(int entityHandle, Registry* registry);

        int getId();
        Entity createChild();
        Entity getParent();

        template<typename T>
        bool hasComponent();

        template<typename T>
        T& getComponent();

        template<typename T, typename... Args>
        T& addComponent(Args&&... args);

        template<typename T, typename... Args>
        void addEmptyComponent(Args&&... args);

        operator bool() const {return m_handle != entt::null; }

    private:
        entt::entity m_handle = entt::null;
        Registry* m_registry = nullptr;

    };

    // REGISTRY DECLARATION

    class Scene; // Forward-declare the scene, for the Registry

    class Registry {

        friend class Entity;
        friend class Scene;

    public:
        Entity createEntity();

    private:
        entt::registry m_registry;

    };

    // ENTITY IMPLEMENTATION

    template<typename T>
    bool Entity::hasComponent() {
        return m_registry->m_registry.all_of<T>(m_handle);
    }

    template<typename T>
    T& Entity::getComponent() {
        if (!hasComponent<T>()) {
            throw std::runtime_error("Component not found");
        }
        return m_registry->m_registry.get<T>(m_handle);
    }

    template<typename T, typename... Args>
    T& Entity::addComponent(Args&&... args) {
        return m_registry->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void Entity::addEmptyComponent(Args&&... args) {
        m_registry->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
    }

}