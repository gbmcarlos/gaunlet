#pragma once

#include "gaunlet/scene/camera/Camera.h"

namespace gaunlet::Scene {

    class PerspectiveCamera : public Camera {

    public:
        PerspectiveCamera();
        PerspectiveCamera(float fieldOfView, float aspectRatio);
        PerspectiveCamera(float fieldOfView, float aspectRatio, float near, float far);

        inline Camera::ProjectionType getProjectionType() const override {return Camera::ProjectionType::Perspective; }
        void resize(unsigned int viewportWidth, unsigned int viewportHeight) override;
        inline float getAspectRatio() const override {return m_aspectRatio; }

    private:
        float m_fieldOfView, m_aspectRatio = 0;

        void calculateProjectionMatrix() override;

    };

}