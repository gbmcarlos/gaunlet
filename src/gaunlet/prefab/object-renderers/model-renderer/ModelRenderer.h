#pragma once

#include "gaunlet/scene/renderer/ObjectRenderer.h"

#include "gaunlet/pch.h"

namespace gaunlet::Prefab::ObjectRenderers {

    struct ModelEntityProperties {

        ModelEntityProperties(int entityId, const glm::mat4& transform, glm::vec4 color)
            : m_entityId(entityId), m_transform(transform), m_color(color), m_textureIndex(0) {}

        // The order of these properties is optimized to minimize required padding when using this data in a uniform buffer
        glm::mat4 m_transform;
        glm::vec4 m_color;
        unsigned int m_textureIndex;
        int m_entityId; glm::vec2 pad2 = {};

    };

    class ModelRenderer : public Scene::ObjectRenderer<Scene::ModelComponent, ModelEntityProperties> {

    public:

        ModelRenderer(unsigned int uniformBufferBindingPoint)
            : Scene::ObjectRenderer<Scene::ModelComponent, ModelEntityProperties>("EntityPropertySets", uniformBufferBindingPoint, {100000, 600000, 10, 100}) {
            loadShaders();
        }

        inline Graphics::ShaderLibrary& getShaders() {return m_shaderLibrary; }

    protected:

        ModelEntityProperties getEntityProperties(Scene::Entity entity) override {

            auto material = entity.hasComponent<Scene::MaterialComponent>() ? entity.getComponent<Scene::MaterialComponent>() : Scene::MaterialComponent();

            return {
                entity.getId(),
                getHierarchicalTransform(entity),
                material.m_color
            };

        }

    private:

        void loadShaders() {

            // FACES SHADER
            std::map<Core::ShaderType, std::string> facesSources {
                {Core::ShaderType::Vertex, PREFABS_PATH"/object-renderers/model-renderer/shaders/model-faces/vertex.glsl"},
                {Core::ShaderType::Fragment, PREFABS_PATH"/object-renderers/model-renderer/shaders/model-faces/fragment.glsl"}
            };

            loadShader(facesSources, "model-faces", m_shaderLibrary, m_propertySetsUniformBuffer, m_renderer.getMaxTextures());

            // OUTLINE SHADER
            std::map<Core::ShaderType, std::string> outlineSources {
                {Core::ShaderType::Vertex, PREFABS_PATH"/object-renderers/model-renderer/shaders/model-outline/vertex.glsl"},
                {Core::ShaderType::Fragment, PREFABS_PATH"/object-renderers/model-renderer/shaders/model-outline/fragment.glsl"}
            };

            auto outlineShader = loadShader(outlineSources, "model-outline", m_shaderLibrary, m_propertySetsUniformBuffer, m_renderer.getMaxTextures());
            outlineShader->setUniform1f("u_width", 0.015f);
            outlineShader->setUniform3f("u_color", {0.9f, 0.9f, 0.1f});

            // WIREFRAME SHADER
            std::map<Core::ShaderType, std::string> wireframeSources {
                {Core::ShaderType::Vertex, PREFABS_PATH"/object-renderers/model-renderer/shaders/model-wireframe/vertex.glsl"},
                {Core::ShaderType::Geometry, PREFABS_PATH"/object-renderers/model-renderer/shaders/model-wireframe/geometry.glsl"},
                {Core::ShaderType::Fragment, PREFABS_PATH"/object-renderers/model-renderer/shaders/model-wireframe/fragment.glsl"}
            };

            loadShader(wireframeSources, "model-wireframe", m_shaderLibrary, m_propertySetsUniformBuffer, m_renderer.getMaxTextures());

        }

        Graphics::ShaderLibrary m_shaderLibrary;

    };

}