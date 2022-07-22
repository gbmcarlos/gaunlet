#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "../renderer/Renderer.h"

namespace engine {

    Entity Scene::createEntity() {

        entt::entity entityHandle = m_registry.create();
        return {entityHandle, this};

    }

    void Scene::onUpdate(TimeStep timeStep) {

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        renderPolygons(timeStep);
        renderCircles(timeStep);

        engine::Renderer::flushPolygons();
        engine::Renderer::flushCircles();

    }

    void Scene::renderPolygons(TimeStep timeStep) {

        auto group = m_registry.group<PolygonComponent>(entt::get<TransformComponent>);
        for (auto entity : group) {

            auto [polygon, transform] = group.get<PolygonComponent, TransformComponent>(entity);

            // MaterialComponent is optional
            auto material = m_registry.all_of<MaterialComponent>(entity) ? m_registry.get<MaterialComponent>(entity) : MaterialComponent();

            Renderer::submit(polygon, transform, material);

        }

    }

    void Scene::renderCircles(TimeStep timeStep) {

        auto group = m_registry.group<CircleComponent>(entt::get<TransformComponent>);
        for (auto entity : group) {

            auto [circle, transform] = group.get<CircleComponent, TransformComponent>(entity);

            // MaterialComponent is optional
            auto material = m_registry.all_of<MaterialComponent>(entity) ? m_registry.get<MaterialComponent>(entity) : MaterialComponent();

            Renderer::submit(circle, transform, material);

        }

    }

}