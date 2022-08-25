#pragma once

#include "graphics-engine/scene/entity/GraphicsComponents.h"

#include "graphics-engine/pch.h"

namespace engine::Scene {

    class Camera {

    public:

        glm::mat4 getProjectionMatrix();
        glm::mat4 getViewMatrix();
        glm::mat4 getViewProjectionMatrix();
        virtual float getAspectRatio() const = 0;
        virtual void resize(unsigned int viewportWidth, unsigned int viewportHeight) = 0;

        void setTranslation(const glm::vec3& translation);
        void setRotation(const glm::vec3& rotation);

        inline float getZoomLevel() const {return m_zoomLevel;}
        void setZoomLevel(float zoomLevel);
        void addZoomLevel(float zoomLevelDelta);

        TransformComponent m_transform;

    protected:

        float m_zoomLevel = 1.0f;
        glm::mat4 m_projectionMatrix;

        virtual void calculateProjectionMatrix() = 0;

    private:
        glm::mat4 m_viewProjectionMatrix;

    };

}