#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(glm::mat4(1.0f)) {
             calculateViewProjectionMatrix();
    }

    void OrthographicCamera::calculateViewProjectionMatrix() {

        // Calculate the position matrix based on our vec3 m_position
        glm::mat4 position = glm::translate(glm::mat4(1.0f), m_position);

        // Calculate the rotation matrix based on our float rotation (rotate only along the z axis)
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

        // The transform of the camera is the m_position * rotation
        glm::mat4 transform = position * rotation;

        // Moving the m_camera actually means moving the whole world, so we invert its transform
        m_viewMatrix = glm::inverse(transform);

        // These 2 can be multiplied already, for ready use
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

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

    const glm::mat4& OrthographicCamera::getDefaultViewProjectionMatrix() {
        return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    }

}