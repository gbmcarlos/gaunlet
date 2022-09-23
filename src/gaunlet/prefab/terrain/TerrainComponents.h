#pragma once

#include "gaunlet/scene/camera/PerspectiveCamera.h"
#include "gaunlet/prefab/terrain/procedural-plane/ProceduralPlane.h"

namespace gaunlet::Prefab::Terrain {

    struct TerrainComponent {

        TerrainComponent(float size, float targetResolution, float resolutionSlope, float triangleSize, float maxHeight, Core::Ref<Scene::PerspectiveCamera>  camera, Core::Ref<Graphics::Texture>  heightmap)
            : m_size(size), m_targetResolution(targetResolution), m_resolutionSlope(resolutionSlope), m_triangleSize(triangleSize), m_maxHeight(maxHeight), m_camera(std::move(camera)), m_heightmap(std::move(heightmap)) {
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
        float m_triangleSize;
        float m_maxHeight;
        Core::Ref<Scene::PerspectiveCamera> m_camera;
        Core::Ref<Graphics::Texture> m_heightmap;

    };

}