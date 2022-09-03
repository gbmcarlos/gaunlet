#include "../include/Scene.h"

class Rendering3DApplication : public gaunlet::Core::Application {

public:
    explicit Rendering3DApplication() : gaunlet::Core::Application() {}

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

    void onReady() override {

        float viewportWidth = (float) gaunlet::Core::Window::getCurrentInstance()->getViewportWidth();
        float viewportHeight = (float) gaunlet::Core::Window::getCurrentInstance()->getViewportHeight();

        m_camera = gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, viewportWidth / viewportHeight, 1.0f, 100.0f);
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

private:
    gaunlet::Scene::Scene m_scene;
    gaunlet::Core::Ref<gaunlet::Scene::PerspectiveCamera> m_camera;
    gaunlet::Scene::DirectionalLightComponent m_directionalLight;
    int m_renderMode = 0;

};

int main() {

    auto window = gaunlet::Core::CreateRef<gaunlet::Core::Window>("Rendering 3D");
    gaunlet::Core::RunLoop runLoop(window);
    Rendering3DApplication app;

    runLoop.run(app);

    return 0;

}