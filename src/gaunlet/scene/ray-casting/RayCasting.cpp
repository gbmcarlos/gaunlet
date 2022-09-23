#include "gaunlet/scene/ray-casting/RayCasting.h"

namespace gaunlet::Scene {

    Plane::Plane()
        : Plane({0, 1, 0}, {0, 0, 0}) {}

    Plane::Plane(glm::vec3 normal, glm::vec3 point)
        : m_normal(glm::normalize(normal)), m_point(point), m_distance(glm::dot(m_normal, point)) {}

    bool Plane::pointOver(const glm::vec3 &point) {

        float distance = glm::dot(m_normal, point) - m_distance;
        return distance > 0;

    }

    bool Plane::polygonOver(const std::vector<glm::vec3>& points) {

        for (auto& point : points) {
            if (pointOver(point)) {
                return true;
            }
        }

        return false;

    }

    glm::vec3 Plane::rayIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) {

        float distance;

        bool intersects = glm::intersectRayPlane(
            rayOrigin, rayDirection,
            m_point, m_normal,
            distance
        );

        if (intersects) {
            return rayOrigin + (rayDirection * distance);
        } else {
            return {NAN, NAN, NAN};
        }

    }

}