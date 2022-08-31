#include "gaunlet/editor/gizmos/TranslationGizmo.h"

#include "gaunlet/scene/components/GraphicsComponents.h"
#include "gaunlet/editor/Tags.h"

namespace gaunlet::Editor {

    Scene::Entity TranslationGizmo::create(Scene::Registry &registry) {

        auto gizmo = registry.createTaggedEntity<UIEntityTag>();
        gizmo.addComponent<Scene::TransformComponent>();

        createAxis(gizmo, 1.5f, 0.03f);
        createPlanes(gizmo, 0.5f, 0.5f);

        return gizmo;

    }

    void TranslationGizmo::createAxis(Scene::Entity gizmo, float length, float thickness) {

        auto axisX = gizmo.createTaggedChild<UIEntityTag>();
        axisX.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        axisX.addComponent<Scene::TransformComponent>(
            glm::vec3(length/2, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(length, thickness, 1.0f)
        );
        axisX.addComponent<Scene::MaterialComponent>(TranslationGizmo::m_colorX);

        auto axisY = gizmo.createTaggedChild<UIEntityTag>();
        axisY.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        axisY.addComponent<Scene::TransformComponent>(
            glm::vec3(0.0f, length/2, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(thickness, length, 1.0f)
        );
        axisY.addComponent<Scene::MaterialComponent>(TranslationGizmo::m_colorY);

        auto axisZ = gizmo.createTaggedChild<UIEntityTag>();
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

        auto planeYZ = gizmo.createTaggedChild<UIEntityTag>();
        planeYZ.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        planeYZ.addComponent<Scene::TransformComponent>(
            glm::vec3(0.0f, finalGap, finalGap),
            glm::vec3(0.0f, 90.0f, 00.0f),
            glm::vec3(size, size, 1.0f)
        );
        planeYZ.addComponent<Scene::MaterialComponent>(TranslationGizmo::m_colorX);

        auto planeXZ = gizmo.createTaggedChild<UIEntityTag>();
        planeXZ.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        planeXZ.addComponent<Scene::TransformComponent>(
            glm::vec3(finalGap, finalGap, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(size, size, 1.0f)
        );
        planeXZ.addComponent<Scene::MaterialComponent>(TranslationGizmo::m_colorY);

        auto planeXY = gizmo.createTaggedChild<UIEntityTag>();
        planeXY.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        planeXY.addComponent<Scene::TransformComponent>(
            glm::vec3(finalGap, 0.0f, finalGap),
            glm::vec3(90.0f, 00.0f, 00.0f),
            glm::vec3(size, size, 1.0f)
        );
        planeXY.addComponent<Scene::MaterialComponent>(TranslationGizmo::m_colorZ);

    }

}