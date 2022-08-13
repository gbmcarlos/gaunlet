#include <Scene.h>

#include "glm/glm.hpp"

#include <array>

class MainLayer : public engine::Layer {

public:

    engine::Scene m_mainScene;
    std::shared_ptr<engine::OrthographicCamera> m_camera;
    std::shared_ptr<engine::Framebuffer> m_framebuffer = nullptr;

public:

    MainLayer(unsigned int viewportWidth, unsigned int viewportHeight) {

        m_camera = std::make_shared<engine::OrthographicCamera>(viewportWidth, viewportHeight, 100);

        m_framebuffer = std::make_shared<engine::Framebuffer>(std::initializer_list<engine::FramebufferAttachmentSpec>{
            {engine::FramebufferDataFormat::RGBA, engine::FramebufferAttachmentType::Color},
            {engine::FramebufferDataFormat::Depth, engine::FramebufferAttachmentType::Depth}
        }, viewportWidth, viewportHeight);

        auto triangle1 = m_mainScene.createEntity();
        triangle1.addComponent<engine::PolygonComponent>(engine::TriangleMesh());
        triangle1.addComponent<engine::TransformComponent>(
            glm::vec3(-0.5f, 0.0f, 0.5f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 2.0f, 1.0f)
        );
        triangle1.addComponent<engine::MaterialComponent>(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

        auto triangle2 = m_mainScene.createEntity();
        triangle2.addComponent<engine::PolygonComponent>(engine::TriangleMesh());
        triangle2.addComponent<engine::TransformComponent>(
            glm::vec3(0.5f, 0.0f, 0.6f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 2.0f, 1.0f)
        );
        triangle2.addComponent<engine::MaterialComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        m_mainScene.start();

    }

    void onUpdate(engine::TimeStep timeStep) override {
        m_mainScene.render(m_camera, m_framebuffer);
    }

    void onGuiRender() override {

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Scene");
        ImVec2 windowSize = ImGui::GetContentRegionAvail();

        if (m_framebuffer->getWidth() != windowSize.x || m_framebuffer->getHeight() != windowSize.y) {
            m_framebuffer->resize((unsigned int) windowSize.x, (unsigned int) windowSize.y);
            m_camera->resize((unsigned int) windowSize.x, (unsigned int) windowSize.y);
        }

        // Render the framebuffer's texture to the "Scene" window
        engine::ViewportLayout::renderFramebuffer(m_framebuffer,0);
        ImGui::End();
        ImGui::PopStyleVar(1);
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
                {engine::DockSpacePosition::Left, 0.3f,  {"Dear ImGui Demo"}, ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResizeFlagsMask_},
                {engine::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResizeFlagsMask_}
            },
            {0, 0},
            {m_window->getViewportWidth(), m_window->getViewportHeight()}
        );

        // Init the layout
        layout.begin("Main Window");
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

    BasicRenderingApplication app("Viewports");
    engine::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}