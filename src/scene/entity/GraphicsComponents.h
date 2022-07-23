#pragma once

#include "../mesh/PolygonMesh.h"
#include "../mesh/CircleMesh.h"
#include "../../graphics/texture/Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <utility>

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

    struct PolygonComponent {

        PolygonComponent() = default;
        PolygonComponent(const PolygonComponent&) = default;
        PolygonComponent(PolygonMesh mesh) : m_mesh(std::move(mesh)) {}

        PolygonMesh m_mesh;

    };

    struct CircleComponent {

        CircleComponent(const CircleComponent&) = default;

        CircleComponent() :
            m_thickness(0.5f),
            m_fade(0.01f) {}

        CircleComponent(float thickness) :
            m_thickness(thickness),
            m_fade(0.01f){}

        CircleComponent(float thickness, float fade) :
            m_thickness(thickness),
            m_fade(fade) {}

        CircleMesh m_mesh;
        float m_thickness;
        float m_fade;

    };

    struct MaterialComponent {

        MaterialComponent(const MaterialComponent&) = default;
        MaterialComponent() : m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), m_texture(nullptr) {}
        MaterialComponent(glm::vec4 color) : m_color(color), m_texture(nullptr) {}
        MaterialComponent(std::shared_ptr<Texture> texture) : m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), m_texture(std::move(texture)) {}
        MaterialComponent(glm::vec4 color, std::shared_ptr<Texture> texture) : m_color(color), m_texture(std::move(texture)) {}

        glm::vec4 m_color;
        std::shared_ptr<Texture> m_texture;

    };

}