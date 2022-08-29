#include "../include/Scene.h"

class SceneLayer : public gaunlet::Core::Layer {

private:

    gaunlet::Scene::Scene m_mainScene;
    gaunlet::Core::Ref<gaunlet::Scene::OrthographicCamera> m_camera;

public:

    SceneLayer(int viewportWidth, int viewportHeight) {

        m_camera = gaunlet::Core::CreateRef<gaunlet::Scene::OrthographicCamera>((float) viewportWidth, (float) viewportHeight, 100);
        gaunlet::Core::Ref<gaunlet::Graphics::TextureImage2D> texture1 = gaunlet::Core::CreateRef<gaunlet::Graphics::TextureImage2D>("assets/texture-1.jpeg");
        gaunlet::Core::Ref<gaunlet::Graphics::TextureImage2D> texture2 = gaunlet::Core::CreateRef<gaunlet::Graphics::TextureImage2D>("assets/texture-2.jpeg");

        // TRIANGLE 1
        auto triangle1 = m_mainScene.getRegistry().createEntity();
        triangle1.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Triangle2DModel());
        triangle1.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-5.0f, 3.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        triangle1.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));

        // TRIANGLE 2
        auto triangle2 = m_mainScene.getRegistry().createEntity();
        triangle2.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Triangle2DModel());
        triangle2.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-5.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, -20.0f),
            glm::vec3(1.2f, 1.2f, 1.0f)
        );
        triangle2.addComponent<gaunlet::Scene::MaterialComponent>(texture1);

        // TRIANGLE 3
        auto triangle3 = m_mainScene.getRegistry().createEntity();
        triangle3.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Triangle2DModel());
        triangle3.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-5.0f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -40.0f),
            glm::vec3(1.8f, 1.8f, 1.0f)
        );
        triangle3.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f), texture2);

        // SQUARE 1
        auto square1 = m_mainScene.getRegistry().createEntity();
        square1.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Square2DModel());
        square1.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, 3.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        square1.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.8f, 0.0f, 1.0f));

        // SQUARE 2
        auto square2 = m_mainScene.getRegistry().createEntity();
        square2.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Square2DModel());
        square2.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, -20.0f),
            glm::vec3(1.2f, 1.2f, 1.0f)
        );
        square2.addComponent<gaunlet::Scene::MaterialComponent>(texture1);

        // SQUARE 3
        auto square3 = m_mainScene.getRegistry().createEntity();
        square3.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Square2DModel());
        square3.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -40.0f),
            glm::vec3(1.8f, 1.8f, 1.0f)
        );
        square3.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f), texture2);

        // CIRCLE 1
        auto circle1 = m_mainScene.getRegistry().createEntity();
        circle1.addComponent<gaunlet::Scene::CircleComponent>(0.1f, 0.01f);
        circle1.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(5.0f, 3.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        circle1.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

        // CIRCLE 2
        auto circle2 = m_mainScene.getRegistry().createEntity();
        circle2.addComponent<gaunlet::Scene::CircleComponent>(0.5, 0.1f);
        circle2.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(5.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, -20.0f),
            glm::vec3(1.5f, 1.2f, 1.0f)
        );
        circle2.addComponent<gaunlet::Scene::MaterialComponent>(texture1);

        // CIRCLE 3
        auto circle3 = m_mainScene.getRegistry().createEntity();
        circle3.addComponent<gaunlet::Scene::CircleComponent>(1.0f, 0.3f);
        circle3.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(5.0f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -40.0f),
            glm::vec3(1.8f, 2.2f, 1.0f)
        );
        circle3.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.8f, 0.0f, 1.0f), texture2);

        gaunlet::Core::Ref<gaunlet::Scene::OrthographicCamera> camera = gaunlet::Core::CreateRef<gaunlet::Scene::OrthographicCamera>((float) viewportWidth, (float) viewportHeight, 100);
        m_mainScene.start();

    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {
        gaunlet::Core::RenderCommand::clearColorBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        gaunlet::Core::RenderCommand::clearDepthBuffer();
        m_mainScene.render(gaunlet::Scene::RenderMode::Faces, m_camera, gaunlet::Scene::DirectionalLightComponent());
    }

};


class Rendering2DApplication : public gaunlet::Core::Application {

public:
    explicit Rendering2DApplication(const std::string &name) : gaunlet::Core::Application(name) {}

    void onReady() override {
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(), m_window->getViewportHeight());
        pushLayer(m_sceneLayer);
    }

private:
    SceneLayer* m_sceneLayer = nullptr;

};

int main() {

    Rendering2DApplication app("Rendering 2D");
    gaunlet::Core::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}