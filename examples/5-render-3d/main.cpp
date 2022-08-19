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

        auto cup = m_scene.createEntity();
        cup.addComponent<engine::PolygonModelComponent>(engine::Model("assets/cup/cup.obj"));
        cup.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.5f)
        );
        cup.addComponent<engine::MaterialComponent>(glm::vec4(0.8f, 1.0f, 1.0f, 1.0f));

        m_scene.start();

    }

    ~SceneLayer() {
        m_scene.stop();
    }

    void onUpdate(engine::TimeStep timeStep) override {
        m_scene.render(m_camera);
    }

    void onGuiRender() override {
        ImGui::ShowMetricsWindow();
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