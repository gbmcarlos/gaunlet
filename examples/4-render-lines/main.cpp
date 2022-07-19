#include <core/Core.h>
#include <graphics/Graphics.h>

#include <graphics/mesh/2d/samples/TriangleMesh.h>
#include <graphics/mesh/2d/samples/SquareMesh.h>

#include <array>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class MainLayer : public engine::Layer {

private:

    std::shared_ptr<engine::VertexArray> m_vertexArray;
    std::shared_ptr<engine::VertexBuffer> m_vertexBuffer;
    std::shared_ptr<engine::IndexBuffer> m_indexBuffer;
    std::shared_ptr<engine::Shader> m_shader;

    std::shared_ptr<engine::OrthographicCamera> m_camera;
    glm::vec2 m_cameraPosition = {0.0f, 0.0f};

    engine::SquareMesh m_quadMesh;
    engine::TriangleMesh m_triangleMesh;

    // Triangle properties
    glm::vec2 m_trianglePosition = {-3.0, -2.0f};
    float m_triangleSize = 2.0f;
    glm::mat4 m_triangleTransform;

    // Quad properties
    glm::vec2 m_quad1Position = {0.0f, 0.0f};
    float m_quadSize = 3.0f;
    glm::mat4 m_quadTransform;

public:

    MainLayer(int viewportWidth, int viewportHeight) {

        m_camera = std::make_shared<engine::OrthographicCamera>(viewportWidth, viewportHeight, 100);

        m_shader = std::make_shared<engine::Shader>();
        m_shader->attach(GL_VERTEX_SHADER, "res/shaders/vertex-position.glsl");
        m_shader->attach(GL_FRAGMENT_SHADER, "res/shaders/fragment-color.glsl");
        m_shader->compile();

        // Prepare the transform matrix of the triangle
        m_triangleTransform =
                glm::translate(glm::mat4(1.0f), glm::vec3(m_trianglePosition, 0.0f)) *
                glm::scale(glm::mat4(1.0f), {m_triangleSize, m_triangleSize, 1.0f})
        ;

        // Prepare the transform matrix of the quad
        m_quadTransform =
            glm::translate(glm::mat4(1.0f), glm::vec3(m_quad1Position, 0.0f)) *
            glm::scale(glm::mat4(1.0f), {m_quadSize, m_quadSize, 1.0f})
            ;

    }

    void onUpdate(engine::TimeStep timeStep) override {

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        engine::Renderer::beginScene(m_camera);
        engine::Renderer::beginBatch(m_shader, engine::MeshElementType::Edge);

        engine::Renderer::submit(m_quadMesh, m_quadTransform);
        engine::Renderer::submit(m_triangleMesh, m_triangleTransform);

        engine::Renderer::flushBatch();
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

    RenderLinesApplication app("Playable Quad");
    engine::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}