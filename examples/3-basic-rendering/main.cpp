#include <Scene.h>

#include "glm/glm.hpp"

#include <array>

class SceneLayer : public engine::Layer {

private:

    engine::Scene m_mainScene;
    engine::Ref<engine::OrthographicCamera> m_camera;

public:

    SceneLayer(int viewportWidth, int viewportHeight) {

        m_camera = engine::CreateRef<engine::OrthographicCamera>((float) viewportWidth, (float) viewportHeight, 100);
        engine::Ref<engine::TextureImage2D> texture1 = engine::CreateRef<engine::TextureImage2D>("assets/texture-1.jpeg");
        engine::Ref<engine::TextureImage2D> texture2 = engine::CreateRef<engine::TextureImage2D>("assets/texture-2.jpeg");

        // TRIANGLE 1
        auto triangle1 = m_mainScene.createEntity();
        triangle1.addComponent<engine::PolygonModelComponent>(engine::Triangle2DModel());
        triangle1.addComponent<engine::TransformComponent>(
            glm::vec3(-5.0f, 3.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        triangle1.addComponent<engine::MaterialComponent>(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));

        // TRIANGLE 2
        auto triangle2 = m_mainScene.createEntity();
        triangle2.addComponent<engine::PolygonModelComponent>(engine::Triangle2DModel());
        triangle2.addComponent<engine::TransformComponent>(
            glm::vec3(-5.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, -20.0f),
            glm::vec3(1.2f, 1.2f, 1.0f)
        );
        triangle2.addComponent<engine::MaterialComponent>(texture1);

        // TRIANGLE 3
        auto triangle3 = m_mainScene.createEntity();
        triangle3.addComponent<engine::PolygonModelComponent>(engine::Triangle2DModel());
        triangle3.addComponent<engine::TransformComponent>(
            glm::vec3(-5.0f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -40.0f),
            glm::vec3(1.8f, 1.8f, 1.0f)
        );
        triangle3.addComponent<engine::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f), texture2);

        // SQUARE 1
        auto square1 = m_mainScene.createEntity();
        square1.addComponent<engine::PolygonModelComponent>(engine::Square2DModel());
        square1.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, 3.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        square1.addComponent<engine::MaterialComponent>(glm::vec4(0.0f, 0.8f, 0.0f, 1.0f));

        // SQUARE 2
        auto square2 = m_mainScene.createEntity();
        square2.addComponent<engine::PolygonModelComponent>(engine::Square2DModel());
        square2.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, -20.0f),
            glm::vec3(1.2f, 1.2f, 1.0f)
        );
        square2.addComponent<engine::MaterialComponent>(texture1);

        // SQUARE 3
        auto square3 = m_mainScene.createEntity();
        square3.addComponent<engine::PolygonModelComponent>(engine::Square2DModel());
        square3.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -40.0f),
            glm::vec3(1.8f, 1.8f, 1.0f)
        );
        square3.addComponent<engine::MaterialComponent>(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f), texture2);

        // CIRCLE 1
        auto circle1 = m_mainScene.createEntity();
        circle1.addComponent<engine::CircleModelComponent>(0.1f, 0.01f);
        circle1.addComponent<engine::TransformComponent>(
            glm::vec3(5.0f, 3.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        circle1.addComponent<engine::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

        // CIRCLE 2
        auto circle2 = m_mainScene.createEntity();
        circle2.addComponent<engine::CircleModelComponent>(0.5, 0.1f);
        circle2.addComponent<engine::TransformComponent>(
            glm::vec3(5.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, -20.0f),
            glm::vec3(1.5f, 1.2f, 1.0f)
        );
        circle2.addComponent<engine::MaterialComponent>(texture1);

        // CIRCLE 3
        auto circle3 = m_mainScene.createEntity();
        circle3.addComponent<engine::CircleModelComponent>(1.0f, 0.3f);
        circle3.addComponent<engine::TransformComponent>(
            glm::vec3(5.0f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -40.0f),
            glm::vec3(1.8f, 2.2f, 1.0f)
        );
        circle3.addComponent<engine::MaterialComponent>(glm::vec4(0.0f, 0.8f, 0.0f, 1.0f), texture2);

        engine::Ref<engine::OrthographicCamera> camera = engine::CreateRef<engine::OrthographicCamera>((float) viewportWidth, (float) viewportHeight, 100);
        m_mainScene.start();

    }

    void onUpdate(engine::TimeStep timeStep) override {
        m_mainScene.render(m_camera);
    }

};


class ViewportLayoutApplication : public engine::Application {

public:
    explicit ViewportLayoutApplication(const std::string &name) : engine::Application(name) {}

    void onReady() override {
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(), m_window->getViewportHeight());
        pushLayer(m_sceneLayer);
    }

private:
    SceneLayer* m_sceneLayer = nullptr;

};

int main() {

    ViewportLayoutApplication app("Basic Rendering");
    engine::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}