#pragma once

#include "gaunlet/scene/components/GraphicsComponents.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    class Camera {

    public:

        glm::mat4 getProjectionMatrix();
        glm::mat4 getViewMatrix();
        virtual float getAspectRatio() const = 0;
        virtual void resize(unsigned int viewportWidth, unsigned int viewportHeight) = 0;

        void setPosition(const glm::vec3& position);
        void setYaw(float yaw);
        void addYaw(float yawDelta);
        void setPitch(float pitch);
        void addPitch(float pitchDelta);
        void lookAt(const glm::vec3& target);
        void lookAt(const TransformComponent& transform);

        inline float getZoomLevel() const {return m_zoomLevel;}
        void setZoomLevel(float zoomLevel);
        void addZoomLevel(float zoomLevelDelta);

    protected:

        float m_zoomLevel = 1.0f;
        glm::mat4 m_projectionMatrix;

        virtual void calculateProjectionMatrix() = 0;

    private:

        glm::mat4 m_viewMatrix;

        glm::vec3 m_position = {0, 0, 1};
        float m_yaw = -90.0f;
        float m_pitch = 0.0f;

        void reverseViewMatrix();
        void calculateViewMatrix();

    };

}