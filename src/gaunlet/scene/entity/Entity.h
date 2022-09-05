#pragma once

#include <entt/entt.hpp>

namespace gaunlet::Scene {

    // ENTITY DECLARATION

    class Registry; // Forward-declare the Registry, for the Entity

    struct RelationshipComponent {
        entt::entity m_parent = entt::null;
        std::vector<entt::entity> m_children = {};
    };

    struct NameComponent {
        explicit NameComponent(const char* name = nullptr) : m_name(name) {}
        const char* m_name = nullptr;
    };

    class Entity {

    public:

        Entity();
        Entity(entt::entity entityHandle, Registry* registry);
        Entity(int entityHandle, Registry* registry);

        int getId() const;
        bool hasName();
        const char* getName();
        explicit operator bool() const;
        bool operator==(const Entity& other) const;
        bool operator!=(const Entity& other) const;

        Entity getParent();
        Entity createChild(const char* name = nullptr);
        Entity addChild(Entity child);

        void destroy();
        void destroyChild(Entity child);

        template<typename T>
        Entity createTaggedChild();

        template<typename T>
        Entity findTaggedAncestor();

        template<typename T>
        bool hasComponent();

        template<typename T>
        T& getComponent();

        template<typename T, typename... Args>
        T& addComponent(Args&&... args);

        template<typename T, typename... Args>
        void addEmptyComponent(Args&&... args);

        template<typename T>
        void removeComponent();

    private:
        entt::entity m_handle = entt::null;
        Registry* m_registry = nullptr;

        void adopt(Entity& parent, Entity& child);
        void abandon(Entity& parent, Entity& child);

    };

    // REGISTRY DECLARATION

    class Scene; // Forward-declare the scene, for the Registry

    class Registry {

        friend class Entity;
        friend class Scene;

    public:
        Entity createEntity(const char* name = nullptr);

        template<typename T>
        Entity createTaggedEntity(const char* name = nullptr);

        int countEntities();

        template<typename T>
        int countTaggedEntities();

    private:
        entt::registry m_registry;

    };

    // ENTITY IMPLEMENTATION

    template<typename T>
    Entity Entity::createTaggedChild() {

        // Create the child entity, delegating on the registry (so it will attach the Relationship component)
        auto child = m_registry->createTaggedEntity<T>();
        adopt(*this, child);

        return child;

    }

    template<typename T>
    Entity Entity::findTaggedAncestor() {

        bool found = false;

        Entity current = *this;
        Entity ancestor = current.getParent();

        while (true) {

            // If we're run out of ancestor in the line, stop
            if (!ancestor) {
                break;
            }

            if (ancestor.hasComponent<T>()) {
                found = true;
                break;
            }

            current = current.getParent();
            ancestor = ancestor.getParent();

        }

        if (found) {
            return ancestor;
        } else {
            return {};
        }

    }

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

    template<typename T>
    void Entity::removeComponent() {
        m_registry->m_registry.remove<T>(m_handle);
    }

    // REGISTRY IMPLEMENTATION

    template<typename T>
    Entity Registry::createTaggedEntity(const char* name) {

        auto entity = createEntity(name);
        entity.addEmptyComponent<T>();

        return entity;

    }

    template<typename T>
    int Registry::countTaggedEntities() {

        auto view = m_registry.view<T>();
        return view.size();

    }

}