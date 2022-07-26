#include <Scene.h>

#include <glm/glm.hpp>

class BallController : public engine::NativeScript {

    void onCreate() override {

        float targetImpulse = 5.0f;

        auto &collider = getComponent<engine::CircleColliderComponent>();
        auto body = collider.getBody();

        body->SetLinearVelocity({targetImpulse, targetImpulse});

    }

};

class PlayerPaddleController : public engine::NativeScript {

public:

    float m_maxHeight = 0;
    float m_minHeight = 0;

    void onUpdate(engine::TimeStep timeStep) override {

        float targetImpulse = 5.0f;

        auto& collider = getComponent<engine::BoxColliderComponent>();
        auto& transform = getComponent<engine::TransformComponent>();
        auto body = collider.getBody();

        float top = body->GetWorldCenter().y + (transform.m_scale.y/2);
        float bottom = body->GetWorldCenter().y - (transform.m_scale.y/2);

        if (engine::Input::isKeyPressed(GE_KEY_DOWN) && bottom > m_minHeight) {
            body->SetLinearVelocity({0.0f, -targetImpulse});
            return;
        }

        if (engine::Input::isKeyPressed(GE_KEY_UP) && top < m_maxHeight) {
            body->SetLinearVelocity({0.0f, targetImpulse});
            return;
        }

        body->SetLinearVelocity({0.0f, 0.0f});

    }

};

class SceneLayer : public engine::Layer {

private:
    engine::Scene m_scene;

public:

    SceneLayer(float viewportWidth, float viewportHeight) {

        auto camera = std::make_shared<engine::OrthographicCamera>(viewportWidth, viewportHeight, 100);
        glm::vec2 projectionSize = camera->getProjectionSize();

        float leftWallX = -projectionSize.x/2;
        float rightWallX = projectionSize.x/2;
        float groundY = -projectionSize.y/2;
        float ceilingY = projectionSize.y/2;
        float playerPaddleX = (-projectionSize.x/2) + 1.0f;

        createRoom(
            leftWallX, rightWallX,
            groundY, ceilingY,
            projectionSize.x, projectionSize.y
        );
        createBall();
        auto playerPaddle = createPlayerPaddle(
            groundY, ceilingY,
            playerPaddleX
        );

        m_scene.enablePhysics({0.0f, 0.0f});
        m_scene.start(camera);

        auto& playerPaddleScriptComponent = playerPaddle.getComponent<engine::NativeScriptComponent>();
        auto* playerPaddleController = dynamic_cast<PlayerPaddleController*>(playerPaddleScriptComponent.getScriptInstance());
        playerPaddleController->m_maxHeight = ceilingY;
        playerPaddleController->m_minHeight = groundY;

    }

    void createRoom(float leftWallX, float rightWallX, float groundY, float ceilingY, float width, float height) {

        auto leftWall = m_scene.createEntity();
        leftWall.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Static);
        leftWall.addComponent<engine::BoxColliderComponent>();
        leftWall.addComponent<engine::TransformComponent>(
            glm::vec3(leftWallX - 0.5f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, height, 1.0f)
        );

        auto rightWall = m_scene.createEntity();
        rightWall.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Static);
        rightWall.addComponent<engine::BoxColliderComponent>();
        rightWall.addComponent<engine::TransformComponent>(
            glm::vec3(rightWallX + 0.5f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, height, 1.0f)
        );

        auto ground = m_scene.createEntity();
        ground.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Static);
        ground.addComponent<engine::BoxColliderComponent>();
        ground.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, groundY - 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(width, 1.0f, 1.0f)
        );

        auto ceiling = m_scene.createEntity();
        ceiling.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Static);
        ceiling.addComponent<engine::BoxColliderComponent>();
        ceiling.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, ceilingY + 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(width, 1.0f, 1.0f)
        );

    }

    void createBall() {

        engine::Entity ball = m_scene.createEntity();
        ball.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        ball.addComponent<engine::CircleComponent>(1.0f, 0.01);
        ball.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Dynamic, true);
        ball.addComponent<engine::CircleColliderComponent>(0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
        ball.addComponent<engine::NativeScriptComponent>().bind<BallController>();

    }

    engine::Entity createPlayerPaddle(float groundY, float ceilingY, float paddleX) {

        engine::Entity playerPaddle = m_scene.createEntity();
        playerPaddle.addComponent<engine::TransformComponent>(
            glm::vec3(paddleX, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.4f, 1.5f, 1.0f)
        );
        playerPaddle.addComponent<engine::PolygonComponent>(engine::SquareMesh());
        playerPaddle.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Kinematic, true);
        playerPaddle.addComponent<engine::BoxColliderComponent>(glm::vec2(0.0f, 0.0f), 1.0f, 0.0f, 0.0f, 1.0f);
        playerPaddle.addComponent<engine::NativeScriptComponent>().bind<PlayerPaddleController>();

        return playerPaddle;

    }

    ~SceneLayer() {
        m_scene.stop();
    }

    void onUpdate(engine::TimeStep timeStep) override {
        m_scene.onUpdate(timeStep);
    }

};

class PongApplication : public engine::Application {

public:
    explicit PongApplication(const std::string& name) : engine::Application(name) {}

    void onReady() override {
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(),m_window->getViewportHeight());
        pushLayer(m_sceneLayer);
    }

private:
    SceneLayer* m_sceneLayer = nullptr;

};

int main() {

    PongApplication app("Pong");
    engine::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}