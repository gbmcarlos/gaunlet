#pragma once

#include "Camera.h"

namespace engine::Scene {

    class OrthographicCamera : public Camera {

    public:
        OrthographicCamera() : OrthographicCamera(2.0f, 2.0f, 100.0f) {}
        OrthographicCamera(unsigned int viewportWidth, unsigned int viewportHeight, float viewportResolution);
        void resize(unsigned int viewportWidth, unsigned int viewportHeight) override;
        inline float getAspectRatio() const override {return ((float) m_viewportWidth) / ((float) m_viewportHeight); }
        inline const glm::vec2& getProjectionSize() const {return m_projectionSize;}

    private:
        unsigned int m_viewportWidth, m_viewportHeight;
        float m_viewportResolution = 0;

        glm::vec2 m_projectionSize;

        void calculateProjectionMatrix() override;

    };

}