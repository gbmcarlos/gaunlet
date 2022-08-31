#include "gaunlet/editor/gizmos/TranslationGizmo.h"

#include "gaunlet/scene/components/GraphicsComponents.h"
#include "gaunlet/editor/Tags.h"

namespace gaunlet::Editor {

    Scene::Entity TranslationGizmo::create(Scene::Registry &registry) {

        auto gizmo = registry.createTaggedEntity<UIEntityTag>();
        gizmo.addComponent<Scene::TransformComponent>();

        auto planeXY = gizmo.createTaggedChild<UIEntityTag>();
        planeXY.addComponent<Scene::ModelComponent>(Scene::Square2DModel());
        planeXY.addComponent<Scene::TransformComponent>(
            glm::vec3(0.2f, 0.2f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 1.0f)
        );
        planeXY.addComponent<Scene::MaterialComponent>(glm::vec4(0.1f, 0.8f, 0.1f, 1.0f));

        return gizmo;

    }

}