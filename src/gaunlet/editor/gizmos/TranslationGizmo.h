#pragma once

#include "gaunlet/scene/entity/Entity.h"

namespace gaunlet::Editor {

    class TranslationGizmo {

    public:

        static Scene::Entity create(Scene::Registry& registry);

    private:

        constexpr static const glm::vec4 m_colorX = {1.0f, 0.0f, 0.0f, 1.0f};
        constexpr static const glm::vec4 m_colorY = {0.0f, 1.0f, 0.0f, 1.0f};
        constexpr static const glm::vec4 m_colorZ = {0.0f, 0.0f, 1.0f, 1.0f};

        static void createAxis(Scene::Entity gizmo, float length, float thickness);
        static void createPlanes(Scene::Entity gizmo, float gap, float size);

    };

}