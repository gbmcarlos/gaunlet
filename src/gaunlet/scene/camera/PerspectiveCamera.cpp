#include "gaunlet/scene/camera/PerspectiveCamera.h"

namespace gaunlet::Scene {

    PerspectiveCamera::PerspectiveCamera()
        : PerspectiveCamera(45.0f, 1.0f, 1.0f, -10.0f){}

    PerspectiveCamera::PerspectiveCamera(float fieldOfView, float aspectRatio)
        : PerspectiveCamera(fieldOfView, aspectRatio, 1.0f, -10.0f) {}

    PerspectiveCamera::PerspectiveCamera(float fieldOfView, float aspectRatio, float near, float far)
        : Camera(near, far, (180.0f / fieldOfView) - 0.1f), m_fieldOfView(fieldOfView), m_aspectRatio(aspectRatio) {
        PerspectiveCamera::calculateProjectionMatrix();
    }

    void PerspectiveCamera::calculateProjectionMatrix() {

        m_projectionMatrix = glm::perspective(glm::radians(m_fieldOfView * m_zoomLevel), m_aspectRatio, m_near, m_far);

    }

    void PerspectiveCamera::resize(unsigned int viewportWidth, unsigned int viewportHeight) {

        m_aspectRatio = ((float) viewportWidth) / ((float) viewportHeight);

        calculateProjectionMatrix();
        calculateFrustum();

    }

    void PerspectiveCamera::calculateFrustum() {

        const float halfVSide = -m_far * tanf(m_fieldOfView * .5f);
        const float halfHSide = halfVSide * m_aspectRatio;
        const glm::vec3& position = getPosition();
        const glm::vec3& forward = getForward();
        const glm::vec3& right = getRight();
        const glm::vec3& up = getUp();
        glm::vec3 forwardFar(getForward() * m_far);

        Plane nearPlane(forward, position + (forward * m_near));
        Plane farPlane(-forward, position - forwardFar);

        Plane leftPlane(glm::cross(forwardFar - right * halfHSide, up), position);
        Plane rightPlane( glm::cross(up, forwardFar + right * halfHSide), position);

        Plane bottomPlane(glm::cross(forwardFar + up * halfVSide, right), position);
        Plane topPlane(glm::cross(right, forwardFar - up * halfVSide), position);

        m_frustum = {
            nearPlane, farPlane,
            leftPlane, rightPlane,
            bottomPlane, topPlane
        };

    }

}