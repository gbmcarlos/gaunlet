#include "../include/Scene.h"

class SceneLayer : public gaunlet::Core::Layer {

private:
    gaunlet::Scene::Scene m_scene;
    gaunlet::Core::Ref<gaunlet::Scene::PerspectiveCamera> m_camera;
    gaunlet::Scene::DirectionalLightComponent m_directionalLight;
    int m_renderMode = 0;

public:

    SceneLayer(float viewportWidth, float viewportHeight) {

        m_camera = gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth /(float) viewportHeight, 1.0f, 100.0f);
        m_camera->setPosition({0, 4, 8});
        m_camera->lookAt({0.0f, 2.0f, 0.0f});

        m_directionalLight = gaunlet::Scene::DirectionalLightComponent(
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(2.0f, 5.0f, 4.0f),
            0.2f, 0.5f
        );

        auto cup = m_scene.getRegistry().createEntity();
        cup.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Model("assets/cup/cup.obj"));
        cup.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 30.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.5f)
        );
        cup.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

        m_scene.start();

    }

    ~SceneLayer() {
        m_scene.stop();
    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {

        gaunlet::Core::RenderCommand::clearColorBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        gaunlet::Core::RenderCommand::clearDepthBuffer();

        if (m_renderMode == 0) {
            m_scene.render(gaunlet::Scene::RenderMode::Faces, m_camera, m_directionalLight);
        } else {
            m_scene.render(gaunlet::Scene::RenderMode::Wireframe, m_camera, m_directionalLight);
        }
    }

    void onGuiRender() override {
        ImGui::ShowMetricsWindow();

        ImGui::Begin("Settings");

        ImGui::RadioButton("Faces", &m_renderMode, 0);
        ImGui::RadioButton("Wireframe", &m_renderMode, 1);

        ImGui::End();

    }

};

class Rendering3DApplication : public gaunlet::Core::Application {

public:
    explicit Rendering3DApplication(const std::string& name) : gaunlet::Core::Application(name) {}

    void onReady() override {
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(),m_window->getViewportHeight());
        pushLayer(m_sceneLayer);
    }

private:
    SceneLayer* m_sceneLayer = nullptr;

};

int main() {

    Rendering3DApplication app("Rendering 3D");
    gaunlet::Core::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}