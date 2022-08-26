#include "../include/Scene.h"

class QuadController : public gaunlet::Scene::NativeScript {

public:

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {

        float targetVelocity = 3.0f;

        auto& collider = getComponent<gaunlet::Scene::BoxColliderComponent>();
        auto& transform = getComponent<gaunlet::Scene::TransformComponent>();
        auto body = collider.getBody();

        auto currentVelocity = body->GetLinearVelocity();

        float velocityX = 0.0f;

        if (gaunlet::Core::Input::isKeyPressed(GE_KEY_A) && !gaunlet::Core::Input::isKeyPressed(GE_KEY_D)) {
            velocityX = -targetVelocity;
        } else if (!gaunlet::Core::Input::isKeyPressed(GE_KEY_A) && gaunlet::Core::Input::isKeyPressed(GE_KEY_D)) {
            velocityX = targetVelocity;
        }

        float velocityY = 0.0f;

        if (gaunlet::Core::Input::isKeyPressed(GE_KEY_S) && !gaunlet::Core::Input::isKeyPressed(GE_KEY_W)) {
            velocityY = -targetVelocity;
        } else if (!gaunlet::Core::Input::isKeyPressed(GE_KEY_S) && gaunlet::Core::Input::isKeyPressed(GE_KEY_W)) {
            velocityY = targetVelocity;
        }

        body->SetLinearVelocity({velocityX, velocityY});

    }

};

class BallController : public gaunlet::Scene::NativeScript {

public:

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {

        float targetImpulse = 8.0f;

        auto& collider = getComponent<gaunlet::Scene::CircleColliderComponent>();
        auto& transform = getComponent<gaunlet::Scene::TransformComponent>();
        auto body = collider.getBody();

        if (gaunlet::Core::Input::isKeyPressed(GE_KEY_DOWN)) {
            body->ApplyForceToCenter({0.0f, -targetImpulse}, true);
        }

        if (gaunlet::Core::Input::isKeyPressed(GE_KEY_UP)) {
            body->ApplyForceToCenter({0.0f, targetImpulse}, true);
        }

        if (gaunlet::Core::Input::isKeyPressed(GE_KEY_LEFT)) {
            body->ApplyForceToCenter({-targetImpulse, 0.0f}, true);
        }

        if (gaunlet::Core::Input::isKeyPressed(GE_KEY_RIGHT)) {
            body->ApplyForceToCenter({targetImpulse, 0.0f}, true);
        }

    }

};

class SceneLayer : public gaunlet::Core::Layer {

private:

    gaunlet::Scene::Scene m_scene;
    gaunlet::Core::Ref<gaunlet::Scene::OrthographicCamera> m_camera;

public:

    SceneLayer(float viewportWidth, float viewportHeight) {

        m_camera = gaunlet::Core::CreateRef<gaunlet::Scene::OrthographicCamera>(viewportWidth, viewportHeight, 100);

        createRoom(m_camera->getProjectionSize());
        createBall();
        createQuad();

        m_scene.enablePhysics({0.0f, -9.8f});
        m_scene.start();

    }

    void onEvent(gaunlet::Core::Event &event) override {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::KeyPressEvent>(GE_BIND_CALLBACK_FN(SceneLayer::onKeyPressEvent));

    }

    bool onKeyPressEvent(gaunlet::Core::KeyPressEvent& event) {

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
        leftWall.addComponent<gaunlet::Scene::RigidBodyComponent>(gaunlet::Scene::RigidBodyComponent::Type::Static);
        leftWall.addComponent<gaunlet::Scene::BoxColliderComponent>();
        leftWall.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(leftWallX - 0.5f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, height, 1.0f)
        );

        auto rightWall = m_scene.createEntity();
        rightWall.addComponent<gaunlet::Scene::RigidBodyComponent>(gaunlet::Scene::RigidBodyComponent::Type::Static);
        rightWall.addComponent<gaunlet::Scene::BoxColliderComponent>();
        rightWall.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(rightWallX + 0.5f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, height, 1.0f)
        );

        auto ground = m_scene.createEntity();
        ground.addComponent<gaunlet::Scene::RigidBodyComponent>(gaunlet::Scene::RigidBodyComponent::Type::Static);
        ground.addComponent<gaunlet::Scene::BoxColliderComponent>(glm::vec2(0.0f, 0.0f), 1.0f, 0.05f, 0.0f, 1.0f);
        ground.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, groundY - 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(width, 1.0f, 1.0f)
        );

        auto ceiling = m_scene.createEntity();
        ceiling.addComponent<gaunlet::Scene::RigidBodyComponent>(gaunlet::Scene::RigidBodyComponent::Type::Static);
        ceiling.addComponent<gaunlet::Scene::BoxColliderComponent>();
        ceiling.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, ceilingY + 0.5f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(width, 1.0f, 1.0f)
        );

    }

    void createQuad() {

        gaunlet::Scene::Entity quad = m_scene.createEntity();
        quad.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 1.0f)
        );
        quad.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Square2DModel());
        quad.addComponent<gaunlet::Scene::RigidBodyComponent>(gaunlet::Scene::RigidBodyComponent::Type::Kinematic, true);
        quad.addComponent<gaunlet::Scene::BoxColliderComponent>(glm::vec2(0.0f, 0.0f), 1.0f, 0.0f, 0.0f, 1.0f);
        quad.addComponent<gaunlet::Scene::NativeScriptComponent>().bind<QuadController>();

    }

    void createBall() {

        gaunlet::Scene::Entity ball = m_scene.createEntity();
        ball.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        ball.addComponent<gaunlet::Scene::CircleComponent>(1.0f, 0.01);
        ball.addComponent<gaunlet::Scene::RigidBodyComponent>(gaunlet::Scene::RigidBodyComponent::Type::Dynamic, true);
        ball.addComponent<gaunlet::Scene::CircleColliderComponent>(0.0f, 1.0f, 0.05f, 0.8f, 1.0f);
        ball.addComponent<gaunlet::Scene::NativeScriptComponent>().bind<BallController>();

    }

    ~SceneLayer() {
        m_scene.stop();
    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {
        m_scene.update(timeStep);
        m_scene.render(gaunlet::Scene::RenderMode::Faces, m_camera);
    }

};

class Physics2DApplication : public gaunlet::Core::Application {

public:
    explicit Physics2DApplication(const std::string& name) : gaunlet::Core::Application(name) {}

    void onReady() override {
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(),m_window->getViewportHeight());
        pushLayer(m_sceneLayer);
    }

private:
    SceneLayer* m_sceneLayer = nullptr;

};

int main() {

    Physics2DApplication app("Physics 2D");
    gaunlet::Core::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}