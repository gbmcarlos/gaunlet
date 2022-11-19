#include "gaunlet/scene/camera/Camera.h"
#include "glm/gtx/vector_angle.hpp"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    Camera::Camera()
        : Camera(1.0f, -10.0f) {}

    Camera::Camera(float near, float far)
        : Camera(near, far, 5.0f) {}

    Camera::Camera(float near, float far, float maxZoomLevel)
        : m_near(near), m_far(far), m_maxZoomLevel(maxZoomLevel) {
        calculateViewMatrix();
    }

    glm::mat4 Camera::getProjectionMatrix() {
        calculateProjectionMatrix();
        return m_projectionMatrix;
    }

    glm::mat4 Camera::getViewMatrix() {
        calculateViewMatrix();
        return m_viewMatrix;
    }

    const Frustum& Camera::getFrustum() {
        calculateFrustum();
        return m_frustum;
    }

    glm::vec3& Camera::getPosition() {
        return m_position;
    }

    void Camera::setPosition(const glm::vec3& position) {
        m_position = position;
        calculateFrustum();
    }

    void Camera::setOrientation(const glm::vec3 &forward, const glm::vec3 &up) {
        m_forward = forward;
        m_up = up;
        m_right = glm::cross(forward, up);
    }

    void Camera::setZoomLevel(float zoomLevel) {
        m_zoomLevel = constrainZoomLevel(zoomLevel);
    }

    void Camera::move(const glm::vec3 &movement) {
        m_position += movement;
        calculateFrustum();
    }

    void Camera::moveRelative(const glm::vec3 &movement) {

        glm::mat3 relativeTransform = glm::mat3(
            m_right,
            m_up,
            -m_forward // The camera's forward point in the direction the camera is looking at, but the Z axis should go backwards
        );

        glm::vec3 relativeMovement = relativeTransform * movement;

        move(relativeMovement);

    }

    void Camera::moveSemiRelative(const glm::vec3 &movement) {

        glm::vec3 up = {0, 1, 0};
        glm::vec3 forward = glm::normalize(glm::cross(up, m_right));

        glm::mat3 relativeTransform = glm::mat3(
            m_right,
            up,
            -forward // The camera's forward point in the direction the camera is looking at, but the Z axis should go backwards
        );

        glm::vec3 relativeMovement = relativeTransform * movement;

        move(relativeMovement);

    }

    void Camera::rotate(glm::vec4 rotation) {

        if (glm::any(glm::isnan(rotation))) {
            int a = 1;
        }

        auto quaternion = getRotationQuaternion(rotation);
        auto inverse = glm::inverse(quaternion);
        m_forward = m_forward * quaternion;
        m_up = m_up * quaternion;
        m_right = glm::cross(m_forward, m_up);
    }

    void Camera::lookAt(glm::vec3 target) {

        glm::vec3 toVector = glm::normalize(target - m_position);

        glm::vec3 rotationAxis = glm::cross(m_forward, toVector);
        float rotationAngle = -acos(glm::dot(toVector, m_forward));

        if (glm::any(glm::isnan(rotationAxis)) || glm::isnan(rotationAngle)) {
            return;
        }

        rotate({rotationAxis, glm::degrees(rotationAngle)});

    }

    void Camera::addZoomLevel(float zoomLevelDelta) {
        m_zoomLevel = constrainZoomLevel(m_zoomLevel + zoomLevelDelta);
    }

    glm::vec3 Camera::ray(glm::vec2 viewportCoordinates, glm::vec2 viewportSize) {

        // Normalized device coordinates (-1:1, -1:1, -1:1)
        float ndcX = (2.0f * viewportCoordinates.x) / viewportSize.x - 1.0f;
        float ndcY = 1.0f - (2.0f * viewportCoordinates.y) / viewportSize.y;
        float ndcZ = 1.0f;
        glm::vec3 ndcRay = {ndcX, ndcY, ndcZ};

        // Homogeneous clip coordinates (point Z forward)
        glm::vec4 clipRay = {ndcRay.x, ndcRay.y, -1.0, 1.0};

        // Camera coordinates ([-x:x, -y:y, -z:z])
        glm::vec4 cameraRayT = glm::inverse(getProjectionMatrix()) * clipRay;
        glm::vec4 cameraRay = {cameraRayT.x, cameraRayT.y, -1.0f, 0.0f};

        // World coordinates ([-x:x, -y:y, -z:z])
        glm::vec4 worldRayT = {glm::inverse(getViewMatrix()) * cameraRay};
        glm::vec3 worldRay = {worldRayT.x, worldRayT.y, worldRayT.z};

        // Normalized ray
        glm::vec ray = glm::normalize(worldRay);

        return ray;

    }

    float Camera::constrainZoomLevel(float zoomLevel) {

        if (zoomLevel < m_minZoomLevel) {
            return m_minZoomLevel;
        } else if (zoomLevel > m_maxZoomLevel) {
            return m_maxZoomLevel;
        } else {
            return zoomLevel;
        }

    }

    void Camera::calculateViewMatrix() {

        // Look At
        m_viewMatrix = glm::lookAt(
            getPosition(),
            getPosition() + m_forward,
            m_up
        );

    }

    // {x, y, z, degrees}
    glm::quat Camera::getRotationQuaternion(glm::vec4 rotation) {

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

}