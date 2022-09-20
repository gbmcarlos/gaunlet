#pragma once

#include "gaunlet/scene/camera/PerspectiveCamera.h"
#include "gaunlet/scene/procedural-plane/ProceduralPlane.h"

namespace gaunlet::Scene {

    struct PlaneComponent {

        PlaneComponent(float size, float targetResolution, float resolutionSlope, float targetTessellationLevel, float tessellationLevelSlope, float maxHeight, Core::Ref<PerspectiveCamera>  camera, Core::Ref<Graphics::Texture>  heightmap)
            : m_size(size), m_targetResolution(targetResolution), m_resolutionSlope(resolutionSlope), m_targetTessellationLevel(targetTessellationLevel), m_tessellationLevelSlope(tessellationLevelSlope), m_maxHeight(maxHeight), m_camera(std::move(camera)), m_heightmap(std::move(heightmap)) {
        }

        std::vector<PlaneQuad> getContent() {
            return QuadTreePatch::compute(
                m_size,
                m_targetResolution,
                m_resolutionSlope,
                m_camera->getPosition(),
                m_camera->getFrustum()
            );
        }

        float m_size;
        float m_targetResolution;
        float m_resolutionSlope;
        float m_targetTessellationLevel;
        float m_tessellationLevelSlope;
        float m_maxHeight;
        Core::Ref<PerspectiveCamera> m_camera;
        Core::Ref<Graphics::Texture> m_heightmap;

    };

}