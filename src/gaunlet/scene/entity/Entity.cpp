#include "Entity.h"

namespace gaunlet::Scene {

    // ENTITY IMPLEMENTATION

    Entity::Entity() : m_handle(entt::null), m_registry(nullptr) {}

    Entity::Entity(entt::entity entityHandle, Registry *registry) : m_handle(entityHandle), m_registry(registry) {}

    Entity::Entity(int entityHandle, Registry *registry)
        : m_handle((entt::entity) entityHandle), m_registry(registry) {
        if (getId() < 0 || registry == nullptr) {
            m_handle = entt::null;
            registry = nullptr;
        }
    }

    int Entity::getId() const {
        if (m_handle == entt::null) {
            return -1;
        }
        return (int) m_handle;
    }

    Entity::operator bool() const {
        return m_handle != entt::null && m_registry != nullptr;
    }

    bool Entity::operator==(const Entity &other) const {
        return other.m_handle == m_handle && other.m_registry == m_registry;
    }

    bool Entity::operator!=(const Entity &other) const {

        // If both of them are invalid (empty, null), then they're the same
        // If e.g. one was null handle, and the other has null registry, they would be considered different, but they're both invalid
        if (!other && !*this) {
            return false; // Not different, the same
        }

        return !operator==(other);

    }

    Entity Entity::getParent() {

        RelationshipComponent childRelationship = getComponent<RelationshipComponent>();
        Entity parent(childRelationship.m_parent, m_registry);

        return parent;
    }

    Entity Entity::createChild() {

        // Create the child entity, delegating on the registry (so it will attach the Relationship component)
        auto child = m_registry->createEntity();
        adopt(*this, child);
        return child;

    }

    void Entity::adopt(Entity& parent, Entity& child) {

        auto& parentRelationship = getComponent<RelationshipComponent>();
        auto& childRelationship = child.getComponent<RelationshipComponent>();

        childRelationship.m_parent = m_handle; // Tell the child about its parent (aka, this)
        parentRelationship.m_children.push_back(child.m_handle); // Tell the parent (aka, this) about its new child

    }

    // REGISTRY IMPLEMENTATION

    Entity Registry::createEntity() {

        entt::entity entityHandle = m_registry.create();
        Entity entity(entityHandle, this);
        entity.addComponent<RelationshipComponent>();

        return entity;
    }

}