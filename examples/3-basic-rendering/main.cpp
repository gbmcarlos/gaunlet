#include <Core.h>
#include <Graphics.h>
#include <Scene.h>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <array>

class MainLayer : public engine::Layer {

private:

    // Camera
    std::shared_ptr<engine::OrthographicCamera> m_camera;
    glm::vec2 m_cameraPosition = {0.0f, 0.0f};

    engine::Scene m_scene;

    engine::SquareMesh m_quadMesh;
    engine::TriangleMesh m_triangleMesh;

public:

    MainLayer(int viewportWidth, int viewportHeight) {

        engine::Renderer::init();
        m_camera = std::make_shared<engine::OrthographicCamera>(viewportWidth, viewportHeight, 100);

        // TRIANGLE 1
        auto triangle1 = m_scene.createEntity();
        triangle1.addComponent<engine::MeshComponent>(m_triangleMesh);
        triangle1.addComponent<engine::TransformComponent>(
            glm::vec3(-5.0f, 2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        triangle1.addComponent<engine::MaterialComponent>(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));

        // TRIANGLE 2
        auto triangle2 = m_scene.createEntity();
        triangle2.addComponent<engine::MeshComponent>(m_triangleMesh);
        triangle2.addComponent<engine::TransformComponent>(
            glm::vec3(-0.5f, 2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -20.0f),
            glm::vec3(2.0f, 2.0f, 1.0f)
        );
        triangle2.addComponent<engine::MaterialComponent>(glm::vec4(0.0f, 0.8f, 0.0f, 1.0f));

        // TRIANGLE 3
        auto triangle3 = m_scene.createEntity();
        triangle3.addComponent<engine::MeshComponent>(m_triangleMesh);
        triangle3.addComponent<engine::TransformComponent>(
            glm::vec3(5.0f, 2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -40.0f),
            glm::vec3(3.0f, 3.0f, 1.0f)
        );
        triangle3.addComponent<engine::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

        // SQUARE 1
        auto square1 = m_scene.createEntity();
        square1.addComponent<engine::MeshComponent>(m_quadMesh);
        square1.addComponent<engine::TransformComponent>(
            glm::vec3(-5.0f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        square1.addComponent<engine::MaterialComponent>(glm::vec4(0.0f, 0.8f, 0.0f, 1.0f));

        // SQUARE 2
        auto square2 = m_scene.createEntity();
        square2.addComponent<engine::MeshComponent>(m_quadMesh);
        square2.addComponent<engine::TransformComponent>(
            glm::vec3(-0.5f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -20.0f),
            glm::vec3(2.0f, 2.0f, 1.0f)
        );
        square2.addComponent<engine::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

        // SQUARE 3
        auto square3 = m_scene.createEntity();
        square3.addComponent<engine::MeshComponent>(m_quadMesh);
        square3.addComponent<engine::TransformComponent>(
            glm::vec3(5.0f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -40.0f),
            glm::vec3(3.0f, 3.0f, 1.0f)
        );
        square3.addComponent<engine::MaterialComponent>(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));

    }

    void onUpdate(engine::TimeStep timeStep) override {

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        engine::Renderer::beginScene(m_camera);
        m_scene.onUpdate(timeStep);
        engine::Renderer::endScene();

    }

};


class RenderLinesApplication : public engine::Application {

public:
    RenderLinesApplication(const std::string &name) : engine::Application(name) {}

    void onReady() override {
        m_mainLayer = new MainLayer(m_window->getViewportWidth(), m_window->getViewportHeight());
        pushLayer(m_mainLayer);

    }

private:
    MainLayer* m_mainLayer;

};

int main() {

    RenderLinesApplication app("Basic Rendering");
    engine::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}