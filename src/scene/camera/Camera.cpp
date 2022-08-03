#include "Camera.h"

#include <algorithm>

namespace engine {

    glm::mat4 Camera::getViewProjectionMatrix() {
        return m_projectionMatrix * glm::inverse(m_transform.getTransformationMatrix());
    }

    void Camera::setTranslation(const glm::vec3 &translation) {
        m_transform.m_translation = translation;
    }

    void Camera::setRotation(const glm::vec3 &rotation) {
        m_transform.m_rotation = rotation;
    }

    void Camera::setZoomLevel(float zoomLevel) {
        m_zoomLevel = std::max(-zoomLevel, 0.1f);
        calculateProjectionMatrix();
    }

    void Camera::addZoomLevel(float zoomLevelDelta) {
        m_zoomLevel = std::max(m_zoomLevel - zoomLevelDelta, 0.1f);
        calculateProjectionMatrix();
    }

}