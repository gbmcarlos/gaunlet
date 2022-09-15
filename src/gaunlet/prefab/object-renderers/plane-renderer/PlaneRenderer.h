#pragma once

#include "gaunlet/scene/renderer/ObjectRenderer.h"
#include "gaunlet/prefab/object-renderers/model-renderer/ModelRenderer.h"

#include "gaunlet/pch.h"

namespace gaunlet::Prefab::ObjectRenderers {

    class PlaneRenderer: public Scene::ObjectRenderer<Scene::PlaneComponent, ModelEntityProperties> {

    public:

        PlaneRenderer(unsigned int uniformBufferBindingPoint)
            : Scene::ObjectRenderer<Scene::PlaneComponent, ModelEntityProperties>("EntityPropertySets", uniformBufferBindingPoint, {100000, 600000, 10, 100}) {
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

            std::map<Core::ShaderType, std::string> facesSources {
                {Core::ShaderType::Vertex, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/vertex.glsl"},
                {Core::ShaderType::Fragment, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-faces/fragment.glsl"}
            };

            loadShader(facesSources, "plane-faces", m_shaderLibrary, m_propertySetsUniformBuffer, m_renderer.getMaxTextures());

            // WIREFRAME SHADER
            std::map<Core::ShaderType, std::string> wireframeSources {
                {Core::ShaderType::Vertex, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-wireframe/vertex.glsl"},
                {Core::ShaderType::Geometry, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-wireframe/geometry.glsl"},
                {Core::ShaderType::Fragment, PREFABS_PATH"/object-renderers/plane-renderer/shaders/plane-wireframe/fragment.glsl"}
            };

            loadShader(wireframeSources, "plane-wireframe", m_shaderLibrary, m_propertySetsUniformBuffer, m_renderer.getMaxTextures());

        }

        Graphics::ShaderLibrary m_shaderLibrary;

    };

}