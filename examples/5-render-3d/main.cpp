#include <Scene.h>

#include <glm/glm.hpp>

class SceneLayer : public engine::Layer {

private:
    engine::Scene m_scene;
    std::shared_ptr<engine::PerspectiveCamera> m_camera;

public:

    SceneLayer(float viewportWidth, float viewportHeight) {

        m_camera = std::make_shared<engine::PerspectiveCamera>(45.0f, (float) viewportWidth /(float) viewportHeight, 100, 1.0f, 100.0f);
        m_camera->setTranslation({0.0f, 0.0f, 10.0f});
        std::shared_ptr<engine::TextureImage2D> texture1 = std::make_shared<engine::TextureImage2D>("assets/texture-1.jpeg");
        std::shared_ptr<engine::TextureImage2D> texture2 = std::make_shared<engine::TextureImage2D>("assets/texture-2.png");

        engine::Entity cube1 = m_scene.createEntity();
        cube1.addComponent<engine::TransformComponent>(
            glm::vec3(-2.0f, 0.0f, 0.0f),
            glm::vec3(20.0f, 110.0f, 0.0f),
            glm::vec3(2.0f, 2.0f, 2.0f)
        );
        cube1.addComponent<engine::PolygonModelComponent>(engine::CubeModel());
        cube1.addComponent<engine::MaterialComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

        engine::Entity cube2 = m_scene.createEntity();
        cube2.addComponent<engine::TransformComponent>(
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(20.0f, -30.0f, 0.0f),
            glm::vec3(2.0f, 2.0f, 2.0f)
        );
        cube2.addComponent<engine::PolygonModelComponent>(engine::CubeModel());
        cube2.addComponent<engine::MaterialComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

        auto camera = std::make_shared<engine::PerspectiveCamera>(45.0f, (float) viewportWidth /(float) viewportHeight, 100, 1.0f, 100.0f);
        camera->setTranslation({0.0f, 0.0f, 10.0f});
        m_scene.start();

    }

    ~SceneLayer() {
        m_scene.stop();
    }

    void onUpdate(engine::TimeStep timeStep) override {
        m_scene.render(m_camera);
    }

};

class Render3DApplication : public engine::Application {

public:
    explicit Render3DApplication(const std::string& name) : engine::Application(name) {}

    void onReady() override {
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(),m_window->getViewportHeight());
        pushLayer(m_sceneLayer);
    }

private:
    SceneLayer* m_sceneLayer = nullptr;

};

int main() {

    Render3DApplication app("Render 3D");
    engine::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}