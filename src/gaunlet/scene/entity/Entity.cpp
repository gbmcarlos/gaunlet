#include "Entity.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    // ENTITY IMPLEMENTATION

    Entity::Entity() : m_handle(entt::null), m_registry(nullptr) {}

    Entity::Entity(entt::entity entityHandle, Registry *registry)
        : m_handle(entityHandle), m_registry(registry) {
        if (registry == nullptr) {
            m_handle = entt::null;
            m_registry = nullptr;
        }
    }

    Entity::Entity(int entityHandle, Registry *registry) {
        if (entityHandle < 0 || registry == nullptr) {
            m_handle = entt::null;
            m_registry = nullptr;
        } else {
            m_handle = (entt::entity) (unsigned int) entityHandle;
            m_registry = registry;
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

    Entity Entity::addChild(Entity child) {

        adopt(*this, child);
        return child;

    }

    void Entity::destroy() {

        RelationshipComponent relationship = getComponent<RelationshipComponent>();

        // First abandon and destroy all the children
        for (auto childHandle : relationship.m_children) {

            Entity child = {childHandle, m_registry};
            destroyChild(child);

        }

        Entity parent = {relationship.m_parent, m_registry};

        // If this entity has been abandoned first, there will be no parent
        if (parent) {
            abandon(parent, *this); // "Emancipate", I guess
        }

        // Then destroy itself
        m_registry->m_registry.destroy(m_handle);

    }

    void Entity::destroyChild(Entity child) {

        abandon(*this, child);
        child.destroy();

    }

    void Entity::adopt(Entity& parent, Entity& child) {

        auto& parentRelationship = getComponent<RelationshipComponent>();
        auto& childRelationship = child.getComponent<RelationshipComponent>();

        childRelationship.m_parent = m_handle; // Tell the child about its parent (aka, this)
        parentRelationship.m_children.push_back(child.m_handle); // Tell the parent (aka, this) about its new child

    }

    void Entity::abandon(Entity &parent, Entity &child) {

        auto& parentRelationship = getComponent<RelationshipComponent>();
        auto& childRelationship = child.getComponent<RelationshipComponent>();

        childRelationship.m_parent = entt::null;
        parentRelationship.m_children.erase(std::find(parentRelationship.m_children.begin(), parentRelationship.m_children.end(), child.m_handle));

    }

    // REGISTRY IMPLEMENTATION

    Entity Registry::createEntity() {

        entt::entity entityHandle = m_registry.create();
        Entity entity(entityHandle, this);
        entity.addComponent<RelationshipComponent>();

        return entity;
    }

    int Registry::countEntities() {
        return m_registry.alive();
    }

}