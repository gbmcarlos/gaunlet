#pragma once

#include "gaunlet/scene/components/BasicComponents.h"
#include "gaunlet/prefab/sprites/Sprites.h"

namespace gaunlet::Prefab::Components {

    struct CircleComponent : public Scene::ContentComponent {

        CircleComponent(const CircleComponent&) = default;

        // A circle is drawn as a square (a single mesh), with a different shader
        CircleComponent() :
            CircleComponent(0.5, 0.01f) {}

        explicit CircleComponent(float thickness) :
            CircleComponent(thickness, 0.01f) {}

        CircleComponent(float thickness, float fade) :
            m_mesh(Prefab::Sprites::Square2DModel().getMeshes()[0]),
            m_thickness(thickness),
            m_fade(fade) {}

        std::tuple<std::vector<Graphics::Vertex>, std::vector<unsigned int>> getContent() override {
            return {m_mesh.getVertices(), m_mesh.getIndices()};
        }

        Scene::Mesh m_mesh;
        float m_thickness;
        float m_fade;

    };

}