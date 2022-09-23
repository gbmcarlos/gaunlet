#pragma once

#include "gaunlet/scene/ray-casting/RayCasting.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    enum PlaneFlags {
        Near = 1 << 0,
        Far = 1 << 1,
        Left = 1 << 2,
        Right = 1 << 3,
        Bottom = 1 << 4,
        Top = 1 << 5
    };

    struct Frustum {

        Frustum() = default;

        Frustum(const Plane& nearPlane, const Plane& farPlane, const Plane& leftPlane, const Plane& rightPlane, const Plane& bottomPlane, const Plane& topPlane)
            : m_nearPlane(nearPlane), m_farPlane(farPlane), m_leftPlane(leftPlane), m_rightPlane(rightPlane), m_bottomPlane(bottomPlane), m_topPlane(topPlane) {}

        bool pointWithin(const glm::vec3& point, PlaneFlags flags = static_cast<PlaneFlags>(PlaneFlags::Near | PlaneFlags::Far | PlaneFlags::Left | PlaneFlags::Right | PlaneFlags::Bottom | PlaneFlags::Top)) {

            if (flags & PlaneFlags::Near) {
                if (!m_nearPlane.pointOver(point)) {
                    return false;
                }
            }

            if (flags & PlaneFlags::Far) {
                if (!m_farPlane.pointOver(point)) {
                    return false;
                }
            }

            if (flags & PlaneFlags::Left) {
                if (!m_leftPlane.pointOver(point)) {
                    return false;
                }
            }

            if (flags & PlaneFlags::Right) {
                if (!m_rightPlane.pointOver(point)) {
                    return false;
                }
            }

            if (flags & PlaneFlags::Bottom) {
                if (!m_bottomPlane.pointOver(point)) {
                    return false;
                }
            }

            if (flags & PlaneFlags::Top) {
                if (!m_topPlane.pointOver(point)) {
                    return false;
                }
            }

            return true;

        }

        // If all the points are behind any of the places, the polygon is outside the frustum
        bool intersectsPolygon(const std::vector<glm::vec3>& points, PlaneFlags flags = static_cast<PlaneFlags>(PlaneFlags::Near | PlaneFlags::Far | PlaneFlags::Left | PlaneFlags::Right | PlaneFlags::Bottom | PlaneFlags::Top)) {

            if (flags & PlaneFlags::Near) {
                if (!m_nearPlane.polygonOver(points)) {
                    return false;
                }
            }

            if (flags & PlaneFlags::Far) {
                if (!m_farPlane.polygonOver(points)) {
                    return false;
                }
            }

            if (flags & PlaneFlags::Left) {
                if (!m_leftPlane.polygonOver(points)) {
                    return false;
                }
            }

            if (flags & PlaneFlags::Right) {
                if (!m_rightPlane.polygonOver(points)) {
                    return false;
                }
            }

            if (flags & PlaneFlags::Bottom) {
                if (!m_bottomPlane.polygonOver(points)) {
                    return false;
                }
            }

            if (flags & PlaneFlags::Top) {
                if (!m_topPlane.polygonOver(points)) {
                    return false;
                }
            }

            return true;

        }

        Plane m_nearPlane;
        Plane m_farPlane;
        Plane m_leftPlane;
        Plane m_rightPlane;
        Plane m_bottomPlane;
        Plane m_topPlane;

    };

}