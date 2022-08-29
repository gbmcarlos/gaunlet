#include "../include/Editor.h"

class ToolsPanel : public gaunlet::Editor::GuiPanel {

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getNodeWidth(), getNodeHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        }

    }

};

class SettingsPanel : public gaunlet::Editor::GuiPanel {

public:

    SettingsPanel(gaunlet::Editor::RenderPanel* renderPanel) : m_renderPanel(renderPanel) {}

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getNodeWidth(), getNodeHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        } else {
            ImGui::NewLine();
        }

        ImGui::Text("Scene layer:");

        if (m_renderPanel->isHovered()) {
            ImGui::Text("%d %d", m_renderPanel->getMousePositionX(), m_renderPanel->getMousePositionYInverted());
        } else {
            ImGui::NewLine();
        }

        ImGui::Text("Selected Scene Entity:");

        if (m_renderPanel->m_selectedSceneEntity) {
            ImGui::Text("%d", m_renderPanel->m_selectedSceneEntity.getId());
        } else {
            ImGui::NewLine();
        }

        ImGui::Text("Selected UI Entity:");

        if (m_renderPanel->m_selectedUIEntity) {
            ImGui::Text("%d", m_renderPanel->m_selectedUIEntity.getId());
        } else {
            ImGui::NewLine();
        }

    }

private:
    gaunlet::Editor::RenderPanel* m_renderPanel = nullptr;

};

class ViewportLayoutApplication : public gaunlet::Core::Application {

public:
    explicit ViewportLayoutApplication(const std::string &name) : gaunlet::Core::Application(name) {}

    void onReady() override {

        // Create and set up the layer that will show the different dock nodes
        m_editorLayer = new gaunlet::Editor::LayoutLayer(m_window);
        m_editorLayer->setLayoutSpec({
{
                    {gaunlet::Editor::DockSpacePosition::Left, 0.25f,  {"Settings"}},
                    {gaunlet::Editor::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar},
                    {gaunlet::Editor::DockSpacePosition::Right, 0.3f,  {"Tools"}},
               }, m_window->getViewportWidth(), m_window->getViewportHeight()
        });

        auto* scenePanel = new gaunlet::Editor::RenderPanel();

        m_editorLayer->pushPanel("Settings", new SettingsPanel(scenePanel));
        m_editorLayer->pushPanel("Tools", new ToolsPanel);
        m_editorLayer->pushPanel("Scene", scenePanel);

        auto triangle = scenePanel->createSceneEntity();
        triangle.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Triangle2DModel());
        triangle.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-1.5f, 1.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        triangle.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        auto circle = scenePanel->createSceneEntity();
        circle.addComponent<gaunlet::Scene::CircleComponent>(0.3f, 0.01f);
        circle.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        circle.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

        auto square = scenePanel->createUIEntity(triangle);
        square.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Square2DModel());
        square.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-1.5f, 1.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 1.0f)
        );
        square.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        scenePanel->startScene();

        pushLayer(m_editorLayer);
    }

private:
    gaunlet::Editor::LayoutLayer* m_editorLayer = nullptr;

};

int main() {

    ViewportLayoutApplication app("Viewport Layout");
    gaunlet::Core::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}