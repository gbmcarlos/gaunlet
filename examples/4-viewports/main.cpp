#include <Scene.h>

#include "glm/glm.hpp"

#include <array>

class MainLayer : public engine::Layer {

public:

    engine::Scene m_mainScene;
    std::shared_ptr<engine::OrthographicCamera> m_camera;
    std::shared_ptr<engine::Framebuffer> m_framebuffer = nullptr;

public:

    MainLayer(int viewportWidth, int viewportHeight) {

        m_camera = std::make_shared<engine::OrthographicCamera>((float) viewportWidth, (float) viewportHeight, 100);

        m_framebuffer = std::make_shared<engine::Framebuffer>(std::initializer_list<engine::FramebufferAttachmentSpecs>{
            {engine::TextureDataFormat::RGBA, engine::TextureType::Image2D, (float) viewportWidth, (float) viewportHeight, engine::FramebufferAttachmentType::Color, true}
        });

        auto triangle = m_mainScene.createEntity();
        triangle.addComponent<engine::PolygonComponent>(engine::TriangleMesh());
        triangle.addComponent<engine::TransformComponent>(
            glm::vec3(-4.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(5.0f, 5.0f, 1.0f)
        );
        triangle.addComponent<engine::MaterialComponent>(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

        m_mainScene.start();

    }

    void onUpdate(engine::TimeStep timeStep) override {
        m_mainScene.onUpdate(timeStep, m_camera, m_framebuffer);
    }

    void onGuiRender() override {
        // Render the framebuffer's texture to the "Scene" window
        engine::ViewportLayout::renderTexture("Scene", m_framebuffer->getTextures()[0]);
    }

};


class BasicRenderingApplication : public engine::Application {

public:
    explicit BasicRenderingApplication(const std::string &name) : engine::Application(name) {}

    void onReady() override {
        m_mainLayer = new MainLayer(m_window->getViewportWidth(), m_window->getViewportHeight());
        pushLayer(m_mainLayer);
    }

    void onGuiRender() override {

        engine::ViewportLayout layout(
            {
                {engine::DockSpacePosition::Left,   0.3f,  {"Dear ImGui Demo"}},
                {engine::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResizeFlagsMask_}
            },
            {0, 0},
            {m_window->getViewportWidth(), m_window->getViewportHeight()}
        );

        // Init the layout
        layout.begin("Main Window");
        // Here we can add stuff to the main window
        layout.end();

        // Left dock, with the demo window
        ImGui::ShowDemoWindow();

        // Delegate to the layers
        engine::Application::onGuiRender();

    }

private:
    MainLayer* m_mainLayer = nullptr;

};

int main() {

    BasicRenderingApplication app("Basic Rendering");
    engine::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}