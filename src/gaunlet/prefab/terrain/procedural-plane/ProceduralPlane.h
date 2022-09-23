#pragma once

#include "gaunlet/graphics/Vertex.h"
#include "gaunlet/scene/camera/Frustum.h"

#include "gaunlet/pch.h"

namespace gaunlet::Prefab::Terrain {

    class QuadTreePatch;

    // This is the information that is ultimately sent to be rendered
    struct PlaneQuad {

        std::vector<Graphics::Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        QuadTreePatch* m_patch;
        unsigned int m_position;
        float m_leftSizeRatio = 0.0f;
        float m_bottomSizeRatio = 0.0f;
        float m_rightSizeRatio = 0.0f;
        float m_topSizeRatio = 0.0f;

    };

    // This is just a collection of data that is passed around
    struct Context {

        Context(float planeSize, float targetResolution, float resolutionSlope, const glm::vec3& cameraPosition, const Scene::Frustum& cameraFrustum)
            : m_planeSize(planeSize), m_targetResolution(targetResolution), m_resolutionSlope(resolutionSlope), m_cameraPosition(cameraPosition), m_cameraFrustum(cameraFrustum) {}

        float m_planeSize;
        float m_targetResolution;
        float m_resolutionSlope;
        glm::vec3 m_cameraPosition;
        Scene::Frustum m_cameraFrustum;
        std::vector<PlaneQuad> m_quads;

    };

    enum class PatchPosition {
        LeftBottom = 0, RightBottom = 1,
        RightTop = 2, LeftTop = 3,
        Root = 4
    };

    enum class HorizontalSide {
        Left, Right
    };

    enum class VerticalSide {
        Bottom, Top
    };

    // This is square in the plane, which can either be subdivided into more squares, or be drawn
    class QuadTreePatch : public std::enable_shared_from_this<QuadTreePatch> {

    public:

        static std::vector<PlaneQuad> compute(float planeSize, float targetResolution, float resolutionSlope, const glm::vec3& cameraPosition, const Scene::Frustum& cameraFrustum);

        QuadTreePatch(Core::Ref<QuadTreePatch>   parent, PatchPosition position, Context& context, glm::vec3 origin, float size)
            : m_parent(std::move(parent)), m_position(position), m_context(context), m_origin(origin), m_size(size) {
            computeDimensions();
        }

    protected:

        Core::Ref<QuadTreePatch> m_parent = nullptr;
        PatchPosition m_position;
        Context& m_context;
        std::vector<Core::Ref<QuadTreePatch>> m_children = {};
        glm::vec3 m_origin;
        float m_size;
        float m_leftEdge;
        float m_rightEdge;
        float m_bottomEdge;
        float m_topEdge;

        void computeDimensions();

        void process();

        bool requiresSubdivision();

        void subdivide();

        float getRelativeResolution(float distance) const;

        void createContent();

        void processEdges(PlaneQuad& quad);

        HorizontalSide getHorizontalSide();

        VerticalSide getVerticalSide();

        Core::Ref<QuadTreePatch> findHorizontalNeighbour(HorizontalSide side);

        Core::Ref<QuadTreePatch> findVerticalNeighbour(VerticalSide side);

        std::tuple<Core::Ref<QuadTreePatch>, std::vector<PatchPosition>> findFirstHorizontalSideAncestor(HorizontalSide side);

        std::tuple<Core::Ref<QuadTreePatch>, std::vector<PatchPosition>> findFirstVerticalSideAncestor(VerticalSide side);

        Core::Ref<QuadTreePatch> findDescendant(const std::vector<PatchPosition>& steps, bool horizontal);

        Core::Ref<QuadTreePatch> getSibling(PatchPosition position);

        Core::Ref<QuadTreePatch> getChild(PatchPosition position);

        PatchPosition getHorizontalNeighbourPosition(PatchPosition position);

        PatchPosition getVerticalNeighbourPosition(PatchPosition position);

    };

}