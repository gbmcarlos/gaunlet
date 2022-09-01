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

    void Camera::addYaw(float yawDelta) {
        setYaw(m_yaw + yawDelta);
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

    void Camera::addPitch(float pitchDelta) {
        setPitch(m_pitch + pitchDelta);
    }

    void Camera::lookAt(const glm::vec3 &target) {

        m_viewMatrix = glm::lookAt(
            m_position,
            target,
            {0, 1, 0}
        );

        reverseViewMatrix();

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

    void Camera::reverseViewMatrix() {

        const glm::mat4 inverted = glm::inverse(m_viewMatrix);
        const glm::vec3 direction = -glm::vec3(inverted[2]);

        auto yaw = glm::degrees(glm::atan(direction.z, direction.x));
        auto pitch = glm::degrees(glm::asin(direction.y));

        m_yaw = yaw;
        m_pitch = pitch;

    }

    void Camera::calculateViewMatrix() {

        // Direction
        glm::vec3 direction = {
            cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
            sin(glm::radians(m_pitch)),
            sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
        };
        direction = glm::normalize(direction);

        // Right
        glm::vec3 right = glm::normalize(glm::cross(
            direction,
            {0, 1, 0}
        ));

        // Up
        glm::vec3 up = glm::normalize(glm::cross(
            right,
            direction
        ));

        // Look At
        m_viewMatrix = glm::lookAt(
            m_position,
            m_position + direction,
            up
        );

    }

}