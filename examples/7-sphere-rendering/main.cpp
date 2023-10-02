#include "../include/Scene.h"
#include "../include/Prefab.h"

class SphereRenderingApplication : public gaunlet::Core::Application {

public:
    explicit SphereRenderingApplication() : gaunlet::Core::Application() {}

    void onReady() override {

        auto window = gaunlet::Core::Window::getCurrentInstance();
        unsigned int viewportWidth = window->getViewportWidth();
        unsigned int viewportHeight = window->getViewportHeight();

        m_workspace = new gaunlet::Editor::Workspace({false, true});

        // Set the docking layout
        m_workspace->setLayoutSpec({
            {
                {gaunlet::Editor::DockSpacePosition::Left, 0.2f,  {"Render Panel"}},
                {gaunlet::Editor::DockSpacePosition::Down, 0.4, 0, {"Entity Components"}},
                {gaunlet::Editor::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar},
                {gaunlet::Editor::DockSpacePosition::Right, 0.25f,  {"Tools Manager"}},
            }, viewportWidth, viewportHeight
        });

        // Create and push the GUI panels
        m_workspace->pushPanel("render-panel", new gaunlet::Prefab::GuiPanels::RenderPanelComponentsPanel, "Render Panel");
        m_workspace->pushPanel("components", new gaunlet::Prefab::GuiPanels::EntityComponentsPanel, "Entity Components");
        m_workspace->pushPanel("tools", new gaunlet::Prefab::GuiPanels::ToolsManagerPanel, "Tools Manager");

        // Prepare the components of the main render panel
        m_workspace->addCamera("main", gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth / (float) viewportHeight, 1.0f, 100.0f));
        m_workspace->addScene("main", gaunlet::Core::CreateRef<gaunlet::Scene::Scene>());
        m_workspace->addRenderPipeline("main", gaunlet::Core::CreateRef<gaunlet::Prefab::RenderPipelines::BasicEditorRenderPipeline>(
            gaunlet::Core::CreateRef<gaunlet::Scene::DirectionalLightComponent>(
                glm::vec3(0.8f, 0.8f, 0.8f),
                glm::vec3(-0.2f, -1.0f, -0.3f),
                0.5f, 0.7f
            ),
            gaunlet::Core::CreateRef<gaunlet::Scene::SkyboxComponent>(gaunlet::Core::CreateRef<gaunlet::Prefab::Skyboxes::SimpleSkyboxCubeMap>())
        ));

        // Create and push the main render panel, referencing the main components
        m_workspace->pushPanel(
            "main",
            new gaunlet::Editor::RenderPanel(),
            "Scene",
            "main",
            "main",
            "main"
        );

        // Create and push the tools
        m_workspace->addTool("fp-camera-controller", gaunlet::Core::CreateRef<gaunlet::Prefab::EditorTools::FirstPersonCameraController>("main", 10.0f, 0.5f));
        m_workspace->addTool("camera-controller", gaunlet::Core::CreateRef<gaunlet::Prefab::EditorTools::TransformerTool>());
        m_workspace->activateTool("fp-camera-controller");

        auto& mainScene = m_workspace->getScene("main");
        auto& mainCamera = m_workspace->getCamera("main");
        mainCamera->setPosition({0.0f, 10.0f, 15.0f});
//        mainCamera->setPitch(-30.0f);

        gaunlet::Core::Ref<gaunlet::Graphics::TextureImage2D> texture1 = gaunlet::Core::CreateRef<gaunlet::Graphics::TextureImage2D>(
            "assets/projection.png",
            gaunlet::Core::TextureFilteringParameterValue::Linear,
            gaunlet::Core::TextureWrappingParameterValue::MirroredRepeat
        );

        auto sphere = mainScene->createTaggedEntity<gaunlet::Editor::SceneEntityTag>();
        sphere.addComponent<gaunlet::Prefab::Components::ModelComponent>(
            gaunlet::Core::CreateRef<gaunlet::Prefab::MeshGenerators::PlaneSphereGenerator>(
                5.0f,
                40,
                mainCamera
            )
        );

        sphere.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        sphere.addComponent<gaunlet::Scene::MaterialComponent>(texture1);

        m_workspace->selectSceneEntity(sphere);

    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {
        m_workspace->update(timeStep);
    }

    void onGuiRender() override {
        m_workspace->render();
    }

    void onEvent(gaunlet::Core::Event &event) override {
        m_workspace->onEvent(event);
    }

private:

    gaunlet::Editor::Workspace* m_workspace = nullptr;

};

int main() {

    auto window = gaunlet::Core::CreateRef<gaunlet::Core::Window>("Sphere Rendering");
    gaunlet::Core::RunLoop runLoop(window);
    SphereRenderingApplication app;

    runLoop.run(app);

    return 0;

}