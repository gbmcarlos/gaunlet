#pragma once

#include "glm/glm.hpp"

namespace engine {

    class OrthographicCamera {

    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        OrthographicCamera() : OrthographicCamera(-1, 1, -1, 1) {}

        static const glm::mat4& getDefaultViewProjectionMatrix();

        inline const glm::vec3& getPosition() {return m_position;}
        void setPosition(const glm::vec3& position);
        void movePosition(const glm::vec3& positionDelta);

        inline float getRotation() {return m_rotation;}
        void setRotation(float rotation);
        void addRotation(float rotationDelta);

        inline const glm::mat4& getProjectionMatrix() {return m_projectionMatrix;}
        inline const glm::mat4& getViewMatrix() {return m_viewMatrix;}
        inline const glm::mat4& getViewProjectionMatrix() {return m_viewProjectionMatrix;}

    private:
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_viewProjectionMatrix;

        glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
        float m_rotation;

        void calculateViewProjectionMatrix();

    };

}