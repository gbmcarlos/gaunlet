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

    int Entity::getId() {
        return (int) m_handle;
    }

    Entity Entity::createChild() {

        // Create the child entity, delegating on the registry (so it will attach the Relationship component)
        auto child = m_registry->createEntity();

        auto& parentRelationship = getComponent<RelationshipComponent>();
        auto& childRelationship = child.getComponent<RelationshipComponent>();

        childRelationship.m_parent = m_handle; // Tell the child about its parent (aka, this)
        parentRelationship.m_children.push_back(child.m_handle); // Tell the parent (aka, this) about its new child

        return child;

    }

    Entity Entity::getParent() {

        RelationshipComponent childRelationship = getComponent<RelationshipComponent>();
        Entity parent(childRelationship.m_parent, m_registry);

        return parent;
    }

    // REGISTRY IMPLEMENTATION

    Entity Registry::createEntity() {

        entt::entity entityHandle = m_registry.create();
        Entity entity(entityHandle, this);
        entity.addComponent<RelationshipComponent>();

        return entity;
    }

}