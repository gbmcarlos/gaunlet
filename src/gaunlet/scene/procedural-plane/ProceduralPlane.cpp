#include "gaunlet/scene/procedural-plane/ProceduralPlane.h"

namespace gaunlet::Scene {

    std::vector<PlaneQuad> QuadTreePatch::compute(float planeSize, float targetResolution, float resolutionSlope, const glm::vec3& cameraPosition, const Frustum& cameraFrustum) {

        Context context(planeSize, targetResolution, resolutionSlope, cameraPosition, cameraFrustum);

        auto rootNode = Core::CreateRef<QuadTreePatch>(nullptr, PatchPosition::Root, context, glm::vec3(0, 0, 0), planeSize);
        rootNode->process();

        for (auto& quad : context.m_quads) {
            quad.m_patch->processEdges(quad);
        }

        return context.m_quads;

    }

    void QuadTreePatch::computeDimensions() {

        float halfSize = m_size/2;

        m_leftEdge = m_origin.x - halfSize;
        m_rightEdge = m_origin.x + halfSize;
        m_bottomEdge = m_origin.z + halfSize; // Positive, because this is the Z axis
        m_topEdge = m_origin.z - halfSize; // Negative, because this is the Z axis

    }

    void QuadTreePatch::process() {

        // If this patch requires subdivision, there's no need to create its own content
        if (requiresSubdivision()) {
            subdivide();
        } else {
            createContent();
        }

    }

    bool QuadTreePatch::requiresSubdivision() {

        glm::vec3 projectedCameraPosition(m_context.m_cameraPosition.x, 0, m_context.m_cameraPosition.z + 1.0);

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
        float relativeResolution = getRelativeResolution(distance);

        return m_size > relativeResolution;

    }

    float QuadTreePatch::getRelativeResolution(float distance) const {

        float relativeResolution = (distance * m_context.m_resolutionSlope) + 1;
        relativeResolution = std::max(relativeResolution, m_context.m_targetResolution);

        return relativeResolution;

    }

    // Add 4 children to the node, each half the size
    void QuadTreePatch::subdivide() {

        float halfSize = m_size/2;
        float quarterSize = m_size/4;

        auto leftBottomPatch = Core::CreateRef<QuadTreePatch>(shared_from_this(), PatchPosition::LeftBottom, m_context, glm::vec3(m_origin.x - quarterSize, 0, m_origin.z + quarterSize), halfSize);
        m_children.emplace_back(leftBottomPatch);
        leftBottomPatch->process();

        auto rightBottomPatch = Core::CreateRef<QuadTreePatch>(shared_from_this(), PatchPosition::RightBottom, m_context, glm::vec3(m_origin.x + quarterSize, 0, m_origin.z + quarterSize), halfSize);
        m_children.emplace_back(rightBottomPatch);
        rightBottomPatch->process();

        auto rightTopPatch = Core::CreateRef<QuadTreePatch>(shared_from_this(), PatchPosition::RightTop, m_context, glm::vec3(m_origin.x + quarterSize, 0, m_origin.z - quarterSize), halfSize);
        m_children.emplace_back(rightTopPatch);
        rightTopPatch->process();

        auto leftTopPatch = Core::CreateRef<QuadTreePatch>(shared_from_this(), PatchPosition::LeftTop, m_context, glm::vec3(m_origin.x - quarterSize, 0, m_origin.z - quarterSize), halfSize);
        m_children.emplace_back(leftTopPatch);
        leftTopPatch->process();

    }

