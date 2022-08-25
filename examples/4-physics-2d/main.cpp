#include "../include/Scene.h"

class QuadController : public engine::Scene::NativeScript {

public:

    void onUpdate(engine::Core::TimeStep timeStep) override {

        float targetVelocity = 3.0f;

        auto& collider = getComponent<engine::Scene::BoxColliderComponent>();
        auto& transform = getComponent<engine::Scene::TransformComponent>();
        auto body = collider.getBody();

        auto currentVelocity = body->GetLinearVelocity();

        float velocityX = 0.0f;

        if (engine::Core::Input::isKeyPressed(GE_KEY_A) && !engine::Core::Input::isKeyPressed(GE_KEY_D)) {
            velocityX = -targetVelocity;
        } else if (!engine::Core::Input::isKeyPressed(GE_KEY_A) && engine::Core::Input::isKeyPressed(GE_KEY_D)) {
            velocityX = targetVelocity;
        }

        float velocityY = 0.0f;

        if (engine::Core::Input::isKeyPressed(GE_KEY_S) && !engine::Core::Input::isKeyPressed(GE_KEY_W)) {
            velocityY = -targetVelocity;
        } else if (!engine::Core::Input::isKeyPressed(GE_KEY_S) && engine::Core::Input::isKeyPressed(GE_KEY_W)) {
            velocityY = targetVelocity;
        }

        body->SetLinearVelocity({velocityX, velocityY});

    }

};

class BallController : public engine::Scene::NativeScript {

public:

    void onUpdate(engine::Core::TimeStep timeStep) override {

        float targetImpulse = 8.0f;

        auto& collider = getComponent<engine::Scene::CircleColliderComponent>();
        auto& transform = getComponent<engine::Scene::TransformComponent>();
        auto body = collider.getBody();

        if (engine::Core::Input::isKeyPressed(GE_KEY_DOWN)) {
            body->ApplyForceToCenter({0.0f, -targetImpulse}, true);
        }

        if (engine::Core::Input::isKeyPressed(GE_KEY_UP)) {
            body->ApplyForceToCenter({0.0f, targetImpulse}, true);
        }

        if (engine::Core::Input::isKeyPressed(GE_KEY_LEFT)) {
            body->ApplyForceToCenter({-targetImpulse, 0.0f}, true);
        }

        if (engine::Core::Input::isKeyPressed(GE_KEY_RIGHT)) {
            body->ApplyForceToCenter({targetImpulse, 0.0f}, true);
        }

    }

};

class SceneLayer : public engine::Core::Layer {

private:

    engine::Scene::Scene m_scene;
    engine::Core::Ref<engine::Scene::OrthographicCamera> m_camera;

public:

    SceneLayer(float viewportWidth, float viewportHeight) {

        m_camera = engine::Core::CreateRef<engine::Scene::OrthographicCamera>(viewportWidth, viewportHeight, 100);

        createRoom(m_camera->getProjectionSize());
        createBall();
        createQuad();

        m_scene.enablePhysics({0.0f, -9.8f});
        m_scene.start();

    }

    void onEvent(engine::Core::Event &event) override {

        engine::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<engine::Core::KeyPressEvent>(GE_BIND_CALLBACK_FN(SceneLayer::onKeyPressEvent));

    }

    bool onKeyPressEvent(engine::Core::KeyPressEvent& event) {

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
        leftWall.addComponent<engine::Scene::RigidBodyComponent>(engine::Scene::RigidBodyComponent::Type::Static);
        leftWall.addComponent<engine::Scene::BoxColliderComponent>();
        leftWall.addComponent<engine::Scene::TransformComponent>(
            glm::vec3(leftWallX - 0.5f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, height, 1.0f)
        );

        auto rightWall = m_scene.createEntity();
        rightWall.addComponent<engine::Scene::RigidBodyComponent>(engine::Scene::RigidBodyComponent::Type::Static);
        rightWall.addComponent<engine::Scene::BoxColliderComponent>();
        rightWall.addComponent<engine::Scene::TransformComponent>(
            glm::vec3(rightWallX + 0.5f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, height, 1.0f)
        );

        auto ground = m_scene.createEntity();
        ground.addComponent<engine::Scene::RigidBodyComponent>(engine::Scene::RigidBodyComponent::Type::Static);
        ground.addComponent<engine::Scene::BoxColliderComponent>(glm::vec2(0.0f, 0.0f), 1.0f, 0.05f, 0.0f, 1.0f);
        ground.addComponent<engine::Scene::TransformComponent>(
            glm::vec3(0.0f, groundY - 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(width, 1.0f, 1.0f)
        );

        auto ceiling = m_scene.createEntity();
        ceiling.addComponent<engine::Scene::RigidBodyComponent>(engine::Scene::RigidBodyComponent::Type::Static);
        ceiling.addComponent<engine::Scene::BoxColliderComponent>();
        ceiling.addComponent<engine::Scene::TransformComponent>(
            glm::vec3(0.0f, ceilingY + 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(width, 1.0f, 1.0f)
        );

    }

    void createQuad() {

        engine::Scene::Entity quad = m_scene.createEntity();
        quad.addComponent<engine::Scene::TransformComponent>(
            glm::vec3(-2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 1.0f)
        );
        quad.addComponent<engine::Scene::ModelComponent>(engine::Scene::Square2DModel());
        quad.addComponent<engine::Scene::RigidBodyComponent>(engine::Scene::RigidBodyComponent::Type::Kinematic, true);
        quad.addComponent<engine::Scene::BoxColliderComponent>(glm::vec2(0.0f, 0.0f), 1.0f, 0.0f, 0.0f, 1.0f);
        quad.addComponent<engine::Scene::NativeScriptComponent>().bind<QuadController>();

    }

    void createBall() {

        engine::Scene::Entity ball = m_scene.createEntity();
        ball.addComponent<engine::Scene::TransformComponent>(
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        ball.addComponent<engine::Scene::CircleComponent>(1.0f, 0.01);
        ball.addComponent<engine::Scene::RigidBodyComponent>(engine::Scene::RigidBodyComponent::Type::Dynamic, true);
        ball.addComponent<engine::Scene::CircleColliderComponent>(0.0f, 1.0f, 0.05f, 0.8f, 1.0f);
        ball.addComponent<engine::Scene::NativeScriptComponent>().bind<BallController>();

    }

    ~SceneLayer() {
        m_scene.stop();
    }

    void onUpdate(engine::Core::TimeStep timeStep) override {
        m_scene.update(timeStep);
        m_scene.render(m_camera);
    }

};

class Physics2DApplication : public engine::Core::Application {

public:
    explicit Physics2DApplication(const std::string& name) : engine::Core::Application(name) {}

    void onReady() override {
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(),m_window->getViewportHeight());
        pushLayer(m_sceneLayer);
    }

private:
    SceneLayer* m_sceneLayer = nullptr;

};

int main() {

    Physics2DApplication app("Physics 2D");
    engine::Core::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}