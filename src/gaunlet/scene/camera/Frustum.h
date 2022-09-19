#pragma once

#include "gaunlet/scene/raw-casting/RayCasting.h"

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    class Frustum {

    public:

        Frustum() = default;

        Frustum(const Plane& nearPlane, const Plane& farPlane, const Plane& leftPlane, const Plane& rightPlane, const Plane& bottomPlane, const Plane& topPlane){
            m_planes.push_back(nearPlane);
            m_planes.push_back(farPlane);
            m_planes.push_back(leftPlane);
            m_planes.push_back(rightPlane);
            m_planes.push_back(bottomPlane);
            m_planes.push_back(topPlane);
        }

        bool pointWithin(const glm::vec3& point) {

            for (auto& plane : m_planes) {
                if (!plane.pointOver(point)) {
                    return false;
                }
            }

            return true;

        }

        // If all the points are behind any of the places, the polygon is outside the frustum
        bool intersectsPolygon(const std::vector<glm::vec3>& points) {

            for (auto& plane : m_planes) {
                if (!plane.polygonOver(points)) {
                    return false;
                }
            }

            return true;

        }

    private:

        std::vector<Plane> m_planes;

    };

}