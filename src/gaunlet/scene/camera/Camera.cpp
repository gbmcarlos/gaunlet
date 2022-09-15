#include "gaunlet/scene/camera/Camera.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    Camera::Camera()
        : Camera(1.0f, 10.0f) {}

    Camera::Camera(float near, float far)
        : Camera(near, far, 5.0f) {}

    Camera::Camera(float near, float far, float maxZoomLevel)
        : m_near(near), m_far(far), m_maxZoomLevel(maxZoomLevel) {
        calculateViewMatrix();
    }

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

    void Camera::move(const glm::vec3 &movement) {
        m_position += movement;
        calculateViewMatrix();
    }

    void Camera::moveRelative(const glm::vec3 &movement) {

        glm::mat3 relativeTransform = glm::mat3(
            m_right,
            m_up,
            m_forward
        );

        glm::vec3 relativeMovement = relativeTransform * movement;

        move(relativeMovement);

    }

    void Camera::setRotation(float yaw, float pitch) {
        m_yaw = yaw;
        m_pitch = constrainPitch(pitch);
        calculateViewMatrix();
    }

    void Camera::setYaw(float yaw) {
        m_yaw = yaw;
        calculateViewMatrix();
    }

    void Camera::setPitch(float pitch) {

        m_pitch = constrainPitch(pitch);
        calculateViewMatrix();

    }

    void Camera::addRotation(float yawDelta, float pitchDelta) {

        m_yaw = m_yaw + yawDelta;
        m_pitch = constrainPitch(m_pitch + pitchDelta);
        calculateViewMatrix();

    }

    void Camera::orbit(float degreesX, float degreesY) {

        glm::vec3 pivot = {0, 0, 0};

        rotate(pivot, degreesX, degreesY);
        lookAt(pivot);

    }

    void Camera::orbit(float radius, float degreesX, float degreesY) {

        glm::vec3 pivot = m_position + (m_forward * radius);
        rotate(pivot, degreesX, degreesY);
        lookAt(pivot);

    }

    void Camera::orbit(TransformComponent origin, float degreesX, float degreesY) {

        glm::vec3 pivot = origin.m_translation;
        rotate(pivot, degreesX, degreesY);
        lookAt(pivot);

    }

    void Camera::lookAt(glm::vec3 target) {

        // If we're trying to look straight up or straight down, move the target slight forward
        glm::vec3 direction = glm::abs(glm::normalize(target - m_position));
        if (direction == glm::vec3(0, 1, 0)) {
            target.z += -0.5;
        }

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

        m_zoomLevel = constrainZoomLevel(zoomLevel);
        calculateProjectionMatrix();

    }

    void Camera::addZoomLevel(float zoomLevelDelta) {

        m_zoomLevel = constrainZoomLevel(m_zoomLevel + zoomLevelDelta);
        calculateProjectionMatrix();

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

    glm::vec3 Camera::rayPlaneIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planePoint, glm::vec3 planeNormal) {

        float distance;

        bool intersects = glm::intersectRayPlane(
            rayOrigin, rayDirection,
            planePoint, planeNormal,
            distance
        );

        if (intersects) {
            return rayOrigin + (rayDirection * distance);
        } else {
            return {NAN, NAN, NAN};
        }

    }

    float Camera::constrainPitch(float pitch) {

        if (pitch > m_maxPitch) {
            return m_maxPitch;
        } else if (pitch < m_minPitch) {
            return m_minPitch;
        } else {
            return pitch;
        }

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

    void Camera::rotate(glm::vec3 pivot, float degreesX, float degreesY) {

        auto translation1 = glm::translate(glm::mat4(1), -pivot); // Move the pivot to the center
        auto rotationX = glm::rotate(glm::mat4(1), glm::radians(degreesX), {1, 0, 0}); // Rotate it
        auto rotationY = glm::rotate(glm::mat4(1), glm::radians(degreesY), {0, 1, 0}); // Rotate it
        auto translation2 = glm::translate(glm::mat4(1), pivot); // Move it back

        m_position = glm::vec3(translation1 * rotationX * rotationY * translation2 * glm::vec4(m_position, 1));

    }

    // Calculate the orientation parameters (right, up, yaw, and pitch) based in the current view matrix
    void Camera::reverseViewMatrix() {

        const glm::mat4 inverted = glm::inverse(m_viewMatrix);

        auto forward = -glm::vec3(inverted[2]);
        auto right = glm::normalize(glm::cross({0, 1, 0}, forward));
        auto up = glm::normalize(glm::cross(forward, right));

        auto yaw = glm::degrees(glm::atan(forward.z, forward.x));
        auto pitch = glm::degrees(glm::asin(forward.y));

        m_yaw = yaw;
        m_pitch = pitch;

        m_right = right;
        m_up = up;
        m_forward = forward;

        calculateViewMatrix();

    }

    // Calculate a view matrix and right and up based on the current position and yaw and pitch
    void Camera::calculateViewMatrix() {

        // Direction (points from the target to the camera)
        glm::vec3 forward = {
            cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
            sin(glm::radians(m_pitch)),
            sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
        };
        m_forward = glm::normalize(forward);

        // Right
        m_right = glm::normalize(glm::cross(
            forward,
            {0, 1, 0}
        ));

        // Up
        m_up = glm::normalize(glm::cross(
            m_right,
            forward
        ));

        // Look At
        m_viewMatrix = glm::lookAt(
            m_position,
            m_position + forward,
            m_up
        );

    }

}