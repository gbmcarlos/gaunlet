#include "Entity.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    // ENTITY IMPLEMENTATION

    Entity::Entity() : m_handle(entt::null), m_scene(nullptr) {}

    Entity::Entity(entt::entity entityHandle, const Core::Ref<Scene>& scene)
        : m_handle(entityHandle), m_scene(scene) {
        if (scene == nullptr) {
            m_handle = entt::null;
            m_scene = nullptr;
        }
    }

    Entity::Entity(int entityHandle, const Core::Ref<Scene>& scene) {
        if (entityHandle < 0 || scene == nullptr) {
            m_handle = entt::null;
            m_scene = nullptr;
        } else {
            m_handle = (entt::entity) (unsigned int) entityHandle;
            m_scene = scene;
        }

    }

    int Entity::getId() const {
        if (m_handle == entt::null) {
            return -1;
        }
        return (int) m_handle;
    }

    bool Entity::hasName() {
        auto& nameComponent = getComponent<NameComponent>();
        return nameComponent.m_name != nullptr;
    }

    const char* Entity::getName() {
        return getComponent<NameComponent>().m_name;
    }

    const Core::Ref<Scene>& Entity::getScene() {
        return m_scene;
    }

    Entity::operator bool() const {
        return m_handle != entt::null && m_scene != nullptr && m_scene->m_registry.valid(m_handle);
    }

    bool Entity::operator==(const Entity &other) const {
        return other.m_handle == m_handle && other.m_scene == m_scene;
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

        auto& childRelationship = getComponent<RelationshipComponent>();
        Entity parent(childRelationship.m_parent, m_scene);

        return parent;
    }

    Entity Entity::createChild(const char* name) {

        // Create the child entity, delegating on the registry (so it will attach the Relationship component)
        auto child = m_scene->createEntity();
        adopt(*this, child);
        return child;

    }

    Entity Entity::addChild(Entity child) {

        adopt(*this, child);
        return child;

    }

    void Entity::destroy() {

        auto& relationship = getComponent<RelationshipComponent>();

        // First abandon and destroy all the children
        while (!relationship.m_children.empty()) {

            Entity child = {relationship.m_children[0], m_scene};
            destroyChild(child);

        }

        Entity parent = {relationship.m_parent, m_scene};

        // If this entity has been abandoned first, there will be no parent
        if (parent) {
            abandon(parent, *this); // "Emancipate", I guess
        }

        // Then destroy itself
        m_scene->m_registry.destroy(m_handle);

    }

    void Entity::destroyChild(Entity child) {

        abandon(*this, child);
        child.destroy();

    }

    void Entity::adopt(Entity& parent, Entity& child) {

        auto& parentRelationship = parent.getComponent<RelationshipComponent>();
        auto& childRelationship = child.getComponent<RelationshipComponent>();

        childRelationship.m_parent = m_handle; // Tell the child about its parent (aka, this)
        parentRelationship.m_children.push_back(child.m_handle); // Tell the parent (aka, this) about its new child

    }

    void Entity::abandon(Entity &parent, Entity &child) {

        auto& parentRelationship = parent.getComponent<RelationshipComponent>();
        auto& childRelationship = child.getComponent<RelationshipComponent>();

        childRelationship.m_parent = entt::null;
        parentRelationship.m_children.erase(std::find(parentRelationship.m_children.begin(), parentRelationship.m_children.end(), child.m_handle));

    }

    // REGISTRY IMPLEMENTATION

    Entity Scene::createEntity(const char* name) {

        entt::entity entityHandle = m_registry.create();
        Entity entity(entityHandle, shared_from_this());

        entity.addComponent<RelationshipComponent>();
        entity.addComponent<NameComponent>(name);

        if (name != nullptr) {
            m_entitiesByName[name] = entityHandle;
        }

        return entity;
    }

    Entity Scene::getEntity(const char *name) {

        auto iterator = m_entitiesByName.find(name);

        if (iterator == m_entitiesByName.end()) {
            return {};
        }

        return {iterator->second, shared_from_this()};

    }

    int Scene::countEntities() {
        return m_registry.alive();
    }

}