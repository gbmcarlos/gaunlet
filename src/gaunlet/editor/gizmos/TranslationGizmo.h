#pragma once

#include "gaunlet/scene/entity/Entity.h"

namespace gaunlet::Editor {

    class TranslationGizmo {

    public:

        enum class Handle {
            None,
            AxisX, AxisY, AxisZ,
            PlaneYZ, PlaneXZ, PlaneXY
        };

        static Scene::Entity create(const Core::Ref<Scene::Scene>& scene, float axisLength, float axisThickness, float planeGap, float planeSize);
        static const char* convert(Handle handleType);
        static Handle convert(const char* handleName);

        static glm::vec3 getPlaneNormal(Handle handleType);
        static glm::vec3 constraintMovement(Handle handleType, glm::vec3 movement);
        static glm::vec3 getMovementConstraint(Handle handleType);

    private:

        constexpr static const glm::vec4 m_colorX = {1.0f, 0.0f, 0.0f, 1.0f};
        constexpr static const glm::vec4 m_colorY = {0.0f, 1.0f, 0.0f, 1.0f};
        constexpr static const glm::vec4 m_colorZ = {0.0f, 0.0f, 1.0f, 1.0f};

        static void createAxis(Scene::Entity gizmo, float length, float thickness);
        static void createPlanes(Scene::Entity gizmo, float gap, float size);

    };

}