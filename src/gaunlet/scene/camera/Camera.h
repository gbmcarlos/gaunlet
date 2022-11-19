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

        // Camera properties
        virtual ProjectionType getProjectionType() const = 0;
        virtual float getAspectRatio() const = 0;
        virtual void resize(unsigned int viewportWidth, unsigned int viewportHeight) = 0;

        // Matrices
        glm::mat4 getProjectionMatrix();
        glm::mat4 getViewMatrix();
        const Frustum& getFrustum();

        // Zoom
        inline const float& getMinZoomLevel() const {return m_minZoomLevel; }
        inline const float& getMaxZoomLevel() const {return m_maxZoomLevel; }

        // State getters
        glm::vec3& getPosition();
        inline const glm::vec3& getForward() {return m_forward; }
        inline const glm::vec3& getRight() {return m_right; }
        inline const glm::vec3& getUp() {return m_up; }
        inline float& getZoomLevel() {return m_zoomLevel;}

        void setPosition(const glm::vec3& position);
        void setOrientation(const glm::vec3& forward, const glm::vec3& up);
        void setZoomLevel(float zoomLevel);

        void move(const glm::vec3& movement);
        void moveRelative(const glm::vec3& movement);
        void moveSemiRelative(const glm::vec3& movement);

        void rotate(glm::vec4 rotation);
        void lookAt(glm::vec3 target);
        void addZoomLevel(float zoomLevelDelta);

        // Utilities
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

        glm::vec3 m_position = {0, 0, 1};

        glm::vec3 m_right = {1, 0, 0};
        glm::vec3 m_up = {0, 1, 0};
        glm::vec3 m_forward = {0, 0, -1};

        float constrainPitch(float pitch);
        float constrainZoomLevel(float zoomLevel);
        void calculateViewMatrix();
        glm::quat getRotationQuaternion(glm::vec4 rotation);

    };

}