#include <Scene.h>

#include <glm/glm.hpp>

class NativeScriptExample : public engine::NativeScript {

    void onUpdate(engine::TimeStep timeStep) {

        if (hasComponent<engine::CircleColliderComponent>()) {

            float targetSpeed = 3.0f;

            auto& collider = getComponent<engine::CircleColliderComponent>();
            auto body = collider.getBody();
            auto currentSpeed = body->GetLinearVelocity();

            if (engine::Input::isKeyPressed(GE_KEY_LEFT)) {
                body->SetLinearVelocity({-targetSpeed, currentSpeed.y});
            }

            if (engine::Input::isKeyPressed(GE_KEY_RIGHT)) {
                body->SetLinearVelocity({targetSpeed, currentSpeed.y});
            }

            if (engine::Input::isKeyPressed(GE_KEY_DOWN)) {
                body->SetLinearVelocity({currentSpeed.x, -targetSpeed});
            }

            if (engine::Input::isKeyPressed(GE_KEY_UP)) {
                body->SetLinearVelocity({currentSpeed.x, targetSpeed});
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
        quad.addComponent<engine::CircleComponent>(1.0f);
        quad.addComponent<engine::MaterialComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        quad.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Dynamic, false);
        quad.addComponent<engine::CircleColliderComponent>(-0.01f, 1.0f, 0.0f, 1.0f, 0.05f);
        quad.addComponent<engine::NativeScriptComponent>().bind<NativeScriptExample>();

        m_scene.start({0.0f, 0.0f});

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
        ground.addComponent<engine::BoxColliderComponent>();
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

class PlayableQuadApplication : public engine::Application {

public:
    explicit PlayableQuadApplication(const std::string& name) : engine::Application(name) {}

private:

    SceneLayer* m_sceneLayer;

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

    PlayableQuadApplication app("Layered Controls");
    engine::RunLoop runLoop(app);
    runLoop.run();

    return 0;

}