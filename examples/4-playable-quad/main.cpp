#include <Core.h>
#include <Graphics.h>
#include <Scene.h>

#include <array>
#include <glm/glm.hpp>

class QuadControlLayer : public engine::Layer {

public:

    engine::Entity& m_quadEntity;

    explicit QuadControlLayer(engine::Entity& quadEntity) : m_quadEntity(quadEntity) {}

    void onEvent(engine::Event &event) override {

        engine::EventDispatcher dispatcher(event);

        dispatcher.dispatch<engine::KeyPressEvent>(GE_BIND_CALLBACK_FN(QuadControlLayer::onKeyPressEvent));
        dispatcher.dispatch<engine::KeyReleaseEvent>(GE_BIND_CALLBACK_FN(QuadControlLayer::onKeyReleaseEvent));

    }

    bool onKeyPressEvent(engine::KeyPressEvent& event) {

        auto& speed = m_quadEntity.getComponent<engine::SpeedComponent>();

        if (event.getKey() == GE_KEY_UP) {
            speed.m_speed.y = 10;
            return true;
        } else if (event.getKey() == GE_KEY_DOWN) {
            speed.m_speed.y = -10;
            return true;
        }

        if (event.getKey() == GE_KEY_RIGHT) {
            speed.m_speed.x = 10;
            return true;
        } else if (event.getKey() == GE_KEY_LEFT) {
            speed.m_speed.x = -10;
            return true;
        }

        return false;

    }

    bool onKeyReleaseEvent(engine::KeyReleaseEvent& event) {

        auto& speed = m_quadEntity.getComponent<engine::SpeedComponent>();

        if ((event.getKey() == GE_KEY_RIGHT && speed.m_speed.x > 0) || (event.getKey() == GE_KEY_LEFT && speed.m_speed.x < 0) ) {
            speed.m_speed.x = 0;
            return true;
        }

        if ((event.getKey() == GE_KEY_UP && speed.m_speed.y > 0) || (event.getKey() == GE_KEY_DOWN && speed.m_speed.y < 0) ) {
            speed.m_speed.y = 0;
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

    engine::Scene m_scene;
    engine::Entity m_quadEntity;

public:

    SceneLayer(float viewportWidth, float viewportHeight) : m_quadEntity(m_scene.createEntity()) {

        engine::Renderer::init();
        m_camera = std::make_shared<engine::OrthographicCamera>(viewportWidth, viewportHeight, 100);
        m_quadEntity.addComponent<engine::MeshComponent>(m_quadMesh);
        m_quadEntity.addComponent<engine::TransformComponent>();
//        m_quadEntity.addComponent<engine::MaterialComponent>(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
        m_quadEntity.addComponent<engine::SpeedComponent>();

    }

    void onUpdate(engine::TimeStep timeStep) override {

        // Update the position of the quad based on its speed
        auto& transform = m_quadEntity.getComponent<engine::TransformComponent>();
        auto& speed = m_quadEntity.getComponent<engine::SpeedComponent>();
        transform.m_position.x += speed.m_speed.x * timeStep;
        transform.m_position.y += speed.m_speed.y * timeStep;

        engine::Renderer::beginScene(m_camera);
        m_scene.onUpdate(timeStep);
        engine::Renderer::endScene();

    }

    inline engine::Entity& getQuadEntity() {return m_quadEntity; }

};

class PlayableQuadApplication : public engine::Application {

public:
    explicit PlayableQuadApplication(const std::string& name) : engine::Application(name) {}

private:

    SceneLayer* m_sceneLayer;
    QuadControlLayer* m_quadControlLayer;

public:

    void onReady() override {
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(), m_window->getViewportHeight());
        m_quadControlLayer = new QuadControlLayer(m_sceneLayer->getQuadEntity());
        pushLayer(m_sceneLayer);
        pushLayer(m_quadControlLayer);
    }

};

int main() {

    PlayableQuadApplication app("Layered Controls");
    engine::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}