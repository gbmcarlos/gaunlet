#include "../include/Editor.h"
#include "../include/Prefab.h"

class BasicEditorApplication : public gaunlet::Core::Application {

public:
    explicit BasicEditorApplication() : gaunlet::Core::Application() {}

    void onReady() override {

        auto window = gaunlet::Core::Window::getCurrentInstance();
        unsigned int viewportWidth = window->getViewportWidth();
        unsigned int viewportHeight = window->getViewportHeight();

        m_workspace = new gaunlet::Editor::Workspace();

        // Set the docking layout
        m_workspace->setLayoutSpec({
            {
                {gaunlet::Editor::DockSpacePosition::Left, 0.2f,  {"Workspace Properties"}},
                {gaunlet::Editor::DockSpacePosition::Down, 0.4f,  0, {"Tools Manager"}},
                {gaunlet::Editor::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar}
            }, viewportWidth, viewportHeight
        });

        m_workspace->pushPanel("settings", new gaunlet::Editor::WorkspacePropertiesPanel, "Workspace Properties");
        m_workspace->pushPanel("tools", new gaunlet::Editor::ToolsManagerPanel, "Tools Manager");

        // Prepare the components of the main render panel
        m_workspace->addCamera("main", gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth / (float) viewportHeight, 1.0f, 100.0f));
        m_workspace->addScene("main", gaunlet::Core::CreateRef<gaunlet::Scene::Scene>());
        m_workspace->addDirectionalLight("main", gaunlet::Core::CreateRef<gaunlet::Scene::DirectionalLightComponent>(
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(-0.2f, -1.0f, -0.3f),
            0.5f, 0.7f
        ));
        m_workspace->addSkybox("main", gaunlet::Core::CreateRef<gaunlet::Scene::SkyboxComponent>(gaunlet::Core::CreateRef<gaunlet::Prefab::Skyboxes::SimpleSkyboxCubeMap>()));
        m_workspace->addRenderPipeline("main", gaunlet::Core::CreateRef<gaunlet::Prefab::BasicEditorRenderPipeline::BasicEditorRenderPipeline>());

        // Create and push the main render panel, referencing the main components
        m_workspace->pushPanel(
            "main",
            new gaunlet::Editor::RenderPanel(),
            "Scene",
            "main",
            "main",
            "main",
            "main",
            "main"
        );

        m_workspace->addTool("camera-controller", gaunlet::Core::CreateRef<gaunlet::Prefab::EditorTools::GlobalCameraController>(0.01f, 1.0f));
        m_workspace->addTool("transformer", gaunlet::Core::CreateRef<gaunlet::Prefab::EditorTools::TransformerTool>());
        m_workspace->activateTool("camera-controller");

        // Prepare the scene
        auto& mainScene = m_workspace->getScene("main");
        auto& mainCamera = m_workspace->getCamera("main");
        mainCamera->setPosition({0.0f, 60.0f, 10.0f});
        mainCamera->setZoomLevel(1.5f);
        mainCamera->lookAt({0, 0, 0});

        gaunlet::Core::Ref<gaunlet::Graphics::TextureImage2D> texture1 = gaunlet::Core::CreateRef<gaunlet::Graphics::TextureImage2D>("assets/texture-1.jpeg");

        auto plane = mainScene->createTaggedEntity<gaunlet::Editor::SceneEntityTag>("plane");
        plane.addComponent<gaunlet::Scene::PlaneComponent>(5, 5, 1.0f, 1.0f);
        plane.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(10.0f, 10.0f, 10.0f)
        );
        plane.addComponent<gaunlet::Scene::MaterialComponent>(texture1);
        m_workspace->selectSceneEntity(plane);

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

    auto window = gaunlet::Core::CreateRef<gaunlet::Core::Window>("Basic editor");
    gaunlet::Core::RunLoop runLoop(window);
    BasicEditorApplication app;

    runLoop.run(app);


    return 0;

}