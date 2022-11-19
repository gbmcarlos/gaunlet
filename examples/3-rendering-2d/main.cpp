#include "../include/Scene.h"
#include "../include/Prefab.h"

class Rendering2DApplication : public gaunlet::Core::Application {

public:

    explicit Rendering2DApplication() : gaunlet::Core::Application() {}

    void onReady() override {

        m_scene = gaunlet::Core::CreateRef<gaunlet::Scene::Scene>();

        float viewportWidth = (float) gaunlet::Core::Window::getCurrentInstance()->getViewportWidth();
        float viewportHeight = (float) gaunlet::Core::Window::getCurrentInstance()->getViewportHeight();

        m_camera = gaunlet::Core::CreateRef<gaunlet::Scene::OrthographicCamera>(viewportWidth, viewportHeight, 100);
        m_camera->setPosition({0, 0, 1});

        m_renderPipeline = gaunlet::Core::CreateRef<gaunlet::Prefab::RenderPipelines::Basic3DRenderPipeline>();

        gaunlet::Core::Ref<gaunlet::Graphics::TextureImage2D> texture1 = gaunlet::Core::CreateRef<gaunlet::Graphics::TextureImage2D>(
            "assets/texture-1.jpeg",
            gaunlet::Core::TextureFilteringParameterValue::Linear,
            gaunlet::Core::TextureWrappingParameterValue::ClampToEdge
        );
        gaunlet::Core::Ref<gaunlet::Graphics::TextureImage2D> texture2 = gaunlet::Core::CreateRef<gaunlet::Graphics::TextureImage2D>(
            "assets/texture-2.jpeg",
            gaunlet::Core::TextureFilteringParameterValue::Linear,
            gaunlet::Core::TextureWrappingParameterValue::ClampToEdge
        );

        // TRIANGLE 1
        auto triangle1 = m_scene->createEntity();
        triangle1.addComponent<gaunlet::Prefab::Components::ModelComponent>(gaunlet::Prefab::Sprites::Triangle2DModel());
        triangle1.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-5.0f, 3.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        triangle1.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));

        // TRIANGLE 2
        auto triangle2 = m_scene->createEntity();
        triangle2.addComponent<gaunlet::Prefab::Components::ModelComponent>(gaunlet::Prefab::Sprites::Triangle2DModel());
        triangle2.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-5.0f, 0.5f, 0.0f),
            glm::vec4(0.0f, 0.0f, 200.0f, 20.0f),
            glm::vec3(1.2f, 1.2f, 1.0f)
        );
        triangle2.addComponent<gaunlet::Scene::MaterialComponent>(texture1);

        // TRIANGLE 3
        auto triangle3 = m_scene->createEntity();
        triangle3.addComponent<gaunlet::Prefab::Components::ModelComponent>(gaunlet::Prefab::Sprites::Triangle2DModel());
        triangle3.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-5.0f, -2.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 40.0f),
            glm::vec3(1.8f, 1.8f, 1.0f)
        );
        triangle3.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f), texture2);

        // SQUARE 1
        auto square1 = m_scene->createEntity();
        square1.addComponent<gaunlet::Prefab::Components::ModelComponent>(gaunlet::Prefab::Sprites::Square2DModel());
        square1.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, 3.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        square1.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.8f, 0.0f, 1.0f));

        // SQUARE 2
        auto square2 = m_scene->createEntity();
        square2.addComponent<gaunlet::Prefab::Components::ModelComponent>(gaunlet::Prefab::Sprites::Square2DModel());
        square2.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 20.0f),
            glm::vec3(1.2f, 1.2f, 1.0f)
        );
        square2.addComponent<gaunlet::Scene::MaterialComponent>(texture1);

        // SQUARE 3
        auto square3 = m_scene->createEntity();
        square3.addComponent<gaunlet::Prefab::Components::ModelComponent>(gaunlet::Prefab::Sprites::Square2DModel());
        square3.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, -2.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 40.0f),
            glm::vec3(1.8f, 1.8f, 1.0f)
        );
        square3.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f), texture2);

        // CIRCLE 1
        auto circle1 = m_scene->createEntity();
        circle1.addComponent<gaunlet::Prefab::Components::CircleComponent>(0.1f, 0.01f);
        circle1.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(5.0f, 3.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        circle1.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

        // CIRCLE 2
        auto circle2 = m_scene->createEntity();
        circle2.addComponent<gaunlet::Prefab::Components::CircleComponent>(0.5f, 0.1f);
        circle2.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(5.0f, 0.5f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 20.0f),
            glm::vec3(1.5f, 1.2f, 1.0f)
        );
        circle2.addComponent<gaunlet::Scene::MaterialComponent>(texture1);

        // CIRCLE 3
        auto circle3 = m_scene->createEntity();
        circle3.addComponent<gaunlet::Prefab::Components::CircleComponent>(1.0f, 0.3f);
        circle3.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(5.0f, -2.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 40.0f),
            glm::vec3(1.8f, 2.2f, 1.0f)
        );
        circle3.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.8f, 0.0f, 1.0f), texture2);

    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {
        m_renderPipeline->run(m_scene, m_camera);
    }

private:
    gaunlet::Core::Ref<gaunlet::Scene::Scene> m_scene;
    gaunlet::Core::Ref<gaunlet::Scene::OrthographicCamera> m_camera;
    gaunlet::Core::Ref<gaunlet::Prefab::RenderPipelines::Basic3DRenderPipeline> m_renderPipeline;

};

int main() {

    auto window = gaunlet::Core::CreateRef<gaunlet::Core::Window>("Rendering 2D");
    gaunlet::Core::RunLoop runLoop(window);
    Rendering2DApplication app;

    runLoop.run(app);

    return 0;

}