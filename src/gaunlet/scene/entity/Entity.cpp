#include "Entity.h"

namespace gaunlet::Scene {

    // ENTITY IMPLEMENTATION

    int Entity::getId() {
        return (int) m_handle;
    }

    Entity Entity::createChild(Entity &parent) {

        auto child = m_registry->createEntity();

        child.m_parent = parent.m_handle; // Tell the child about its parent
        parent.m_children.push_back(child.m_handle); // Tell the parent about its new child

        return child;

    }

    Entity Entity::getParent() {
        return {m_parent, m_registry};
    }

    // REGISTRY IMPLEMENTATION

    Entity Registry::createEntity() {
        entt::entity entityHandle = m_registry.create();
        return {entityHandle, this};
    }

}