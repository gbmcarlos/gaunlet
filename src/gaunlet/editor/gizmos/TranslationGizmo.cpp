#include "gaunlet/editor/gizmos/TranslationGizmo.h"

#include "gaunlet/scene/components/GraphicsComponents.h"
#include "gaunlet/editor/Tags.h"
#include "gaunlet/pch.h"

namespace gaunlet::Editor {

    Scene::Entity TranslationGizmo::create(Scene::Registry &registry) {

        auto gizmo = registry.createTaggedEntity<UIEntityTag>();
        gizmo.addComponent<Scene::TransformComponent>();

        createAxis(gizmo, 1.5f, 0.03f);
        createPlanes(gizmo, 0.5f, 0.5f);

        return gizmo;

    }

    void TranslationGizmo::createAxis(Scene::Entity gizmo, float length, float thickness) {

        auto axisX = gizmo.createTaggedChild<UIEntityTag>(convert(Handle::AxisX));
        axisX.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        axisX.addComponent<Scene::TransformComponent>(
            glm::vec3(length/2, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(length, thickness, 1.0f)
        );
        axisX.addComponent<Scene::MaterialComponent>(TranslationGizmo::m_colorX);

        auto axisY = gizmo.createTaggedChild<UIEntityTag>(convert(Handle::AxisY));
        axisY.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        axisY.addComponent<Scene::TransformComponent>(
            glm::vec3(0.0f, length/2, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(thickness, length, 1.0f)
        );
        axisY.addComponent<Scene::MaterialComponent>(TranslationGizmo::m_colorY);

        auto axisZ = gizmo.createTaggedChild<UIEntityTag>(convert(Handle::AxisZ));
        axisZ.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        axisZ.addComponent<Scene::TransformComponent>(
            glm::vec3(0.0f, 0.0f, length/2),
            glm::vec3(0.0f, 90.0f, 0.0f),
            glm::vec3(length, thickness, 1.0f)
        );
        axisZ.addComponent<Scene::MaterialComponent>(TranslationGizmo::m_colorZ);

    }

    void TranslationGizmo::createPlanes(Scene::Entity gizmo, float gap, float size) {

        float baseGap = size/2;
        float finalGap = baseGap + gap;

        auto planeYZ = gizmo.createTaggedChild<UIEntityTag>(convert(Handle::PlaneYZ));
        planeYZ.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        planeYZ.addComponent<Scene::TransformComponent>(
            glm::vec3(0.0f, finalGap, finalGap),
            glm::vec3(0.0f, 90.0f, 00.0f),
            glm::vec3(size, size, 1.0f)
        );
        planeYZ.addComponent<Scene::MaterialComponent>(TranslationGizmo::m_colorX);

        auto planeXZ = gizmo.createTaggedChild<UIEntityTag>(convert(Handle::PlaneXZ));
        planeXZ.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        planeXZ.addComponent<Scene::TransformComponent>(
            glm::vec3(finalGap, 0.0f, finalGap),
            glm::vec3(90.0f, 00.0f, 00.0f),
            glm::vec3(size, size, 1.0f)
        );
        planeXZ.addComponent<Scene::MaterialComponent>(TranslationGizmo::m_colorY);

        auto planeXY = gizmo.createTaggedChild<UIEntityTag>(convert(Handle::PlaneXY));
        planeXY.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        planeXY.addComponent<Scene::TransformComponent>(
            glm::vec3(finalGap, finalGap, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(size, size, 1.0f)
        );
        planeXY.addComponent<Scene::MaterialComponent>(TranslationGizmo::m_colorZ);

    }

    const char* TranslationGizmo::convert(Handle handleType) {

        switch (handleType) {
            case Handle::None:     return "";
            case Handle::AxisX:     return "axis-x";
            case Handle::AxisY:     return "axis-x";
            case Handle::AxisZ:     return "axis-x";
            case Handle::PlaneYZ:   return "plane-yz";
            case Handle::PlaneXZ:   return "plane-xz";
            case Handle::PlaneXY:   return "plane-xy";
        }

        throw std::runtime_error("Unknown handle type");

    }

    TranslationGizmo::Handle TranslationGizmo::convert(const char* handleName) {

        if (handleName == std::string("axis-x")) {
            return TranslationGizmo::Handle::AxisX;
        } else if (handleName == std::string("axis-y")) {
            return TranslationGizmo::Handle::AxisY;
        } else if (handleName == std::string("axis-z")) {
            return TranslationGizmo::Handle::AxisZ;
        } else if (handleName == std::string("plane-yz")) {
            return TranslationGizmo::Handle::PlaneYZ;
        } else if (handleName == std::string("plane-xz")) {
            return TranslationGizmo::Handle::PlaneXZ;
        } else if (handleName == std::string("plane-xy")) {
            return TranslationGizmo::Handle::PlaneXY;
        } else {
            return TranslationGizmo::Handle::None;
        }

    }

}