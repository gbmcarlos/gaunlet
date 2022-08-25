#include "../include/Scene.h"

class SceneLayer : public engine::Core::Layer {

private:
    engine::Scene::Scene m_scene;
    engine::Core::Ref<engine::Scene::PerspectiveCamera> m_camera;
    engine::Scene::DirectionalLightComponent m_directionalLight;

public:

    SceneLayer(float viewportWidth, float viewportHeight) {

        m_camera = engine::Core::CreateRef<engine::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth /(float) viewportHeight, 100, 1.0f, 100.0f);
        m_camera->setTranslation({0.0f, 0.0f, 10.0f});

        m_directionalLight = engine::Scene::DirectionalLightComponent(
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(1.5f, 1.5f, 10.0f),
            0.1f, 0.6f
        );

        auto cup = m_scene.createEntity();
        cup.addComponent<engine::Scene::ModelComponent>(engine::Scene::Model("assets/cup/cup.obj"));
        cup.addComponent<engine::Scene::TransformComponent>(
            glm::vec3(-2.5f, -2.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.5f)
        );
        cup.addComponent<engine::Scene::MaterialComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

        m_scene.start();

    }

    ~SceneLayer() {
        m_scene.stop();
    }

    void onUpdate(engine::Core::TimeStep timeStep) override {
        m_scene.render(m_camera, m_directionalLight);
    }

    void onGuiRender() override {
        ImGui::ShowMetricsWindow();
    }

};

class Rendering3DApplication : public engine::Core::Application {

public:
    explicit Rendering3DApplication(const std::string& name) : engine::Core::Application(name) {}

    void onReady() override {
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(),m_window->getViewportHeight());
        pushLayer(m_sceneLayer);
    }

private:
    SceneLayer* m_sceneLayer = nullptr;

};

int main() {

    Rendering3DApplication app("Rendering 3D");
    engine::Core::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}