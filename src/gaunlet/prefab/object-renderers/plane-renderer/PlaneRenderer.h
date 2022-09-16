#pragma once

#include "gaunlet/scene/renderer/ObjectRenderer.h"
#include "gaunlet/prefab/object-renderers/model-renderer/ModelRenderer.h"
#include "gaunlet/graphics/renderer/DirectRenderer.h"

#include "gaunlet/pch.h"

namespace gaunlet::Prefab::ObjectRenderers {

    struct Vertex {

    };

    struct PlaneEntityProperties {

        PlaneEntityProperties(int entityId, const glm::mat4& transform, glm::vec4 color)
            : m_entityId(entityId), m_transform(transform), m_color(color), m_textureIndex(0) {}

        // The order of these properties is optimized to minimize required padding when using this data in a uniform buffer
        glm::mat4 m_transform;
        glm::vec4 m_color;
        unsigned int m_textureIndex;
        int m_entityId; glm::vec2 pad2 = {};

    };

    class PlaneRenderer: public Scene::ObjectRenderer<Scene::PlaneComponent, PlaneEntityProperties> {

    public:

        PlaneRenderer(unsigned int uniformBufferBindingPoint)
            : Scene::ObjectRenderer<Scene::PlaneComponent, PlaneEntityProperties>("EntityPropertySets", uniformBufferBindingPoint, {100000, 600000, 10, 100}) {
            loadShaders();
        }

        inline Graphics::ShaderLibrary& getShaders() {return m_shaderLibrary; }

    protected:

        PlaneEntityProperties getEntityProperties(Scene::Entity entity) override {

            auto material = entity.hasComponent<Scene::MaterialComponent>() ? entity.getComponent<Scene::MaterialComponent>() : Scene::MaterialComponent();

            return {
                entity.getId(),
                getHierarchicalTransform(entity),
                material.m_color
            };

        }

        Graphics::RenderMode getRenderMode() override {
            return Graphics::RenderMode::Quad;
        }

    private:

        void loadShaders() {

            std::map<Core::ShaderType, std::string> facesSources {
                {Core::ShaderType::Vertex, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/vertex.glsl"},
                {Core::ShaderType::TessellationControl, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/tessellation-control.glsl"},
                {Core::ShaderType::TessellationEvaluation, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/tessellation-evaluation.glsl"},
                {Core::ShaderType::Fragment, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/fragment.glsl"}
            };

            loadShader(facesSources, "plane-faces", m_shaderLibrary, m_propertySetsUniformBuffer, m_renderer.getMaxTextures());

        }

        Graphics::ShaderLibrary m_shaderLibrary;

    };

}