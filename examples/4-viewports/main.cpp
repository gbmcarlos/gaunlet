#include <Scene.h>

#include "glm/glm.hpp"

#include <array>

class MainLayer : public engine::Layer {

private:

    engine::Scene m_scene;

public:

    MainLayer(int viewportWidth, int viewportHeight) {

        std::shared_ptr<engine::Framebuffer> framebuffer = std::make_shared<engine::Framebuffer>(std::initializer_list<engine::FramebufferAttachmentSpecs>{
            {engine::TextureDataFormat::RGBA, engine::TextureType::Image2D, (float) viewportWidth, (float) viewportHeight, engine::FramebufferAttachmentType::Color, true}
        });

        // TRIANGLE 1
        auto triangle1 = m_scene.createEntity();
        triangle1.addComponent<engine::PolygonComponent>(engine::TriangleMesh());
        triangle1.addComponent<engine::TransformComponent>(
            glm::vec3(-5.0f, 3.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        triangle1.addComponent<engine::MaterialComponent>(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));

        auto camera = std::make_shared<engine::OrthographicCamera>((float) viewportWidth, (float) viewportHeight, 100);
        m_scene.start(camera);

    }

    void onUpdate(engine::TimeStep timeStep) override {
        m_scene.onUpdate(timeStep);
    }

};


class BasicRenderingApplication : public engine::Application {

public:
    explicit BasicRenderingApplication(const std::string &name) : engine::Application(name) {}

    void onReady() override {
        m_mainLayer = new MainLayer(m_window->getViewportWidth(), m_window->getViewportHeight());
        pushLayer(m_mainLayer);
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