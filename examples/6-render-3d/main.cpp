#include <Scene.h>

#include <glm/glm.hpp>

class SceneLayer : public engine::Layer {

private:
    engine::Scene m_scene;

public:

    SceneLayer(float viewportWidth, float viewportHeight) {

        std::shared_ptr<engine::TextureImage2D> texture1 = std::make_shared<engine::TextureImage2D>("assets/texture-1.jpeg");
        std::shared_ptr<engine::TextureImage2D> texture2 = std::make_shared<engine::TextureImage2D>("assets/texture-2.png");

        engine::Entity cube1 = m_scene.createEntity();
        cube1.addComponent<engine::TransformComponent>(
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(20.0f, 110.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        cube1.addComponent<engine::PolygonComponent>(engine::CubeMesh());
        cube1.addComponent<engine::MaterialComponent>(texture1);

        engine::Entity cube2 = m_scene.createEntity();
        cube2.addComponent<engine::TransformComponent>(
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(20.0f, -30.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        cube2.addComponent<engine::PolygonComponent>(engine::CubeMesh());
        cube2.addComponent<engine::MaterialComponent>(texture2);

        auto camera = std::make_shared<engine::PerspectiveCamera>(45.0f, (float) viewportWidth /(float) viewportHeight, 100, 1.0f, 100.0f);
        camera->setTranslation({0.0f, 0.0f, 10.0f});
        m_scene.start(camera);

    }

    ~SceneLayer() {
        m_scene.stop();
    }

    void onUpdate(engine::TimeStep timeStep) override {
        m_scene.onUpdate(timeStep);
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