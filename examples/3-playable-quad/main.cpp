#include <core/Core.h>
#include <graphics/Graphics.h>

#include <graphics/mesh/2d/samples/TriangleMesh.h>
#include <graphics/mesh/2d/samples/SquareMesh.h>

#include <array>
#include <iostream>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

class MainLayer : public engine::Layer {

public:
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
    float m_quadTargetSpeed = m_quadSize * 3;
    glm::vec2 m_quad1Speed = {0.0f, 0.0f};

    MainLayer(float viewportWidth, float viewportHeight) {

        m_camera = std::make_shared<engine::OrthographicCamera>(viewportWidth, viewportHeight, 100);

        m_shader = std::make_shared<engine::Shader>();
        m_shader->attach(GL_VERTEX_SHADER, "res/shaders/vertex-position.glsl");
        m_shader->attach(GL_FRAGMENT_SHADER, "res/shaders/fragment-color.glsl");
        m_shader->compile();

        // Prepare the transform matrix of the quad
        m_triangleTransform =
            glm::translate(glm::mat4(1.0f), glm::vec3(m_trianglePosition, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), {0, 0, 1}) *
            glm::scale(glm::mat4(1.0f), {m_triangleSize, m_triangleSize, 1.0f})
        ;

    }

    void onUpdate(engine::TimeStep timeStep) override {

        // Update the position of the quad based on its speed
        m_quad1Position.x += m_quad1Speed.x * timeStep;
        m_quad1Position.y += m_quad1Speed.y * timeStep;

        // Prepare the transform matrix of the quad
        glm::mat4 quadTransform =
            glm::translate(glm::mat4(1.0f), glm::vec3(m_quad1Position, 0.0f)) *
            glm::scale(glm::mat4(1.0f), {m_quadSize, m_quadSize, 1.0f})
        ;

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        engine::Renderer::beginScene(m_camera);
        engine::Renderer::beginBatch(m_shader, engine::MeshElementType::Face);

        engine::Renderer::submit(m_quadMesh, quadTransform);
        engine::Renderer::submit(m_triangleMesh, m_triangleTransform);

        engine::Renderer::flushBatch();
        engine::Renderer::endScene();

    }

    void onEvent(engine::Event& event) override {

        engine::EventDispatcher dispatcher(event);

        dispatcher.dispatch<engine::WindowResizeEvent>(GE_BIND_CALLBACK_FN(MainLayer::onWindowResizeEvent));
        dispatcher.dispatch<engine::KeyPressEvent>(GE_BIND_CALLBACK_FN(MainLayer::onKeyPressEvent));
        dispatcher.dispatch<engine::KeyReleaseEvent>(GE_BIND_CALLBACK_FN(MainLayer::onKeyReleaseEvent));
        dispatcher.dispatch<engine::MouseButtonPress>(GE_BIND_CALLBACK_FN(MainLayer::onMouseButtonPressEvent));
        dispatcher.dispatch<engine::MouseButtonRelease>(GE_BIND_CALLBACK_FN(MainLayer::onMouseButtonReleaseEvent));
        dispatcher.dispatch<engine::CursorMoveEvent>(GE_BIND_CALLBACK_FN(MainLayer::onCursorMoveEvent));
        dispatcher.dispatch<engine::ScrollEvent>(GE_BIND_CALLBACK_FN(MainLayer::onScrollEvent));

    }

    bool onMouseButtonPressEvent(const engine::MouseButtonPress& event) {
        std::cout << "mouse button pressed, button:  " << event.getButton() << std::endl;
        return false;
    }

    bool onMouseButtonReleaseEvent(engine::MouseButtonRelease& event) {
        std::cout << "mouse button pressed, button:  " << event.getButton() << std::endl;
        return false;
    }

    bool onCursorMoveEvent(engine::CursorMoveEvent& event) {
        std::cout << "cursor move, x: " << event.getXPosition() << ", y: " << event.getYPosition() << std::endl;
        return false;
    }

    bool onScrollEvent(engine::ScrollEvent& event) {
        std::cout << "scroll, x: " << event.getXOffset() << ", y: " << event.getXOffset() << std::endl;
        return false;
    }

    bool onWindowResizeEvent(engine::WindowResizeEvent& event) {
        m_camera->onWindowResize(event.getWidth(), event.getHeight());
        return false;
    }

    bool onKeyPressEvent(engine::KeyPressEvent& event) {

        if (event.getKey() == GE_KEY_UP) {
            m_camera->addZoomLevel(0.1f);
            return true;
        } else if (event.getKey() == GE_KEY_DOWN) {
            m_camera->addZoomLevel(-0.1f);
            return true;
        }

        if (event.getKey() == GE_KEY_RIGHT) {
            m_quad1Speed.x = m_quadTargetSpeed;
            return true;
        } else if (event.getKey() == GE_KEY_LEFT) {
            m_quad1Speed.x = -m_quadTargetSpeed;
            return true;
        }

        return false;

    }

    bool onKeyReleaseEvent(engine::KeyReleaseEvent& event) {

        if ((event.getKey() == GE_KEY_RIGHT && m_quad1Speed.x > 0) || (event.getKey() == GE_KEY_LEFT && m_quad1Speed.x < 0) ) {
            m_quad1Speed.x = 0;
            return true;
        }

        if ((event.getKey() == GE_KEY_UP && m_quad1Speed.y > 0) || (event.getKey() == GE_KEY_DOWN && m_quad1Speed.y < 0) ) {
            m_quad1Speed.y = 0;
            return true;
        }

        return false;

    }

};

class PlayableQuadApplication : public engine::Application {

public:
    explicit PlayableQuadApplication(const std::string& name) : engine::Application(name) {}

private:

    MainLayer* m_mainLayer;

public:

    void onReady() override {
        m_mainLayer = new MainLayer(m_window->getViewportWidth(), m_window->getViewportHeight());
        pushLayer(m_mainLayer);
    }

};

int main() {

    PlayableQuadApplication app("Playable Quad");
    engine::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}