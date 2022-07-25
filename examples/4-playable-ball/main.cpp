#include <Scene.h>

#include <glm/glm.hpp>

class BallController : public engine::NativeScript {

    void onUpdate(engine::TimeStep timeStep) override {

        if (hasComponent<engine::CircleColliderComponent>()) {

            float targetImpulse = 0.5f;

            auto& collider = getComponent<engine::CircleColliderComponent>();
            auto body = collider.getBody();

            if (engine::Input::isKeyPressed(GE_KEY_LEFT)) {
                body->ApplyLinearImpulseToCenter({-targetImpulse, 0.0f}, true);
            }

            if (engine::Input::isKeyPressed(GE_KEY_RIGHT)) {
                body->ApplyLinearImpulseToCenter({targetImpulse, 0.0f}, true);
            }

            if (engine::Input::isKeyPressed(GE_KEY_DOWN)) {
                body->ApplyLinearImpulseToCenter({0.0f, -targetImpulse}, true);
            }

            if (engine::Input::isKeyPressed(GE_KEY_UP)) {
                body->ApplyLinearImpulseToCenter({0.0f, targetImpulse}, true);
            }

        }

    }

};

class SceneLayer : public engine::Layer {

private:

    std::shared_ptr<engine::OrthographicCamera> m_camera;
    engine::Scene m_scene;

public:

    SceneLayer(float viewportWidth, float viewportHeight) {

        engine::Renderer::init();

        m_camera = std::make_shared<engine::OrthographicCamera>(viewportWidth, viewportHeight, 100);

        createRoom();

        engine::Entity quad = m_scene.createEntity();
        quad.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        quad.addComponent<engine::CircleComponent>(1.0f, 0.01);
        quad.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Dynamic, true);
        quad.addComponent<engine::CircleColliderComponent>(0.0f, 1.0f, 0.1f, 0.8f, 0.9f);
        quad.addComponent<engine::NativeScriptComponent>().bind<BallController>();

        m_scene.start({0.0f, -9.8f});

    }

    void createRoom() {

        glm::vec2 projectionSize = m_camera->getProjectionSize();
        float leftWallX = -projectionSize.x/2;
        float rightWallX = projectionSize.x/2;
        float groundY = -projectionSize.y/2;
        float ceilingY = projectionSize.y/2;

        auto leftWall = m_scene.createEntity();
        leftWall.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Static);
        leftWall.addComponent<engine::BoxColliderComponent>();
        leftWall.addComponent<engine::TransformComponent>(
            glm::vec3(leftWallX - 0.5f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, projectionSize.y, 1.0f)
        );

        auto rightWall = m_scene.createEntity();
        rightWall.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Static);
        rightWall.addComponent<engine::BoxColliderComponent>();
        rightWall.addComponent<engine::TransformComponent>(
            glm::vec3(rightWallX + 0.5f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, projectionSize.y, 1.0f)
        );

        auto ground = m_scene.createEntity();
        ground.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Static);
        ground.addComponent<engine::BoxColliderComponent>(glm::vec2(0.0f, 0.0f), 1.0f, 0.05f, 0.0f, 1.0f);
        ground.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, groundY - 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(projectionSize.x, 1.0f, 1.0f)
        );

        auto ceiling = m_scene.createEntity();
        ceiling.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Static);
        ceiling.addComponent<engine::BoxColliderComponent>();
        ceiling.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, ceilingY + 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(projectionSize.x, 1.0f, 1.0f)
        );

    }

    ~SceneLayer() {
        m_scene.stop();
    }

    void onUpdate(engine::TimeStep timeStep) override {

        engine::Renderer::beginScene(m_camera);
        m_scene.onUpdate(timeStep);
        engine::Renderer::endScene();

    }

};

class PlayableBallApplication : public engine::Application {

public:
    explicit PlayableBallApplication(const std::string& name) : engine::Application(name) {}

private:

    SceneLayer* m_sceneLayer = nullptr;

public:

    void onReady() override {

        m_sceneLayer = new SceneLayer(
            m_window->getViewportWidth(),
            m_window->getViewportHeight()
        );
        pushLayer(m_sceneLayer);

    }

};

int main() {

    PlayableBallApplication app("Playable Ball");
    engine::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}