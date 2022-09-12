#pragma once

#include "gaunlet/scene/renderer/ObjectRenderer.h"
#include "gaunlet/prefab/render-pipelines/basic-editor-render-pipeline/PropertySets.h"

namespace gaunlet::Prefab::BasicEditorRenderPipeline {

    class ModelRenderer : public Scene::ObjectRenderer<Scene::ModelComponent, ModelEntityProperties> {

    public:

        ModelRenderer(const char* uniformBufferName, unsigned int uniformBufferBindingPoint)
            : Scene::ObjectRenderer<Scene::ModelComponent, ModelEntityProperties>(uniformBufferName, uniformBufferBindingPoint, {100000, 600000, 10, 100}) {}

    protected:

        ModelEntityProperties getEntityProperties(Scene::Entity entity) override {

            auto material = entity.hasComponent<Scene::MaterialComponent>() ? entity.getComponent<Scene::MaterialComponent>() : Scene::MaterialComponent();

            return {
                entity.getId(),
                getHierarchicalTransform(entity),
                material.m_color
            };

        }

    };

    class CircleRenderer: public Scene::ObjectRenderer<Scene::CircleComponent, CircleEntityProperties> {

    public:

        CircleRenderer(const char* uniformBufferName, unsigned int uniformBufferBindingPoint)
            : Scene::ObjectRenderer<Scene::CircleComponent, CircleEntityProperties>(uniformBufferName, uniformBufferBindingPoint, {100000, 600000, 10, 100}) {}

    protected:

        CircleEntityProperties getEntityProperties(Scene::Entity entity) override {

            auto circle = entity.getComponent<Scene::CircleComponent>();

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

    };

}