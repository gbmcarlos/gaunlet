#include "gaunlet/scene/procedural-plane/ProceduralPlane.h"

namespace gaunlet::Scene {

    std::vector<PlaneQuad> QuadTreePatch::compute(float planeSize, float targetResolution, float resolutionSlope, const glm::vec3& cameraPosition, const Frustum& cameraFrustum) {

        Context context(planeSize, targetResolution, resolutionSlope, cameraPosition, cameraFrustum);

        QuadTreePatch rootNode({0, 0, 0}, planeSize);
        rootNode.process(context);

        return context.m_quads;

    }

    void QuadTreePatch::computeEdges() {

        float halfSize = m_size/2;

        m_leftEdge = m_origin.x - halfSize;
        m_rightEdge = m_origin.x + halfSize;
        m_bottomEdge = m_origin.z + halfSize; // Positive, because this is the Z axis
        m_topEdge = m_origin.z - halfSize; // Negative, because this is the Z axis

    }

    void QuadTreePatch::process(Context &context) {

        // If this patch requires subdivision, there's no need to create its own content
        if (requiresSubdivision(context)) {
            subdivide(context);
        } else {
            createContent(context);
        }

    }

    bool QuadTreePatch::requiresSubdivision(Context &context) const {

        glm::vec3 projectedCameraPosition(context.m_cameraPosition.x, 0, context.m_cameraPosition.z + 1.0);

        // Calculate the position of the 4 corners of the quad
        glm::vec3 leftBottomCorner = {m_leftEdge, 0, m_bottomEdge};
        glm::vec3 rightBottomCorner = {m_rightEdge, 0, m_bottomEdge};
        glm::vec3 rightTopCorner = {m_rightEdge, 0, m_topEdge};
        glm::vec3 leftTopCorner = {m_leftEdge, 0, m_topEdge};

        // The distance between the camera and each corner of the quad
        float distanceLeftBottom = glm::length(leftBottomCorner - projectedCameraPosition);
        float distanceRightBottom = glm::length(rightBottomCorner - projectedCameraPosition);
        float distanceRightTop = glm::length(rightTopCorner - projectedCameraPosition);
        float distanceLeftTop = glm::length(leftTopCorner - projectedCameraPosition);

        // The distance to the closest corner
        float distance = std::min(std::min(distanceLeftBottom, distanceRightBottom), std::min(distanceRightTop, distanceLeftTop));

        // Calculate the required resolution based on the distance to the camera
        float relativeResolution = getRelativeResolution(context, distance);

        return m_size > relativeResolution;

    }

    float QuadTreePatch::getRelativeResolution(Context &context, float distance) const {

        float relativeResolution = (distance * context.m_resolutionSlope) + 1;
        relativeResolution = std::max(relativeResolution, context.m_targetResolution);

        return relativeResolution;

    }

    // Add 4 children to the node, each a quarter of the size
    void QuadTreePatch::subdivide(Context &context) {

        float halfSize = m_size/2;
        float quarterSize = m_size/4;

        QuadTreePatch leftBottomPatch({m_origin.x - quarterSize, 0, m_origin.z + quarterSize}, halfSize);
        m_children.push_back(leftBottomPatch);
        leftBottomPatch.process(context);

        QuadTreePatch rightBottomPatch({m_origin.x + quarterSize, 0, m_origin.z + quarterSize}, halfSize);
        m_children.push_back(rightBottomPatch);
        rightBottomPatch.process(context);

        QuadTreePatch rightTopPatch({m_origin.x + quarterSize, 0, m_origin.z - quarterSize}, halfSize);
        m_children.push_back(rightTopPatch);
        rightTopPatch.process(context);

        QuadTreePatch leftTopPatch({m_origin.x - quarterSize, 0, m_origin.z - quarterSize}, halfSize);
        m_children.push_back(leftTopPatch);
        leftTopPatch.process(context);

    }

    void QuadTreePatch::createContent(Context &context) {

        float planeLeft = -context.m_planeSize / 2;
        float planeBottom = context.m_planeSize/2; // Positive, because this is the Z axis

        float patchUVLeft = planeLeft == m_leftEdge ? 0 : ((m_leftEdge - planeLeft) / context.m_planeSize); // If the patch is on the left edge of the plane, avoid dividing by 0, U is 0
        float patchUVRight = (m_rightEdge - planeLeft) / context.m_planeSize;
        float patchUVBottom = planeBottom == m_bottomEdge ? 0 : ((m_bottomEdge - planeBottom) / context.m_planeSize); // Same as with planeLeft, avoid dividing by 0, V is 0
        float patchUVTop = (m_topEdge - planeBottom) / context.m_planeSize;

        context.m_quads.push_back({
            {
                {{m_leftEdge, 0, m_bottomEdge, 1}, {0, 1, 0}, {patchUVLeft, patchUVBottom}},
                {{m_rightEdge, 0, m_bottomEdge, 1}, {0, 1, 0}, {patchUVRight, patchUVBottom}},
                {{m_rightEdge, 0, m_topEdge, 1}, {0, 1, 0}, {patchUVRight, patchUVTop}},
                {{m_leftEdge, 0, m_topEdge, 1}, {0, 1, 0}, {patchUVLeft, patchUVTop}}
            },
            {0, 1, 2, 3}
        });

    }

}