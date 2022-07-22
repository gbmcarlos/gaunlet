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

        auto group = m_registry.group<MeshComponent>(entt::get<TransformComponent>);
        for (auto entity : group) {

            auto [mesh, transform] = group.get<MeshComponent, TransformComponent>(entity);

            // MaterialComponent is optional
            glm::vec4 color;
            std::shared_ptr<Texture> texture;
            if (m_registry.all_of<MaterialComponent>(entity)) {
                auto& material = m_registry.get<MaterialComponent>(entity);
                color = material.m_color;
                texture = material.m_texture;
            } else {
                color = glm::vec4(1.0f);
                texture = nullptr;
            }

            Renderer::submit(mesh.m_mesh, transform.getTransformationMatrix(), color, texture);

        }

        engine::Renderer::flush();

    }

}