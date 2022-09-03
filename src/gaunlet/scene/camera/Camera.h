#pragma once

#include "gaunlet/scene/components/GraphicsComponents.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    class Camera {

    public:

        Camera();
        Camera(float near, float far);

        glm::mat4 getProjectionMatrix();
        glm::mat4 getViewMatrix();
        virtual float getAspectRatio() const = 0;
        virtual void resize(unsigned int viewportWidth, unsigned int viewportHeight) = 0;

        void setPosition(const glm::vec3& position);
        void move(const glm::vec3& movement);
        void moveRelative(const glm::vec3& movement);
        void setRotation(float yaw, float pitch);
        void setYaw(float yaw);
        void setPitch(float pitch);
        void addRotation(float yawDelta, float pitchDelta);
        void lookAt(glm::vec3 target);
        void lookAt(const TransformComponent& transform);

        inline float getZoomLevel() const {return m_zoomLevel;}
        void setZoomLevel(float zoomLevel);
        void addZoomLevel(float zoomLevelDelta);

    protected:

        float m_zoomLevel = 1.0f;
        float m_near;
        float m_far;

        glm::mat4 m_projectionMatrix;

        virtual void calculateProjectionMatrix() = 0;

    private:

        glm::mat4 m_viewMatrix;

        glm::vec3 m_position = {0, 0, 1};
        float m_yaw = -90.0f;
        float m_pitch = 0.0f;

        glm::vec3 m_right = {1, 0, 0};
        glm::vec3 m_up = {0, 1, 0};
        glm::vec3 m_forward = {0, 0, 1}; // Pointing toward the camera

        float constrainPitch(float pitch);
        void reverseViewMatrix();
        void calculateViewMatrix();

    };

}