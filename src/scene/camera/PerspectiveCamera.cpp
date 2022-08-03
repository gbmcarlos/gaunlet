#include "PerspectiveCamera.h"

namespace engine {

    PerspectiveCamera::PerspectiveCamera(float fieldOfView, float aspectRatio, float viewportResolution, float near, float far)
        : m_fieldOfView(fieldOfView), m_aspectRatio(aspectRatio), m_viewportResolution(viewportResolution), m_near(near), m_far(far) {
        PerspectiveCamera::calculateProjectionMatrix();
    }

    void PerspectiveCamera::calculateProjectionMatrix() {

        m_projectionMatrix = glm::perspective(glm::radians(m_fieldOfView), m_aspectRatio, m_near, m_far);

    }

    void PerspectiveCamera::onWindowResize(float viewportWidth, float viewportHeight) {

        m_aspectRatio = viewportWidth / viewportHeight;

        calculateProjectionMatrix();
    }

}