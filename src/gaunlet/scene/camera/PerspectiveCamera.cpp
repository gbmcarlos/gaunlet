#include "gaunlet/scene/camera/PerspectiveCamera.h"

namespace gaunlet::Scene {

    PerspectiveCamera::PerspectiveCamera()
        : PerspectiveCamera(45.0f, 1.0f, 1.0f, 10.0f){}

    PerspectiveCamera::PerspectiveCamera(float fieldOfView, float aspectRatio)
        : PerspectiveCamera(fieldOfView, aspectRatio, 1.0f, 10.0f) {}

    PerspectiveCamera::PerspectiveCamera(float fieldOfView, float aspectRatio, float near, float far)
        : Camera(near, far), m_fieldOfView(fieldOfView), m_aspectRatio(aspectRatio) {
        PerspectiveCamera::calculateProjectionMatrix();
    }

    void PerspectiveCamera::calculateProjectionMatrix() {

        m_projectionMatrix = glm::perspective(glm::radians(m_fieldOfView * m_zoomLevel), m_aspectRatio, m_near, m_far);

    }

    void PerspectiveCamera::resize(unsigned int viewportWidth, unsigned int viewportHeight) {

        m_aspectRatio = ((float) viewportWidth) / ((float) viewportHeight);

        calculateProjectionMatrix();
    }

}