    void QuadTreePatch::createContent() {

        float planeLeft = -m_context.m_planeSize / 2;
        float planeBottom = m_context.m_planeSize/2; // Positive, because this is the Z axis

        float patchUVLeft = planeLeft == m_leftEdge ? 0 : ((m_leftEdge - planeLeft) / m_context.m_planeSize); // If the patch is on the left edge of the plane, avoid dividing by 0, U is 0
        float patchUVRight = (m_rightEdge - planeLeft) / m_context.m_planeSize;
        float patchUVBottom = planeBottom == m_bottomEdge ? 0 : ((m_bottomEdge - planeBottom) / m_context.m_planeSize); // Same as with planeLeft, avoid dividing by 0, V is 0
        float patchUVTop = (m_topEdge - planeBottom) / m_context.m_planeSize;

        m_context.m_quads.push_back({
            {
                {{m_leftEdge, 0, m_bottomEdge, 1}, {0, 1, 0}, {patchUVLeft, patchUVBottom}},
                {{m_rightEdge, 0, m_bottomEdge, 1}, {0, 1, 0}, {patchUVRight, patchUVBottom}},
                {{m_rightEdge, 0, m_topEdge, 1}, {0, 1, 0}, {patchUVRight, patchUVTop}},
                {{m_leftEdge, 0, m_topEdge, 1}, {0, 1, 0}, {patchUVLeft, patchUVTop}}
            },
            {0, 1, 2, 3},
            this
        });

    }

    void QuadTreePatch::processEdges(PlaneQuad& quad) {

        auto leftNeighbour = findHorizontalNeighbour(HorizontalSide::Left);
        if (leftNeighbour != nullptr) {
            quad.m_leftSizeRatio = leftNeighbour->m_size / m_size;
        }

        auto bottomNeighbour = findVerticalNeighbour(VerticalSide::Bottom);
        if (bottomNeighbour != nullptr) {
            quad.m_bottomSizeRatio = bottomNeighbour->m_size / m_size;
        }

        auto rightNeighbour = findHorizontalNeighbour(HorizontalSide::Right);
        if (rightNeighbour != nullptr) {
            quad.m_rightSizeRatio = rightNeighbour->m_size / m_size;
        }

        auto topNeighbour = findVerticalNeighbour(VerticalSide::Top);
        if (topNeighbour != nullptr) {
            quad.m_topSizeRatio = topNeighbour->m_size / m_size;
        }

    }

    /*
     * To find a neighbour, we first ascend the ancestry tree until we find the common ancestor
     * The common ancestor will be that node that is not on the same side as the edge we're looking for
     * In the case where we are on the left, and we're looking for the right neighbour, this would mean that we are the common ancestor
     * Once we've reached the common ancestor, we descend back down, keeping on the same side
     * WARNING This last part (descending keeping on the same side) only works because of the way we subdivide based on the distance to the camera
     */
    Core::Ref<QuadTreePatch> QuadTreePatch::findHorizontalNeighbour(HorizontalSide side) {

        // Discard the root-node case
        if (m_parent == nullptr) {
            return nullptr;
        }

        // Look for the common ancestor
        HorizontalSide oppositeSide = side == HorizontalSide::Left ? HorizontalSide::Right : HorizontalSide::Left;
        auto commonAncestor = findFirstHorizontalSideAncestor(oppositeSide);

        // If we haven't found such an ancestor, it means that we are on the edge of the plane
        if (commonAncestor == nullptr) {
            return nullptr;
        }

        // Now that we have an ancestor that is not on the same side, we get its horizontal neighbour
        auto neighbourAncestor = commonAncestor->getSibling(
            getHorizontalNeighbourPosition(commonAncestor->m_position)
        );

        // Now go back down the ancestry tree
        auto neighbour = neighbourAncestor->findLastDescendant(m_position);

        return neighbour;

    }

    // Same as findHorizontalNeighbour, but vertically
    Core::Ref<QuadTreePatch> QuadTreePatch::findVerticalNeighbour(VerticalSide side) {

        // Discard the root-node case
        if (m_parent == nullptr) {
            return nullptr;
        }

        // Look for the common ancestor
        VerticalSide oppositeSide = side == VerticalSide::Bottom ? VerticalSide::Top : VerticalSide::Bottom;
        auto commonAncestor = findFirstVerticalSideAncestor(oppositeSide);

        // If we haven't found such an ancestor, it means that we are on the edge of the plane
        if (commonAncestor == nullptr) {
            return nullptr;
        }

        // Now that we have an ancestor that is not on the same side, we get its vertical neighbour
        auto neighbourAncestor = commonAncestor->getSibling(
            getVerticalNeighbourPosition(commonAncestor->m_position)
        );

        // Now go back down the ancestry tree
        auto neighbour = neighbourAncestor->findLastDescendant(m_position);

        return neighbour;

    }

