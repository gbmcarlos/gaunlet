#include "../include/Scene.h"

class Rendering3DApplication : public gaunlet::Core::Application {

public:
    explicit Rendering3DApplication() : gaunlet::Core::Application() {}

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {

        gaunlet::Core::RenderCommand::clearColorBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        gaunlet::Core::RenderCommand::clearDepthBuffer();

        if (m_renderMode == 0) {
            m_scene.render(gaunlet::Scene::RenderMode::Faces, m_camera, m_directionalLight, m_skybox);
        } else {
            m_scene.render(gaunlet::Scene::RenderMode::Wireframe, m_camera, m_directionalLight, m_skybox);
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

        auto viewportWidth = (float) gaunlet::Core::Window::getCurrentInstance()->getViewportWidth();
        auto viewportHeight = (float) gaunlet::Core::Window::getCurrentInstance()->getViewportHeight();

        m_camera = gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, viewportWidth / viewportHeight, 1.0f, 100.0f);
        m_camera->setPosition({0, 4, 8});
        m_camera->lookAt({0.0f, 2.0f, 0.0f});

        m_directionalLight = gaunlet::Core::CreateRef<gaunlet::Scene::DirectionalLightComponent>(
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(2.0f, 5.0f, 4.0f),
            0.2f, 0.5f
        );

        m_skybox = gaunlet::Core::CreateRef<gaunlet::Scene::SkyboxComponent>(std::vector<const char*>{
            "assets/skybox/right.jpg",
            "assets/skybox/left.jpg",
            "assets/skybox/top.jpg",
            "assets/skybox/bottom.jpg",
            "assets/skybox/front.jpg",
            "assets/skybox/back.jpg"
        });

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
    int m_renderMode = 0;
    gaunlet::Scene::Scene m_scene;
    gaunlet::Core::Ref<gaunlet::Scene::PerspectiveCamera> m_camera;
    gaunlet::Core::Ref<gaunlet::Scene::DirectionalLightComponent> m_directionalLight;
    gaunlet::Core::Ref<gaunlet::Scene::SkyboxComponent> m_skybox;

};

int main() {

    auto window = gaunlet::Core::CreateRef<gaunlet::Core::Window>("Rendering 3D");
    gaunlet::Core::RunLoop runLoop(window);
    Rendering3DApplication app;

    runLoop.run(app);

    return 0;

}