#include <core/Core.h>
#include <graphics/Graphics.h>

#include <graphics/mesh/2d/samples/SquareMesh.h>

#include <imgui.h>

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraControlLayer : public engine::Layer {

public:

    std::shared_ptr<engine::OrthographicCamera>& m_camera;

    glm::vec2 m_cameraSpeed;

    explicit CameraControlLayer(std::shared_ptr<engine::OrthographicCamera>& camera) : m_camera(camera), m_cameraSpeed({0.0f, 0.0f}) {}

    void onUpdate(engine::TimeStep timeStep) override {

        m_camera->movePosition({m_cameraSpeed.x * timeStep, m_cameraSpeed.y * timeStep, 0.0f});

    }

    void onEvent(engine::Event &event) override {

        engine::EventDispatcher dispatcher(event);

        dispatcher.dispatch<engine::KeyPressEvent>(GE_BIND_CALLBACK_FN(CameraControlLayer::onKeyPressEvent));
        dispatcher.dispatch<engine::KeyReleaseEvent>(GE_BIND_CALLBACK_FN(CameraControlLayer::onKeyReleaseEvent));

    }

    bool onKeyPressEvent(engine::KeyPressEvent& event) {

        if (event.getKey() == GE_KEY_UP) {
            m_cameraSpeed.y = 10;
            return true;
        } else if (event.getKey() == GE_KEY_DOWN) {
            m_cameraSpeed.y = -10;
            return true;
        }

        if (event.getKey() == GE_KEY_RIGHT) {
            m_cameraSpeed.x = 10;
            return true;
        } else if (event.getKey() == GE_KEY_LEFT) {
            m_cameraSpeed.x = -10;
            return true;
        }

        return false;

    }

    bool onKeyReleaseEvent(engine::KeyReleaseEvent& event) {

        if ((event.getKey() == GE_KEY_RIGHT && m_cameraSpeed.x > 0) || (event.getKey() == GE_KEY_LEFT && m_cameraSpeed.x < 0) ) {
            m_cameraSpeed.x = 0;
            return true;
        }

        if ((event.getKey() == GE_KEY_UP && m_cameraSpeed.y > 0) || (event.getKey() == GE_KEY_DOWN && m_cameraSpeed.y < 0) ) {
            m_cameraSpeed.y = 0;
            return true;
        }

        return false;

    }

};

struct QuadEntity {

    glm::vec3 m_position;
    glm::vec2 m_speed;

    QuadEntity() : m_position({0.0f, 0.0f, 0.0f,}), m_speed({0.0f, 0.0f}) {}

};

class QuadControlLayer : public engine::Layer {

public:

    QuadEntity& m_quadEntity;

    explicit QuadControlLayer(QuadEntity& quadEntity) : m_quadEntity(quadEntity) {}

    void onEvent(engine::Event &event) override {

        engine::EventDispatcher dispatcher(event);

        dispatcher.dispatch<engine::KeyPressEvent>(GE_BIND_CALLBACK_FN(QuadControlLayer::onKeyPressEvent));
        dispatcher.dispatch<engine::KeyReleaseEvent>(GE_BIND_CALLBACK_FN(QuadControlLayer::onKeyReleaseEvent));

    }

    bool onKeyPressEvent(engine::KeyPressEvent& event) {

        if (event.getKey() == GE_KEY_UP) {
            m_quadEntity.m_speed.y = 10;
            return true;
        } else if (event.getKey() == GE_KEY_DOWN) {
            m_quadEntity.m_speed.y = -10;
            return true;
        }

        if (event.getKey() == GE_KEY_RIGHT) {
            m_quadEntity.m_speed.x = 10;
            return true;
        } else if (event.getKey() == GE_KEY_LEFT) {
            m_quadEntity.m_speed.x = -10;
            return true;
        }

        return false;

    }

