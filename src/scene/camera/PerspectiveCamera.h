#pragma once

#include "Camera.h"

namespace engine {

    class PerspectiveCamera : public Camera {

    public:
        PerspectiveCamera() : PerspectiveCamera(45.0f, 1.0f, 100.0f, 1.0f, 10.0f) {}
        PerspectiveCamera(float fieldOfView, float aspectRatio, float viewportResolution, float near, float far);
        void onWindowResize(float viewportWidth, float viewportHeight) override;

    private:
        float m_fieldOfView, m_aspectRatio, m_viewportResolution = 0;
        float m_near, m_far;

        void calculateProjectionMatrix() override;

    };

}