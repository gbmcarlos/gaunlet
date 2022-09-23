#pragma once

#include "gaunlet/graphics/Vertex.h"
#include "gaunlet/graphics/texture/TextureCubeMap.h"

namespace gaunlet::Scene {

    class ContentComponent {

        virtual std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() = 0;

    };

    struct TransformComponent {

        TransformComponent() : m_translation(glm::vec3(0.0f, 0.0f, 0.0f)), m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)), m_scale(glm::vec3(1.0f, 1.0f, 1.0f)) {}
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(glm::vec3 position,  glm::vec3 rotation, glm::vec3 scale) : m_translation(position), m_rotation(rotation), m_scale(scale) {}

        glm::vec3 m_translation;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        glm::mat4 getTransformationMatrix() const {

            glm::mat4 translationsMatrix = glm::translate(glm::mat4(1.0f), m_translation);

            glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), {1, 0, 0});
            glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), {0, 1, 0});
            glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), {0, 0, 1});

            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

            return translationsMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;
        }

    };

    struct MaterialComponent {

        MaterialComponent(const MaterialComponent&) = default;
        MaterialComponent() : m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), m_texture(nullptr) {}
        explicit MaterialComponent(glm::vec4 color) : m_color(color), m_texture(nullptr) {}
        explicit MaterialComponent(Core::Ref<Graphics::Texture> texture) : m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), m_texture(std::move(texture)) {}
        MaterialComponent(glm::vec4 color, Core::Ref<Graphics::Texture> texture) : m_color(color), m_texture(std::move(texture)) {}

        glm::vec4 m_color;
        Core::Ref<Graphics::Texture> m_texture;

    };

    struct DirectionalLightComponent {

        DirectionalLightComponent()
            : m_color(glm::vec3(1.0f, 1.0f, 1.0f)), m_direction(glm::vec3(0.0f, 0.0f, 0.0f)), m_ambientIntensity(1.0f), m_diffuseIntensity(1.0f) {}

        DirectionalLightComponent(glm::vec3 color, glm::vec3 direction, float ambientIntensity, float diffuseIntensity)
            : m_color(color), m_ambientIntensity(ambientIntensity), m_direction(direction), m_diffuseIntensity(diffuseIntensity) {}

        glm::vec3 m_color;
        glm::vec3 m_direction;
        float m_ambientIntensity;
        float m_diffuseIntensity;

    };

    struct SkyboxComponent {

        SkyboxComponent()
            : m_cubeMap(nullptr) {}
        explicit SkyboxComponent(Core::Ref<Graphics::TextureCubeMap> cubeMap)
            : m_cubeMap(std::move(cubeMap)) {}
        explicit SkyboxComponent(const std::vector<const char*>& filePaths)
            : m_cubeMap(Core::CreateRef<Graphics::TextureCubeMap>(filePaths)) {}

        Core::Ref<Graphics::TextureCubeMap> m_cubeMap = nullptr;

    };

}