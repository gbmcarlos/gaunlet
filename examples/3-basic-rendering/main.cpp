#include <core/Core.h>
#include <graphics/Graphics.h>

#include <graphics/mesh/2d/samples/TriangleMesh.h>
#include <graphics/mesh/2d/samples/SquareMesh.h>

#include <array>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class MainLayer : public engine::Layer {

private:

    // Camera
    std::shared_ptr<engine::OrthographicCamera> m_camera;
    glm::vec2 m_cameraPosition = {0.0f, 0.0f};

    std::shared_ptr<engine::Shader> m_shader;

    engine::SquareMesh m_quadMesh;
    engine::TriangleMesh m_triangleMesh;

public:

    MainLayer(int viewportWidth, int viewportHeight) {

        m_camera = std::make_shared<engine::OrthographicCamera>(viewportWidth, viewportHeight, 100);

        m_shader = std::make_shared<engine::Shader>();
        m_shader->attach(GL_VERTEX_SHADER, "res/shaders/vertex-position.glsl");
        m_shader->attach(GL_FRAGMENT_SHADER, "res/shaders/fragment-color.glsl");
        m_shader->compile();

    }

    void onUpdate(engine::TimeStep timeStep) override {

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        engine::Renderer::beginScene(m_camera);
        submitFaces();
        submitEdges();
        submitPolygons();
        engine::Renderer::endScene();

    }

    void submitFaces() {

        engine::Renderer::beginBatch(m_shader, engine::MeshElementType::Face);

        glm::mat4 triangleTransform = glm::translate(glm::mat4(1.0f), {-5.0f, 2.0f, 0.0f}) *
                                      glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), {0, 0, 1}) *
                                      glm::scale(glm::mat4(1.0f), {1.0f, 1.0f, 1.0f});
        engine::Renderer::submit(m_triangleMesh, triangleTransform);

        glm::mat4 quadTransform = glm::translate(glm::mat4(1.0f), {-5.0f, -2.0f, 0.0f}) *
                                      glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), {0, 0, 1}) *
                                      glm::scale(glm::mat4(1.0f), {1.0f, 1.0f, 1.0f});
        engine::Renderer::submit(m_quadMesh, quadTransform);

        engine::Renderer::flushBatch();

    }


    void submitEdges() {

        engine::Renderer::beginBatch(m_shader, engine::MeshElementType::Edge);

        glm::mat4 triangleTransform = glm::translate(glm::mat4(1.0f), {-0.5f, 2.0f, 0.0f}) *
                                      glm::rotate(glm::mat4(1.0f), glm::radians(-20.0f), {0, 0, 1}) *
                                      glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 1.0f});
        engine::Renderer::submit(m_triangleMesh, triangleTransform);

        glm::mat4 quadTransform = glm::translate(glm::mat4(1.0f), {-0.5f, -2.0f, 0.0f}) *
                                  glm::rotate(glm::mat4(1.0f), glm::radians(-20.0f), {0, 0, 1}) *
                                  glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 1.0f});
        engine::Renderer::submit(m_quadMesh, quadTransform);

        engine::Renderer::flushBatch();

    }


    void submitPolygons() {

        engine::Renderer::beginBatch(m_shader, engine::MeshElementType::Polygon);

        glm::mat4 triangleTransform = glm::translate(glm::mat4(1.0f), {5.0f, 2.0f, 0.0f}) *
                                      glm::rotate(glm::mat4(1.0f), glm::radians(-40.0f), {0, 0, 1}) *
                                      glm::scale(glm::mat4(1.0f), {3.0f, 3.0f, 1.0f});
        engine::Renderer::submit(m_triangleMesh, triangleTransform);

        glm::mat4 quadTransform = glm::translate(glm::mat4(1.0f), {5.0f, -2.0f, 0.0f}) *
                                  glm::rotate(glm::mat4(1.0f), glm::radians(-40.0f), {0, 0, 1}) *
                                  glm::scale(glm::mat4(1.0f), {3.0f, 3.0f, 1.0f});
        engine::Renderer::submit(m_quadMesh, quadTransform);

        engine::Renderer::flushBatch();

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