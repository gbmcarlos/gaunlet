#pragma once

#include "gaunlet/scene/renderer/ObjectRenderer.h"
#include "gaunlet/prefab/components/CircleComponent.h"

#include "gaunlet/pch.h"

namespace gaunlet::Prefab::ObjectRenderers {

    struct CircleEntityProperties {

        CircleEntityProperties(int entityId, const glm::mat4& transform, glm::vec4 color, float thickness, float fade)
            : m_entityId(entityId), m_transform(transform), m_color(color), m_textureIndex(0), m_thickness(thickness), m_fade(fade) {}

        // The order of these properties is optimized to minimize required padding when using this data in a uniform buffer
        glm::mat4 m_transform;
        glm::vec4 m_color;
        unsigned int m_textureIndex;
        int m_entityId;
        float m_thickness;
        float m_fade;

    };

    class CircleRenderer: public Scene::ObjectRenderer<Prefab::Components::CircleComponent, CircleEntityProperties> {

    public:

        CircleRenderer(unsigned int uniformBufferBindingPoint)
            : Scene::ObjectRenderer<Prefab::Components::CircleComponent, CircleEntityProperties>("EntityPropertySets", uniformBufferBindingPoint, {100000, 600000, 10, 100}) {
            loadShaders();
        }

        inline Graphics::ShaderLibrary& getShaders() {return m_shaderLibrary; }

    protected:

        CircleEntityProperties getEntityProperties(Scene::Entity entity) override {

            auto circle = entity.getComponent<Prefab::Components::CircleComponent>();

            // MaterialComponent is optional
            auto material = entity.hasComponent<Scene::MaterialComponent>() ? entity.getComponent<Scene::MaterialComponent>() : Scene::MaterialComponent();

            return {
                entity.getId(),
                getHierarchicalTransform(entity),
                material.m_color,
                circle.m_thickness,
                circle.m_fade
            };

        }

    private:

        void loadShaders() {

            // FACES SHADER
            std::map<Core::ShaderType, std::string> facesSources {
                {Core::ShaderType::Vertex, PREFABS_PATH"/object-renderers/circle-renderer/shaders/circle-faces/vertex.glsl"},
                {Core::ShaderType::Fragment, PREFABS_PATH"/object-renderers/circle-renderer/shaders/circle-faces/fragment.glsl"}
            };

            loadShader(facesSources, "circle-faces", m_shaderLibrary, m_propertySetsUniformBuffer);

        }

        Graphics::ShaderLibrary m_shaderLibrary;

    };

}