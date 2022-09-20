#pragma once

#include "gaunlet/graphics/Vertex.h"
#include "gaunlet/scene/camera/Frustum.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    struct PlaneQuad {

        std::vector<Graphics::Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

    };

    struct Context {

        Context(float planeSize, float targetResolution, float resolutionSlope, const glm::vec3& cameraPosition, const Frustum& cameraFrustum)
            : m_planeSize(planeSize), m_targetResolution(targetResolution), m_resolutionSlope(resolutionSlope), m_cameraPosition(cameraPosition), m_cameraFrustum(cameraFrustum) {}

        float m_planeSize;
        float m_targetResolution;
        float m_resolutionSlope;
        glm::vec3 m_cameraPosition;
        Frustum m_cameraFrustum;
        std::vector<PlaneQuad> m_quads;

    };

    class QuadTreePatch {

    public:

        static std::vector<PlaneQuad> compute(float planeSize, float targetResolution, float resolutionSlope, const glm::vec3& cameraPosition, const Frustum& cameraFrustum);

    protected:

        QuadTreePatch(glm::vec3 origin, float size)
            : m_origin(origin), m_size(size) {
            computeEdges();
        }

        glm::vec3 m_origin;
        float m_size;
        float m_leftEdge;
        float m_rightEdge;
        float m_bottomEdge;
        float m_topEdge;
        std::vector<QuadTreePatch> m_children = {};

        void computeEdges();

        void process(Context& context);

        bool withinFrustum(Context& context) const;

        bool requiresSubdivision(Context& context) const;

        void subdivide(Context& context);

        float getRelativeResolution(Context &context, float distance) const;

        void createContent(Context& context);

    };

}