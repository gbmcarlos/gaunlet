#include "gaunlet/scene/camera/OrthographicCamera.h"

namespace gaunlet::Scene {

    OrthographicCamera::OrthographicCamera()
        : OrthographicCamera(2.0f, 2.0f, 100.0f, 0.0f, 10.0f) {}

    OrthographicCamera::OrthographicCamera(unsigned int viewportWidth, unsigned int viewportHeight, float viewportResolution)
        : OrthographicCamera(viewportWidth, viewportHeight, viewportResolution, 0.0f, 10.0f) {}

    OrthographicCamera::OrthographicCamera(unsigned int viewportWidth, unsigned int viewportHeight, float viewportResolution, float near, float far)
        : Camera(near, far), m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight), m_viewportResolution(viewportResolution) {
        OrthographicCamera::calculateProjectionMatrix();
    }

    OrthographicCamera::OrthographicCamera(unsigned int viewportWidth, unsigned int viewportHeight, float viewportResolution, float near, float far, float maxZoomLevel)
        : Camera(near, far, maxZoomLevel), m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight), m_viewportResolution(viewportResolution) {
        OrthographicCamera::calculateProjectionMatrix();
    }

    void OrthographicCamera::calculateProjectionMatrix() {

        m_projectionSize = {
                (m_viewportWidth / m_viewportResolution) * m_zoomLevel,
                (m_viewportHeight / m_viewportResolution) * m_zoomLevel,
        };

        m_projectionMatrix = glm::ortho(-m_projectionSize.x/2, m_projectionSize.x/2, -m_projectionSize.y/2, m_projectionSize.y/2, m_near, m_far);

    }

    void OrthographicCamera::resize(unsigned int viewportWidth, unsigned int viewportHeight) {

        m_viewportWidth = viewportWidth;
        m_viewportHeight = viewportHeight;

        calculateProjectionMatrix();
    }

    void OrthographicCamera::calculateFrustum() {
        m_frustum = {};
    }

}