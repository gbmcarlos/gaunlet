#pragma once

#include "../model/Model.h"
#include "../model/circle-mesh/CircleMesh.h"
#include "../../graphics/texture/Texture.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

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

    struct PolygonModelComponent {

        PolygonModelComponent() = default;
        PolygonModelComponent(const PolygonModelComponent&) = default;
        PolygonModelComponent(Model model) : m_model(std::move(model)) {}

        Model m_model;

    };

    struct CircleModelComponent {

        CircleModelComponent(const CircleModelComponent&) = default;

        CircleModelComponent() :
            m_thickness(0.5f),
            m_fade(0.01f) {}

        CircleModelComponent(float thickness) :
            m_thickness(thickness),
            m_fade(0.01f){}

        CircleModelComponent(float thickness, float fade) :
            m_thickness(thickness),
            m_fade(fade) {}

        CircleMesh m_mesh;
        float m_thickness;
        float m_fade;

    };

    struct MaterialComponent {

        MaterialComponent(const MaterialComponent&) = default;
        MaterialComponent() : m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), m_texture(nullptr) {}
        explicit MaterialComponent(glm::vec4 color) : m_color(color), m_texture(nullptr) {}
        explicit MaterialComponent(Ref<Texture> texture) : m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), m_texture(std::move(texture)) {}
        MaterialComponent(glm::vec4 color, Ref<Texture> texture) : m_color(color), m_texture(std::move(texture)) {}

        glm::vec4 m_color;
        Ref<Texture> m_texture;

    };

}