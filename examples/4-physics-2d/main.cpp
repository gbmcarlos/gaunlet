#include <Scene.h>

#include <glm/glm.hpp>
#include <iostream>

class QuadController : public engine::NativeScript {

public:

    void onUpdate(engine::TimeStep timeStep) override {

        float targetVelocity = 3.0f;

        auto& collider = getComponent<engine::BoxColliderComponent>();
        auto& transform = getComponent<engine::TransformComponent>();
        auto body = collider.getBody();

        auto currentVelocity = body->GetLinearVelocity();

        float velocityX = 0.0f;

        if (engine::Input::isKeyPressed(GE_KEY_A) && !engine::Input::isKeyPressed(GE_KEY_D)) {
            velocityX = -targetVelocity;
        } else if (!engine::Input::isKeyPressed(GE_KEY_A) && engine::Input::isKeyPressed(GE_KEY_D)) {
            velocityX = targetVelocity;
        }

        float velocityY = 0.0f;

        if (engine::Input::isKeyPressed(GE_KEY_S) && !engine::Input::isKeyPressed(GE_KEY_W)) {
            velocityY = -targetVelocity;
        } else if (!engine::Input::isKeyPressed(GE_KEY_S) && engine::Input::isKeyPressed(GE_KEY_W)) {
            velocityY = targetVelocity;
        }

        body->SetLinearVelocity({velocityX, velocityY});

    }

};

class BallController : public engine::NativeScript {

public:

    void onUpdate(engine::TimeStep timeStep) override {

        float targetImpulse = 8.0f;

        auto& collider = getComponent<engine::CircleColliderComponent>();
        auto& transform = getComponent<engine::TransformComponent>();
        auto body = collider.getBody();

        if (engine::Input::isKeyPressed(GE_KEY_DOWN)) {
            body->ApplyForceToCenter({0.0f, -targetImpulse}, true);
        }

        if (engine::Input::isKeyPressed(GE_KEY_UP)) {
            body->ApplyForceToCenter({0.0f, targetImpulse}, true);
        }

        if (engine::Input::isKeyPressed(GE_KEY_LEFT)) {
            body->ApplyForceToCenter({-targetImpulse, 0.0f}, true);
        }

        if (engine::Input::isKeyPressed(GE_KEY_RIGHT)) {
            body->ApplyForceToCenter({targetImpulse, 0.0f}, true);
        }

    }

};

class SceneLayer : public engine::Layer {

private:

    engine::Scene m_scene;
    std::shared_ptr<engine::OrthographicCamera> m_camera;

public:

    SceneLayer(float viewportWidth, float viewportHeight) {

        m_camera = std::make_shared<engine::OrthographicCamera>(viewportWidth, viewportHeight, 100);

        createRoom(m_camera->getProjectionSize());
        createBall();
        createQuad();

        m_scene.enablePhysics({0.0f, -9.8f});
        m_scene.start();

    }

    void onEvent(engine::Event &event) override {

        engine::EventDispatcher dispatcher(event);
        dispatcher.dispatch<engine::KeyPressEvent>(GE_BIND_CALLBACK_FN(SceneLayer::onKeyPressEvent));

    }

    bool onKeyPressEvent(engine::KeyPressEvent& event) {

        if (event.getKey() == GE_KEY_SPACE) {
            m_scene.togglePlay();
            return true;
        }

        return false;

    }

    void createRoom(glm::vec2 projectionSize) {

        float leftWallX = -projectionSize.x/2;
        float rightWallX = projectionSize.x/2;
        float groundY = -projectionSize.y/2;
        float ceilingY = projectionSize.y/2;
        float height = projectionSize.y;
        float width = projectionSize.x;

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
        ground.addComponent<engine::BoxColliderComponent>(glm::vec2(0.0f, 0.0f), 1.0f, 0.05f, 0.0f, 1.0f);
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

    void createQuad() {

        engine::Entity quad = m_scene.createEntity();
        quad.addComponent<engine::TransformComponent>(
            glm::vec3(-2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 1.0f)
        );
        quad.addComponent<engine::PolygonModelComponent>(engine::Square2DModel());
        quad.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Kinematic, true);
        quad.addComponent<engine::BoxColliderComponent>(glm::vec2(0.0f, 0.0f), 1.0f, 0.0f, 0.0f, 1.0f);
        quad.addComponent<engine::NativeScriptComponent>().bind<QuadController>();

    }

    void createBall() {

        engine::Entity ball = m_scene.createEntity();
        ball.addComponent<engine::TransformComponent>(
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        ball.addComponent<engine::CircleModelComponent>(1.0f, 0.01);
        ball.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Dynamic, true);
        ball.addComponent<engine::CircleColliderComponent>(0.0f, 1.0f, 0.05f, 0.8f, 1.0f);
        ball.addComponent<engine::NativeScriptComponent>().bind<BallController>();

    }

    ~SceneLayer() {
        m_scene.stop();
    }

    void onUpdate(engine::TimeStep timeStep) override {
        m_scene.update(timeStep);
        m_scene.render(m_camera);
    }

};

class Physics2DApplication : public engine::Application {

public:
    explicit Physics2DApplication(const std::string& name) : engine::Application(name) {}

    void onReady() override {
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(),m_window->getViewportHeight());
        pushLayer(m_sceneLayer);
    }

private:
    SceneLayer* m_sceneLayer = nullptr;

};

int main() {

    Physics2DApplication app("Physics 2D");
    engine::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}