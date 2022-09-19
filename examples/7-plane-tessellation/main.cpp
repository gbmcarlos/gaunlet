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

        m_workspace->pushPanel("settings", new gaunlet::Editor::WorkspacePropertiesPanel, "Workspace Properties");
        m_workspace->pushPanel("tools", new gaunlet::Editor::ToolsManagerPanel, "Tools Manager");

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
        auto mainCamera = gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth / (float) viewportHeight, 1.0f, -100.0f);
        m_workspace->addCamera("main", mainCamera);
        mainCamera->setPosition({-5.0f, 5.0f, -5.0f});
        mainCamera->setRotation(-90.0f, 0.0);

        // Preview Render Panel, with its own camera
        m_workspace->addRenderPipeline("preview", gaunlet::Core::CreateRef<gaunlet::Prefab::BasicEditorRenderPipeline::BasicEditorRenderPipeline>(gaunlet::Prefab::BasicEditorRenderPipeline::BasicEditorRenderPipeline::getUniformBufferBindingPointOffset()));
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
        auto& previewCamera = m_workspace->addCamera("preview", gaunlet::Core::CreateRef<gaunlet::Scene::OrthographicCamera>(viewportWidth, viewportHeight, 5, 0, 1000));
        previewCamera->setPosition({0, 100, 0});
        previewCamera->lookAt({0, 0, 0});

        // Tools
        m_workspace->addTool("fp-camera-controller", gaunlet::Core::CreateRef<gaunlet::Prefab::EditorTools::FirstPersonCameraController>("main", 5.0f, 0.5f));
        m_workspace->addTool("transformer", gaunlet::Core::CreateRef<gaunlet::Prefab::EditorTools::TransformerTool>());
        m_workspace->activateTool("fp-camera-controller");

        // Prepare the scene
        auto& mainScene = m_workspace->getScene("main");

        gaunlet::Core::Ref<gaunlet::Graphics::TextureImage2D> texture1 = gaunlet::Core::CreateRef<gaunlet::Graphics::TextureImage2D>("assets/texture-1.jpeg");

        auto plane = mainScene->createTaggedEntity<gaunlet::Editor::SceneEntityTag>("plane");
        plane.addComponent<gaunlet::Scene::PlaneComponent>(
            100.0f, // Plane size
            10.0f, 0.5f, // Quad subdivision
            10.0f, 1.1f, // Tessellation
            mainCamera
        );
        plane.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        plane.addComponent<gaunlet::Scene::MaterialComponent>(texture1);
        m_workspace->selectSceneEntity(plane);
        m_plane = plane;

    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {
        m_workspace->update(timeStep);
    }

    void onGuiRender() override {
        m_workspace->render();

        auto& planeComponent = m_plane.getComponent<gaunlet::Scene::PlaneComponent>();

//        ImGui::SliderFloat("Min Camera Distance", &planeComponent.m_minCameraDistance, 0, 1000);
//        ImGui::SliderFloat("Max Camera Distance", &planeComponent.m_maxCameraDistance, 0, 10000);
//        ImGui::SliderFloat("Min Tessellation Level", &planeComponent.m_minTessellationLevel, 1, 100);
//        ImGui::SliderFloat("Max Tessellation Level", &planeComponent.m_maxTessellationLevel, 1, 64);
//        ImGui::SliderFloat("Constant Tessellation Level", &planeComponent.m_tessellationLevel, 0, 64);

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