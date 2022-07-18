#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace engine {

    OrthographicCamera::OrthographicCamera(float viewportWidth, float viewportHeight, float viewportResolution)
        : m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight), m_viewportResolution(viewportResolution), m_viewMatrix(glm::mat4(1.0f)) {
        calculateProjectionMatrix();
        calculateViewProjectionMatrix();
    }

    OrthographicCamera::OrthographicCamera(float viewportLeft, float viewportRight, float viewportBottom, float viewportTop, float viewportResolution)
        : m_viewportWidth(viewportRight - viewportLeft), m_viewportHeight(viewportTop - viewportBottom), m_viewportResolution(viewportResolution), m_viewMatrix(glm::mat4(1.0f)) {
        calculateProjectionMatrix();
        calculateViewProjectionMatrix();
    }

    void OrthographicCamera::calculateProjectionMatrix() {

        m_projectionSize = {
                (m_viewportWidth / m_viewportResolution),
                (m_viewportHeight / m_viewportResolution),
        };

        m_projectionMatrix = glm::ortho(-m_projectionSize.x/2 * m_zoomLevel, m_projectionSize.x/2 * m_zoomLevel, -m_projectionSize.y/2 * m_zoomLevel, m_projectionSize.y/2 * m_zoomLevel, -1.0f, 1.0f);

    }

    void OrthographicCamera::calculateViewProjectionMatrix() {

        // Calculate the position matrix based on our vec3 m_position
        glm::mat4 position = glm::translate(glm::mat4(1.0f), m_position);

        // Calculate the rotation matrix based on our float rotation (rotate only along the z axis)
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

        // The transform of the camera is the position * rotation
        glm::mat4 transform = position * rotation;

        // Moving the camera actually means moving the whole world, so we invert its transform
        m_viewMatrix = glm::inverse(transform);

        // These 2 can be multiplied already, for ready use
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

    }

    void OrthographicCamera::onWindowResize(const WindowResizeEvent& event) {

        m_viewportWidth = event.getWidth();
        m_viewportHeight = event.getHeight();

        calculateProjectionMatrix();
        calculateViewProjectionMatrix();
    }

    void OrthographicCamera::setPosition(const glm::vec3 &position) {
        m_position = position;
        calculateViewProjectionMatrix();
    }

    void OrthographicCamera::movePosition(const glm::vec3 &positionDelta) {
        m_position += positionDelta;
        calculateViewProjectionMatrix();
    }

    void OrthographicCamera::setRotation(float rotation) {
        m_rotation = rotation;
        calculateViewProjectionMatrix();
    }

    void OrthographicCamera::addRotation(float rotationDelta) {
        m_rotation += rotationDelta;
        calculateViewProjectionMatrix();
    }

    void OrthographicCamera::setZoomLevel(float zoomLevel) {
        m_zoomLevel = std::max(-zoomLevel, 0.1f);
        calculateProjectionMatrix();
        calculateViewProjectionMatrix();
    }

    void OrthographicCamera::addZoomLevel(float zoomLevelDelta) {
        m_zoomLevel = std::max(m_zoomLevel - zoomLevelDelta, 0.1f);
        calculateProjectionMatrix();
        calculateViewProjectionMatrix();
    }

    glm::mat4 OrthographicCamera::getDefaultViewProjectionMatrix() {
        return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    }

}