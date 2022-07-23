#include "Scene.h"

#include "entity/Entity.h"
#include "entity/GraphicsComponents.h"
#include "renderer/Renderer.h"

namespace engine {

    Entity Scene::createEntity() {
        entt::entity entityHandle = m_registry.create();
        return {entityHandle, this};
    }

    void Scene::onUpdate(TimeStep timeStep) {

        renderScene();

    }

    void Scene::renderScene() {

        renderPolygons();
        renderCircles();

    }

    void Scene::renderPolygons() {

        auto group = m_registry.group<PolygonComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};
            auto [polygon, transform] = group.get<PolygonComponent, TransformComponent>(e);

            // MaterialComponent is optional
            auto material = entity.hasComponent<MaterialComponent>() ? entity.getComponent<MaterialComponent>() : MaterialComponent();

            Renderer::submit(polygon, transform, material);

        }

    }

    void Scene::renderCircles() {

        auto group = m_registry.group<CircleComponent>(entt::get<TransformComponent>);
        for (auto e : group) {

            Entity entity = {e, this};
            auto [circle, transform] = group.get<CircleComponent, TransformComponent>(e);

            // MaterialComponent is optional
            auto material = entity.hasComponent<MaterialComponent>() ? entity.getComponent<MaterialComponent>() : MaterialComponent();

            Renderer::submit(circle, transform, material);

        }

    }

}