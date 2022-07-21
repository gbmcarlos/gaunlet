#pragma once

#include "../graphics/mesh/Mesh.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    struct TransformComponent {

        TransformComponent() : m_position(glm::vec3(0.0f, 0.0f, 0.0f)), m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)), m_scale(glm::vec3(1.0f, 1.0f, 1.0f)) {}
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(glm::vec3 position,  glm::vec3 rotation, glm::vec3 scale) : m_position(position), m_rotation(rotation), m_scale(scale) {}

        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        glm::mat4 getTransformationMatrix() {

            glm::mat4 translationsMatrix = glm::translate(glm::mat4(1.0f), m_position);

            glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), {1, 0, 0});
            glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), {0, 1, 0});
            glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), {0, 0, 1});

            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

            return translationsMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;
        }

    };

    struct MeshComponent {

        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;
        MeshComponent(Mesh& mesh) : m_mesh(mesh) {}

        Mesh m_mesh;

    };

    struct MaterialComponent {

        MaterialComponent() = default;
        MaterialComponent(const MaterialComponent&) = default;
        MaterialComponent(glm::vec4 color) : m_color(color) {}

        glm::vec4 m_color;

    };

    struct SpeedComponent {

        SpeedComponent() = default;
        SpeedComponent(const SpeedComponent&) = default;
        SpeedComponent(glm::vec3 speed) : m_speed(speed) {}

        glm::vec3 m_speed;

    };

}