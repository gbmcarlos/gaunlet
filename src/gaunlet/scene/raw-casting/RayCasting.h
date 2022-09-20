#pragma once

#include "gaunlet/pch.h"

namespace gaunlet::Scene {

    struct Plane {

        Plane();
        Plane(glm::vec3 normal, glm::vec3 point);

        // Check if a point is over the plane or below
        bool pointOver(const glm::vec3& point);

        // Check if all the points out NOT over the plane. Return true if any of the points is ove the plane
        bool polygonOver(const std::vector<glm::vec3>& points);

        // Return the point of intersection with a ray
        glm::vec3 rayIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection);

        glm::vec3 m_normal;
        glm::vec3 m_point;
        float m_distance;

    };

}