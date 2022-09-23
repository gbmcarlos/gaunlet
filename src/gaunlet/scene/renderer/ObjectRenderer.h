#pragma once

#include "gaunlet/graphics/render-pass/BatchedRenderPass.h"
#include "gaunlet/graphics/shader/ShaderLibrary.h"
#include "gaunlet/scene/entity/Entity.h"
#include "gaunlet/scene/components/BasicComponents.h"

namespace gaunlet::Scene {

    template<typename T, typename Y>
    class ObjectRenderer {

        static_assert(std::is_base_of<ContentComponent, T>::value, "T must derive from Base");

    public:

        ObjectRenderer(const char* uniformBufferName, unsigned int uniformBufferBindingPoint, const Graphics::BatchParameters& batchParameters);

        void submitObject(Entity entity, const Core::Ref<Graphics::Shader>& shader);
        void renderObjects(const Core::Ref<Graphics::Shader>& shader);
        static unsigned int getUniformBufferCount() {return 1; }

    protected:

        virtual Y getEntityProperties(Entity entity) = 0;
        virtual Graphics::RenderMode getRenderMode() {return Graphics::RenderMode::Triangle; }

        const Core::Ref<Graphics::Shader>& loadShader(const std::map<Core::ShaderType, std::string>& sources, const char *name, Graphics::ShaderLibrary &library, const Core::Ref<Graphics::UniformBuffer>& uniformBuffer);
        glm::mat4 getHierarchicalTransform(Entity entity);

        Graphics::BatchedRenderPass<Y> m_renderPass;
        Core::Ref<Graphics::UniformBuffer> m_propertySetsUniformBuffer = nullptr;

    };

    template<typename T, typename Y>
    ObjectRenderer<T, Y>::ObjectRenderer(const char *uniformBufferName, unsigned int uniformBufferBindingPoint, const Graphics::BatchParameters& batchParameters) : m_renderPass(batchParameters) {

        // Create a uniform buffer that will contain the properties of every object, and will be linked to the shader
        m_propertySetsUniformBuffer = Core::CreateRef<Graphics::UniformBuffer>(
            uniformBufferName,
            uniformBufferBindingPoint,
            sizeof (Y) * batchParameters.m_maxPropertySets
        );

    }

    template<typename T, typename Y>
    void ObjectRenderer<T, Y>::submitObject(Entity entity, const Core::Ref<Graphics::Shader>& shader) {

        if (!entity.hasComponent<T>()) {
            throw std::runtime_error("Entity doesn't have a required component");
        }

        if (!entity.hasComponent<TransformComponent>()) {
            throw std::runtime_error("Entity doesn't have a TransformComponent");
        }

        auto object = entity.getComponent<T>();

        // Get the entity's transform, relative to its parent's (all the way up the chain)
        auto hierarchicalTransform = getHierarchicalTransform(entity);

        // MaterialComponent is optional
        auto material = entity.hasComponent<MaterialComponent>() ? entity.getComponent<MaterialComponent>() : MaterialComponent();

        auto[vertices, indices] = object.getContent();

        auto entityProperties = getEntityProperties(entity);

        bool batched = m_renderPass.submitIndexedTriangles(
            vertices,
            indices,
            material.m_texture,
            entityProperties
        );

        // If this submission was the one that crossed the batch limit, flush, and submit again
        if (!batched) {
            renderObjects(shader);
            m_renderPass.submitIndexedTriangles(
                vertices,
                indices,
                material.m_texture,
                entityProperties
            );
        }

    }

    template<typename T, typename Y>
    void ObjectRenderer<T, Y>::renderObjects(const Core::Ref<Graphics::Shader>& shader) {

        auto& entityPropertySets = m_renderPass.getPropertySets();

        // Submit the entity properties to the uniform buffer
        m_propertySetsUniformBuffer->setData(
            (const void*) entityPropertySets.data(),
            sizeof(Y) * entityPropertySets.size()
        );

        m_renderPass.flush(shader, getRenderMode());

    }

    template<typename T, typename Y>
    const Core::Ref<Graphics::Shader>& ObjectRenderer<T, Y>::loadShader(const std::map<Core::ShaderType, std::string>& sources, const char *name, Graphics::ShaderLibrary &library, const Core::Ref<Graphics::UniformBuffer>& uniformBuffer) {

        auto shader = library.load(name, sources);

        for (int i = 0; i < m_renderPass.getBatchParameters().m_maxTextures; i++) {
            auto textureName = std::string("texture") + std::to_string(i);
            shader->setUniform1i(textureName, i);
        }

        shader->linkUniformBuffer(uniformBuffer);

        return library.get(name);

    }

    template<typename T, typename Y>
    glm::mat4 ObjectRenderer<T, Y>::getHierarchicalTransform(Entity entity) {

        if (!entity.hasComponent<TransformComponent>()) {
            return glm::mat4(1);
        }

        glm::mat4 result = entity.getComponent<TransformComponent>().getTransformationMatrix();
        Entity current = entity;

        while (true) {

            auto parent = current.getParent();

            if (!parent || !parent.hasComponent<TransformComponent>()) {
                break;
            }

            // Multiply with the parent's transform and move to the next generation
            result = parent.getComponent<TransformComponent>().getTransformationMatrix() * result;
            current = parent;

        }

        return result;

    }

}