#pragma once

#include "gaunlet/pch.h"

namespace gaunlet::Prefab::Basic3DRenderPipeline {

    struct SceneProperties {

        SceneProperties(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 directionalLightColor, glm::vec3 directionalLightDirection, float directionalLightAmbientStrength, float directionalLightDiffuseIntensity)
            : m_viewMatrix(viewMatrix), m_projectionMatrix(projectionMatrix), m_directionalLightColor(directionalLightColor), m_directionalLightDirection(directionalLightDirection), m_directionalLightAmbientIntensity(directionalLightAmbientStrength), m_directionalLightDiffuseIntensity(directionalLightDiffuseIntensity) {}

        glm::mat4 m_viewMatrix;
        glm::mat4 m_projectionMatrix;

        // The order of these properties is optimized to minimize required padding when using this data in a uniform buffer
        glm::vec3 m_directionalLightColor;
        float m_directionalLightAmbientIntensity;
        glm::vec3 m_directionalLightDirection;
        float m_directionalLightDiffuseIntensity;

    };

}