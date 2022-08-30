#pragma once

#include "gaunlet/scene/model/Model.h"
#include "gaunlet/scene/model/Sprites.h"
#include "gaunlet/graphics/texture/Texture.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

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

    struct ModelComponent {

        ModelComponent(const ModelComponent&) = default;
        ModelComponent(Model model) : m_model(std::move(model)) {}

        Model m_model;

    };

    struct CircleComponent {

        CircleComponent(const CircleComponent&) = default;

        // A circle is drawn as a square (a single mesh), with a different shader
        CircleComponent() :
            m_mesh(Square2DModel().getMeshes()[0]),
            m_thickness(0.5f),
            m_fade(0.01f) {}

        CircleComponent(float thickness) :
            m_mesh(Square2DModel().getMeshes()[0]),
            m_thickness(thickness),
            m_fade(0.01f){}

        CircleComponent(float thickness, float fade) :
            m_mesh(Square2DModel().getMeshes()[0]),
            m_thickness(thickness),
            m_fade(fade) {}

        Mesh m_mesh;
        float m_thickness;
        float m_fade;

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

}