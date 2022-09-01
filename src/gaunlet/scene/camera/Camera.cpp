#include "gaunlet/scene/camera/Camera.h"

namespace gaunlet::Scene {

    glm::mat4 Camera::getProjectionMatrix() {
        return m_projectionMatrix;
    }

    glm::mat4 Camera::getViewMatrix() {
        return m_viewMatrix;
    }

    void Camera::setPosition(const glm::vec3& position) {
        m_position = position;
        calculateViewMatrix();
    }

    void Camera::setYaw(float yaw) {
        m_yaw = yaw;
        calculateViewMatrix();
    }

    void Camera::setPitch(float pitch) {

        // Restrict the pitch, to avoid flipping
        if (pitch > 89) {
            pitch = 89;
        } else if (pitch < -89) {
            pitch = -89;
        }

        m_pitch = pitch;
        calculateViewMatrix();

    }

    void Camera::lookAt(const glm::vec3 &target) {

        m_viewMatrix = glm::lookAt(
            m_position,
            target,
            {0, 1, 0}
        );

    }

    void Camera::lookAt(const TransformComponent &transform) {
        lookAt(transform.m_translation);
    }

    void Camera::setZoomLevel(float zoomLevel) {
        m_zoomLevel = zoomLevel;
        calculateProjectionMatrix();
    }

    void Camera::addZoomLevel(float zoomLevelDelta) {
        m_zoomLevel += zoomLevelDelta;
        calculateProjectionMatrix();
    }

    void Camera::calculateViewMatrix() {

        // Front
        glm::vec3 front = {
            cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
            sin(glm::radians(m_pitch)),
            sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
        };
        front = glm::normalize(front);

        // Right
        glm::vec3 right = glm::normalize(glm::cross(
            front,
            {0, 1, 0}
        ));

        // Up
        glm::vec3 up = glm::normalize(glm::cross(
            right,
            front
        ));

        // Look At
        m_viewMatrix = glm::lookAt(
            m_position,
            m_position + front,
            up
        );

    }

}