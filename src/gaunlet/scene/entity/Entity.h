#pragma once

#include <entt/entt.hpp>

namespace gaunlet::Scene {

    // ENTITY DECLARATION

    class Scene; // Forward-declare the Registry, for the Entity

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
        Entity(entt::entity entityHandle, Scene* scene);
        Entity(int entityHandle, Scene* scene);

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
        Entity createTaggedChild(const char* name = nullptr);

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
        Scene* m_scene = nullptr;

        void adopt(Entity& parent, Entity& child);
        void abandon(Entity& parent, Entity& child);

    };

    // REGISTRY DECLARATION

    class Scene {

        friend class Entity;

    public:
        Entity createEntity(const char* name = nullptr);

        template<typename T>
        Entity createTaggedEntity(const char* name = nullptr);

        int countEntities();

        template<typename T>
        int countTaggedEntities();

        inline entt::registry& getRegistry() {return m_registry; }

    private:
        entt::registry m_registry;

    };

    // ENTITY IMPLEMENTATION

    template<typename T>
    Entity Entity::createTaggedChild(const char* name) {

        // Create the child entity, delegating on the registry (so it will attach the Relationship component)
        auto child = m_scene->createTaggedEntity<T>(name);
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
        return m_scene->m_registry.all_of<T>(m_handle);
    }

    template<typename T>
    T& Entity::getComponent() {
        if (!hasComponent<T>()) {
            throw std::runtime_error("Component not found");
        }
        return m_scene->m_registry.get<T>(m_handle);
    }

    template<typename T, typename... Args>
    T& Entity::addComponent(Args&&... args) {
        return m_scene->m_registry.emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void Entity::addEmptyComponent(Args&&... args) {
        m_scene->m_registry.emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
    }

    template<typename T>
    void Entity::removeComponent() {
        m_scene->m_registry.remove<T>(m_handle);
    }

    // REGISTRY IMPLEMENTATION

    template<typename T>
    Entity Scene::createTaggedEntity(const char* name) {

        auto entity = createEntity(name);
        entity.addEmptyComponent<T>();

        return entity;

    }

    template<typename T>
    int Scene::countTaggedEntities() {

        auto view = m_registry.view<T>();
        return view.size();

    }

}