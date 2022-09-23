#include "../include/Editor.h"
#include "../include/Prefab.h"

class ToolsPanel : public gaunlet::Editor::GuiPanel {

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getWidth(), getHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        }

    }

};

class SettingsPanel : public gaunlet::Editor::GuiPanel {

public:

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getWidth(), getHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        } else {
            ImGui::NewLine();
        }

        ImGui::Text("Scene layer:");

        auto renderPanel = getWorkspace()->getRenderPanel("main");

        if (renderPanel->isHovered()) {
            ImGui::Text("%d %d", renderPanel->getMousePositionX(), renderPanel->getMousePositionYInverted());
        } else {
            ImGui::NewLine();
        }

    }

};

class ViewportLayoutApplication : public gaunlet::Core::Application {

public:
    explicit ViewportLayoutApplication() : gaunlet::Core::Application() {}

    void onReady() override {

        auto window = gaunlet::Core::Window::getCurrentInstance();
        unsigned int viewportWidth = window->getViewportWidth();
        unsigned int viewportHeight = window->getViewportHeight();

        m_workspace = new gaunlet::Editor::Workspace();

        // Set the docking layout
        m_workspace->setLayoutSpec({
            {
                {gaunlet::Editor::DockSpacePosition::Left, 0.25f,  {"Settings"}},
                {gaunlet::Editor::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar},
                {gaunlet::Editor::DockSpacePosition::Right, 0.3f,  {"Tools"}},
            }, viewportWidth, viewportHeight
        });

        // Create and push the GUI panels
        m_workspace->pushPanel("settings", new SettingsPanel(), "Settings");
        m_workspace->pushPanel("tools", new ToolsPanel, "Tools");

        // Prepare the components of the main render panel
        m_workspace->addCamera("main", gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth / (float) viewportHeight, 1.0f, 100.0f));
        m_workspace->addScene("main", gaunlet::Core::CreateRef<gaunlet::Scene::Scene>());
        m_workspace->addDirectionalLight("main", gaunlet::Core::CreateRef<gaunlet::Scene::DirectionalLightComponent>());
        m_workspace->addSkybox("main", gaunlet::Core::CreateRef<gaunlet::Scene::SkyboxComponent>());
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

        // Prepare the scene
        auto& mainScene = m_workspace->getScene("main");
        auto& mainCamera = m_workspace->getCamera("main");
        mainCamera->setPosition({0.0f, 0.0f, 5.0f});
        mainCamera->setZoomLevel(1.5f);

        auto triangle = mainScene->createTaggedEntity<gaunlet::Editor::SceneEntityTag>();
        triangle.addComponent<gaunlet::Prefab::Components::ModelComponent>(gaunlet::Prefab::Sprites::Triangle2DModel());
        triangle.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-1.5f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        triangle.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        auto circle = mainScene->createTaggedEntity<gaunlet::Editor::SceneEntityTag>();
        circle.addComponent<gaunlet::Prefab::Components::CircleComponent>(0.3f, 0.01f);
        circle.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(1.5f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        circle.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {
        m_workspace->update(timeStep);
    }

    void onGuiRender() override {
        m_workspace->render();
    }

private:
    gaunlet::Editor::Workspace* m_workspace = nullptr;

};

int main() {

    auto window = gaunlet::Core::CreateRef<gaunlet::Core::Window>("Viewport Layout");
    gaunlet::Core::RunLoop runLoop(window);
    ViewportLayoutApplication app;

    runLoop.run(app);

    return 0;

}