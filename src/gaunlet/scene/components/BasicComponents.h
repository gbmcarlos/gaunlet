#pragma once

#include "gaunlet/graphics/Vertex.h"
#include "gaunlet/graphics/texture/TextureCubeMap.h"

namespace gaunlet::Scene {

    class ContentComponent {

        virtual std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() = 0;

    };

    struct TransformComponent {

        TransformComponent() : m_translation(glm::vec3(0.0f, 0.0f, 0.0f)), m_rotation(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)), m_scale(glm::vec3(1.0f, 1.0f, 1.0f)) {}
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale) : m_translation(position), m_rotation(rotation), m_scale(scale) {}

        glm::vec3 m_translation;
        glm::vec4 m_rotation;
        glm::vec3 m_scale;

        glm::mat4 getTransformationMatrix() const {

            glm::mat4 translationsMatrix = glm::translate(glm::mat4(1.0f), m_translation);
            glm::mat4 rotationMatrix = glm::mat4_cast(glm::normalize(getRotationQuaternion(m_rotation)));
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

            return translationsMatrix * rotationMatrix * scaleMatrix;
        }

        glm::quat getRotationQuaternion(glm::vec4 rotation) const {

            float angleRadians = glm::radians(rotation.w);
            float cos = glm::cos(angleRadians*0.5f);
            float sin = glm::sin(angleRadians*0.5f);

            glm::vec3 axis = glm::vec3(rotation.x, rotation.y, rotation.z);
            if (glm::length(axis) == 0.0f && angleRadians == 0.0f) {
                axis = {1, 0, 0};
            }
            axis = glm::normalize(axis);

            return {
                cos,
                axis.x * sin,
                axis.y * sin,
                axis.z * sin,
            };

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