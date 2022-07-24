#include <Scene.h>

#include <glm/glm.hpp>
#include <box2d/box2d.h>

class NativeScriptExample : public engine::NativeScript {

    void onUpdate(engine::TimeStep timeStep) {

        if (hasComponent<engine::BoxColliderComponent>()) {

            auto& boxCollider = getComponent<engine::BoxColliderComponent>();

            if (engine::Input::isKeyPressed(GE_KEY_UP) && boxCollider.getBody()->GetLinearVelocity().y < 0.01f && boxCollider.getBody()->GetLinearVelocity().y > -0.01f) {

                boxCollider.getBody()->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 5.0f), true);

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

        engine:: Entity quad = m_scene.createEntity();
        quad.addComponent<engine::PolygonComponent>(engine::SquareMesh());
        quad.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, 4.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        quad.addComponent<engine::MaterialComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        quad.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Dynamic);
        quad.addComponent<engine::BoxColliderComponent>();
        quad.addComponent<engine::NativeScriptComponent>().bind<NativeScriptExample>();

        auto ground = m_scene.createEntity();
        ground.addComponent<engine::PolygonComponent>(engine::SquareMesh());
        ground.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, -3.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(5.0f, 1.0f, 1.0f)
        );
        ground.addComponent<engine::RigidBodyComponent>(engine::RigidBodyComponent::Type::Static);
        ground.addComponent<engine::BoxColliderComponent>();

        m_scene.start();

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