#pragma once

#include "Camera.h"

namespace engine {

    class OrthographicCamera : public Camera {

    public:
        OrthographicCamera() : OrthographicCamera(2.0f, 2.0f, 100.0f) {}
        OrthographicCamera(float viewportWidth, float viewportHeight, float viewportResolution);
        void onWindowResize(float viewportWidth, float viewportHeight) override;

        inline const glm::vec2& getProjectionSize() const {return m_projectionSize;}

    private:
        float m_viewportWidth, m_viewportHeight, m_viewportResolution = 0;

        glm::vec2 m_projectionSize;

        void calculateProjectionMatrix() override;

    };

}