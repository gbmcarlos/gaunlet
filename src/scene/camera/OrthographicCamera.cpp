#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    OrthographicCamera::OrthographicCamera(unsigned int viewportWidth, unsigned int viewportHeight, float viewportResolution)
        : m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight), m_viewportResolution(viewportResolution) {
        OrthographicCamera::calculateProjectionMatrix();
    }

    void OrthographicCamera::calculateProjectionMatrix() {

        m_projectionSize = {
                (m_viewportWidth / m_viewportResolution) * m_zoomLevel,
                (m_viewportHeight / m_viewportResolution) * m_zoomLevel,
        };

        m_projectionMatrix = glm::ortho(-m_projectionSize.x/2, m_projectionSize.x/2, -m_projectionSize.y/2, m_projectionSize.y/2, -1.0f, 1.0f);

    }

    void OrthographicCamera::resize(unsigned int viewportWidth, unsigned int viewportHeight) {

        m_viewportWidth = viewportWidth;
        m_viewportHeight = viewportHeight;

        calculateProjectionMatrix();
    }

}