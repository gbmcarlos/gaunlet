#pragma once

#include "Camera.h"

namespace engine::Scene {

    class PerspectiveCamera : public Camera {

    public:
        PerspectiveCamera() : PerspectiveCamera(45.0f, 1.0f, 100.0f, 1.0f, 10.0f) {}
        PerspectiveCamera(float fieldOfView, float aspectRatio, float viewportResolution, float near, float far);
        void resize(unsigned int viewportWidth, unsigned int viewportHeight) override;
        inline float getAspectRatio() const override {return m_aspectRatio; }

    private:
        float m_fieldOfView, m_aspectRatio = 0;
        float m_near, m_far;

        void calculateProjectionMatrix() override;

    };

}