    bool onKeyReleaseEvent(engine::KeyReleaseEvent& event) {

        if ((event.getKey() == GE_KEY_RIGHT && m_quadEntity.m_speed.x > 0) || (event.getKey() == GE_KEY_LEFT && m_quadEntity.m_speed.x < 0) ) {
            m_quadEntity.m_speed.x = 0;
            return true;
        }

        if ((event.getKey() == GE_KEY_UP && m_quadEntity.m_speed.y > 0) || (event.getKey() == GE_KEY_DOWN && m_quadEntity.m_speed.y < 0) ) {
            m_quadEntity.m_speed.y = 0;
            return true;
        }

        return false;

    }

};

class SceneLayer : public engine::Layer {

private:

    std::shared_ptr<engine::OrthographicCamera> m_camera;
    std::shared_ptr<engine::Shader> m_shader;

    engine::SquareMesh m_quadMesh;

    QuadEntity m_quadEntity;

public:

    SceneLayer(float viewportWidth, float viewportHeight) {

        m_camera = std::make_shared<engine::OrthographicCamera>(viewportWidth, viewportHeight, 100);

        m_shader = std::make_shared<engine::Shader>();
        m_shader->attach(GL_VERTEX_SHADER, "res/shaders/vertex-position.glsl");
        m_shader->attach(GL_FRAGMENT_SHADER, "res/shaders/fragment-color.glsl");
        m_shader->compile();

        m_quadEntity = QuadEntity();

    }

    void onUpdate(engine::TimeStep timeStep) override {

        // Update the position of the quad based on its speed
        m_quadEntity.m_position.x += m_quadEntity.m_speed.x * timeStep;
        m_quadEntity.m_position.y += m_quadEntity.m_speed.y * timeStep;

        // Prepare the transform matrix of the quad
        glm::mat4 quadTransform =
            glm::translate(glm::mat4(1.0f), glm::vec3(m_quadEntity.m_position)) *
            glm::scale(glm::mat4(1.0f), {1.0f, 1.0f, 1.0f})
        ;

        glm::mat4 quad2Transform =
            glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -2.0f, 0.0f)) *
            glm::scale(glm::mat4(1.0f), {1.0f, 1.0f, 1.0f})
        ;

        engine::RenderCommand::clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        engine::Renderer::beginScene(m_camera);
        engine::Renderer::beginBatch(m_shader, engine::MeshElementType::Face);

        engine::Renderer::submit(m_quadMesh, quadTransform);
        engine::Renderer::submit(m_quadMesh, quad2Transform);

        engine::Renderer::flushBatch();
        engine::Renderer::endScene();

    }

    inline QuadEntity& getQuadEntity() {return m_quadEntity; }
    inline std::shared_ptr<engine::OrthographicCamera>& getCamera() {return m_camera; }

};

class PlayableQuadApplication : public engine::Application {

public:
    explicit PlayableQuadApplication(const std::string& name) : engine::Application(name) {}

private:

    SceneLayer* m_sceneLayer;
    QuadControlLayer* m_quadControlLayer;
    CameraControlLayer* m_cameraControlLayer;

    int m_activeControl ;

public:

    void onReady() override {
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(), m_window->getViewportHeight());
        m_quadControlLayer = new QuadControlLayer(m_sceneLayer->getQuadEntity());
        m_cameraControlLayer = new CameraControlLayer(m_sceneLayer->getCamera());
        pushLayer(m_sceneLayer);
    }

    void onUpdate(engine::TimeStep timeStep) override {

        engine::Application::onUpdate(timeStep);

    }

    void onGuiRender() override {

        ImGui::Begin("Demo");
        if (ImGui::RadioButton("Control Quad", &m_activeControl, 0)) {
            popLayer(m_quadControlLayer);
            popLayer(m_cameraControlLayer);

            pushLayer(m_quadControlLayer);
        }

        if (ImGui::RadioButton("Control Camera", &m_activeControl, 1)) {
            popLayer(m_quadControlLayer);
            popLayer(m_cameraControlLayer);

            pushLayer(m_cameraControlLayer);
        }
        ImGui::End();

    }

};

int main() {

    PlayableQuadApplication app("Layered Controls");
    engine::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}