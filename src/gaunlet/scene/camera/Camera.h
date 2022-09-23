#pragma once

#include "gaunlet/scene/camera/Frustum.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    class Camera {

    public:

        enum class ProjectionType {
            Orthographic, Perspective
        };

        Camera();
        Camera(float near, float far);
        Camera(float near, float far, float maxZoomLevel);

        virtual ProjectionType getProjectionType() const = 0;
        virtual float getAspectRatio() const = 0;
        virtual void resize(unsigned int viewportWidth, unsigned int viewportHeight) = 0;

        glm::mat4 getProjectionMatrix();
        glm::mat4 getViewMatrix();
        const Frustum& getFrustum();

        inline const float& getMinZoomLevel() const {return m_minZoomLevel; }
        inline const float& getMaxZoomLevel() const {return m_maxZoomLevel; }

        glm::vec3& getPosition();
        inline float& getYaw() {return m_yaw; }
        inline float& getPitch() {return m_pitch; }
        inline float& getZoomLevel() {return m_zoomLevel;}

        void setPosition(const glm::vec3& position);
        void move(const glm::vec3& movement);
        void moveRelative(const glm::vec3& movement);
        void moveSemiRelative(const glm::vec3& movement);
        void follow(glm::vec3* target);
        glm::vec3* getFollowTarget();

        inline const glm::vec3& getForward() {return m_forward; }
        inline const glm::vec3& getRight() {return m_right; }
        inline const glm::vec3& getUp() {return m_up; }
        void setRotation(float yaw, float pitch);
        void setYaw(float yaw);
        void setPitch(float pitch);
        void addRotation(float yawDelta, float pitchDelta);
        void orbit(float degreesX, float degreesY);
        void orbit(float radius, float degreesX, float degreesY);
        void lookAt(glm::vec3 target);

        void setZoomLevel(float zoomLevel);
        void addZoomLevel(float zoomLevelDelta);

        glm::vec3 ray(glm::vec2 viewportCoordinates, glm::vec2 viewportSize);

    protected:

        float m_zoomLevel = 1.0f;
        float m_minPitch = -89;
        float m_maxPitch = 89;
        float m_minZoomLevel = 0.1;

        float m_maxZoomLevel;
        float m_near;
        float m_far;

        glm::mat4 m_projectionMatrix;
        virtual void calculateProjectionMatrix() = 0;

        Frustum m_frustum;
        virtual void calculateFrustum() = 0;

    private:

        glm::mat4 m_viewMatrix;

        bool m_following = false;
        glm::vec3* m_followTarget;
        glm::vec3 m_position = {0, 0, 1};
        float m_yaw = 0.0f;
        float m_pitch = 0.0f;

        glm::vec3 m_right = {1, 0, 0};
        glm::vec3 m_up = {0, 1, 0};
        glm::vec3 m_forward = {0, 0, 1}; // Pointing toward the camera

        float constrainPitch(float pitch);
        float constrainZoomLevel(float zoomLevel);
        void rotate(glm::vec3 pivot, float degreesX, float degreesY);
        void reverseViewMatrix();
        void calculateViewMatrix();

    };

}