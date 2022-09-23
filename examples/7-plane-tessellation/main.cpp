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
                {gaunlet::Editor::DockSpacePosition::Left, 0.25f,  {"Workspace Properties"}},
                {gaunlet::Editor::DockSpacePosition::Down, 0.4f,  0, {"Tools Manager"}},
                {gaunlet::Editor::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar},
                {gaunlet::Editor::DockSpacePosition::Right, 0.3f, {"Preview"}},
            }, viewportWidth, viewportHeight
        });

        m_workspace->pushPanel("settings", new gaunlet::Prefab::GuiPanels::WorkspacePropertiesPanel, "Workspace Properties");
        m_workspace->pushPanel("tools", new gaunlet::Prefab::GuiPanels::ToolsManagerPanel, "Tools Manager");

        // Common components
        m_workspace->addScene("main", gaunlet::Core::CreateRef<gaunlet::Scene::Scene>());
        m_workspace->addDirectionalLight("main", gaunlet::Core::CreateRef<gaunlet::Scene::DirectionalLightComponent>(
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3(-0.2f, -1.0f, -0.3f),
            0.5f, 0.7f
        ));
        m_workspace->addSkybox("main", gaunlet::Core::CreateRef<gaunlet::Scene::SkyboxComponent>(gaunlet::Core::CreateRef<gaunlet::Prefab::Skyboxes::SimpleSkyboxCubeMap>()));

        // Main Render Panel, with its own camera
        m_workspace->addRenderPipeline("main", gaunlet::Core::CreateRef<gaunlet::Prefab::BasicEditorRenderPipeline::BasicEditorRenderPipeline>());
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
        // Prepare the main camera
        auto mainCamera = gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth / (float) viewportHeight, 1.0f, -100000.0f);
        m_workspace->addCamera("main", mainCamera);
        mainCamera->setPosition({1.0f, 800.0f, 1.0f});

        // Preview Render Panel, with its own camera
        m_workspace->addRenderPipeline("preview", gaunlet::Core::CreateRef<gaunlet::Prefab::BasicEditorRenderPipeline::BasicEditorRenderPipeline>(gaunlet::Prefab::BasicEditorRenderPipeline::BasicEditorRenderPipeline::getUniformBufferCount()));
        m_workspace->pushPanel(
            "preview",
            new gaunlet::Editor::RenderPanel(),
            "Preview",
            "preview",
            "main",
            "main",
            nullptr,
            "preview"
        );
        // Prepare the preview camera
        auto& previewCamera = m_workspace->addCamera("preview", gaunlet::Core::CreateRef<gaunlet::Scene::OrthographicCamera>(viewportWidth, viewportHeight, 2, 0, 10000));
        previewCamera->setPosition({0, 100, 0});
        previewCamera->lookAt({0, 0, 0});

        // Tools
        m_workspace->addTool("fp-camera-controller", gaunlet::Core::CreateRef<gaunlet::Prefab::EditorTools::FirstPersonCameraController>("main", 300.0f, 0.5f));
        m_workspace->addTool("transformer", gaunlet::Core::CreateRef<gaunlet::Prefab::EditorTools::TransformerTool>());
        m_workspace->activateTool("fp-camera-controller");

        // Prepare the scene
        auto& mainScene = m_workspace->getScene("main");

        gaunlet::Core::Ref<gaunlet::Graphics::TextureImage2D> heightmap = gaunlet::Core::CreateRef<gaunlet::Graphics::TextureImage2D>("assets/heightmap.png");

        auto plane = mainScene->createTaggedEntity<gaunlet::Editor::SceneEntityTag>("plane");
        plane.addComponent<gaunlet::Scene::TerrainComponent>(
            100000.0f, // Plane size
            150.0f, 0.5f, // Quad subdivision
            25.0f, // Triangle size
            1000.0f, // Max height
            mainCamera,
            heightmap
        );
        m_workspace->selectSceneEntity(plane);
        m_plane = plane;

    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {
        m_workspace->update(timeStep);
    }

    void onGuiRender() override {
        m_workspace->render();

        ImGui::ShowMetricsWindow();

        auto& planeComponent = m_plane.getComponent<gaunlet::Scene::TerrainComponent>();

        ImGui::SliderFloat("Triangle Size", &planeComponent.m_triangleSize, 1.0, 500.0f);

    }

    void onEvent(gaunlet::Core::Event &event) override {
        m_workspace->onEvent(event);
    }

private:
    gaunlet::Editor::Workspace* m_workspace = nullptr;
    gaunlet::Scene::Entity m_plane = {};

};

int main() {

    auto window = gaunlet::Core::CreateRef<gaunlet::Core::Window>("Basic editor");
    gaunlet::Core::RunLoop runLoop(window);
    BasicEditorApplication app;

    runLoop.run(app);

    return 0;

}