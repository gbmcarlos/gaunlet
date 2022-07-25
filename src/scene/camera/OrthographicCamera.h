#pragma once

#include "glm/glm.hpp"

namespace engine {

    class OrthographicCamera {

    public:
        OrthographicCamera() : OrthographicCamera(2.0f, 2.0f, 100.0f) {}
        OrthographicCamera(float viewportWidth, float viewportHeight, float viewportResolution);
        OrthographicCamera(float viewportLeft, float viewportRight, float viewportBottom, float viewportTop, float viewportResolution);

        void onWindowResize(float viewportWidth, float viewportHeight);

        inline const glm::vec3& getPosition() const {return m_position;}
        void setPosition(const glm::vec3& position);
        void movePosition(const glm::vec3& positionDelta);

        inline float getRotation() const {return m_rotation;}
        void setRotation(float rotation);
        void addRotation(float rotationDelta);

        inline float getZoomLevel() const {return m_zoomLevel;}
        void setZoomLevel(float zoomLevel);
        void addZoomLevel(float zoomLevelDelta);

        inline const glm::vec2 getProjectionSize() const {return m_projectionSize;}

        inline const glm::mat4& getProjectionMatrix() const {return m_projectionMatrix;}
        inline const glm::mat4& getViewMatrix() const {return m_viewMatrix;}
        inline const glm::mat4& getViewProjectionMatrix() const {return m_viewProjectionMatrix;}

    private:

        float m_viewportWidth, m_viewportHeight, m_viewportResolution = 0;

        glm::vec2 m_projectionSize;
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_viewProjectionMatrix;

        glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
        float m_rotation = 0.0f;
        float m_zoomLevel = 1.0f;

        void calculateProjectionMatrix();
        void calculateViewProjectionMatrix();

    };

}