    // Find the first ancestor that is on the left/right
    Core::Ref<QuadTreePatch> QuadTreePatch::findFirstHorizontalSideAncestor(HorizontalSide side) {

        auto current = shared_from_this();
        Core::Ref<QuadTreePatch> ancestor = nullptr;

        while (ancestor == nullptr) {

            // We've reached the root node, stop here
            if (current->m_parent == nullptr) {
                break;
            }

            // If we're still on the wrong side, move on to the next generation
            if (current->getHorizontalSide() != side) {
                current = current->m_parent;
                continue;
            }

            // We've found an ancestor that is on the correct side
            if (current->getHorizontalSide() == side) {
                ancestor = current;
                continue;
            }

        }

        return ancestor;

    }

    Core::Ref<QuadTreePatch> QuadTreePatch::findFirstVerticalSideAncestor(VerticalSide side) {

        auto current = shared_from_this();
        Core::Ref<QuadTreePatch> ancestor = nullptr;

        while (ancestor == nullptr) {

            // We've reached the root node, stop here
            if (current->m_parent == nullptr) {
                break;
            }

            // If we're still on the wrong side, move on to the next generation
            if (current->getVerticalSide() != side) {
                current = current->m_parent;
                continue;
            }

            // We've found an ancestor that is on the correct side
            if (current->getVerticalSide() == side) {
                ancestor = current;
                continue;
            }

        }

        return ancestor;

    }

    Core::Ref<QuadTreePatch> QuadTreePatch::findLastDescendant(PatchPosition position) {

        auto current = shared_from_this();
        Core::Ref<QuadTreePatch> descendant = nullptr;

        while (descendant == nullptr) {

            if (!current->m_children.empty()) {
                current = current->getChild(position);
                continue;
            } else {
                descendant = current;
                continue;
            }

        }

        return descendant;

    }

    Core::Ref<QuadTreePatch> QuadTreePatch::getSibling(PatchPosition position) {
        return m_parent->getChild(position);
    }

    Core::Ref<QuadTreePatch> QuadTreePatch::getChild(PatchPosition position) {
        return m_children[static_cast<unsigned int>(position)];
    }

    HorizontalSide QuadTreePatch::getHorizontalSide() {
        if (m_position == PatchPosition::LeftBottom || m_position == PatchPosition::LeftTop) {
            return HorizontalSide::Left;
        } else {
            return HorizontalSide::Right;
        }
    }

    VerticalSide QuadTreePatch::getVerticalSide() {
        if (m_position == PatchPosition::LeftBottom || m_position == PatchPosition::RightBottom) {
            return VerticalSide::Bottom;
        } else {
            return VerticalSide::Top;
        }
    }

    PatchPosition QuadTreePatch::getHorizontalNeighbourPosition(PatchPosition position) {

        switch (position) {
            case PatchPosition::LeftBottom:     return PatchPosition::RightBottom;
            case PatchPosition::RightBottom:    return PatchPosition::LeftBottom;
            case PatchPosition::RightTop:       return PatchPosition::LeftTop;
            case PatchPosition::LeftTop:        return PatchPosition::RightTop;
        }

    }

    PatchPosition QuadTreePatch::getVerticalNeighbourPosition(PatchPosition position) {

        switch (position) {
            case PatchPosition::LeftBottom:     return PatchPosition::LeftTop;
            case PatchPosition::RightBottom:    return PatchPosition::RightTop;
            case PatchPosition::RightTop:       return PatchPosition::RightBottom;
            case PatchPosition::LeftTop:        return PatchPosition::LeftBottom;
        }

    }

}