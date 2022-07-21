#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "../graphics/renderer/Renderer.h"

namespace engine {

    Entity Scene::createEntity() {

        entt::entity entityHandle = m_registry.create();
        return {entityHandle, this};

    }

    void Scene::onUpdate(TimeStep timeStep) {

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        engine::Renderer::beginBatch(engine::MeshElementType::Face);

        auto group = m_registry.group<MeshComponent>(entt::get<TransformComponent, MaterialComponent>);
        for (auto entity : group) {

            auto [mesh, transform, material] = group.get<MeshComponent, TransformComponent, MaterialComponent>(entity);
            Renderer::submit(mesh.m_mesh, transform.getTransformationMatrix(), material.m_color);

        }

        engine::Renderer::flushBatch();

    }

}