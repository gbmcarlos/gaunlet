#include "../include/Editor.h"

class ToolsPanel : public gaunlet::Editor::GuiPanel {

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getNodeWidth(), getNodeHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        }

    }

};

class ScenePanel : public gaunlet::Editor::RenderPanel {

public:

    bool onEvent(gaunlet::Core::Event &event) override {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GE_BIND_CALLBACK_FN(ScenePanel::onMouseButtonPressEvent));
        return true;

    }

private:

    bool onMouseButtonPressEvent(gaunlet::Core::MouseButtonPress& event) {

        mousePickEntity(
            getMousePositionX(),
            getMousePositionYInverted()
        );

        return true;

    }

};

class SettingsPanel : public gaunlet::Editor::GuiPanel {

public:

    gaunlet::Scene::Entity m_selectedSceneEntity;
    gaunlet::Scene::Entity m_selectedUIEntity;

    explicit SettingsPanel(ScenePanel* scenePanel) : m_scenePanel(scenePanel) {
        m_scenePanel->setSceneSelectionCallback(GE_BIND_CALLBACK_FN(SettingsPanel::onSceneSelection));
        m_scenePanel->setUISelectionCallback(GE_BIND_CALLBACK_FN(SettingsPanel::onUISelection));
    }

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getNodeWidth(), getNodeHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        } else {
            ImGui::NewLine();
        }

        ImGui::Text("Scene layer:");

        if (m_scenePanel->isHovered()) {
            ImGui::Text("%d %d", m_scenePanel->getMousePositionX(), m_scenePanel->getMousePositionYInverted());
        } else {
            ImGui::NewLine();
        }

        ImGui::Text("Selected Scene Entity:");

        if (m_selectedSceneEntity) {
            ImGui::Text("%d", m_selectedSceneEntity.getId());
        } else {
            ImGui::NewLine();
        }

        ImGui::Text("Selected UI Entity:");

        if (m_selectedUIEntity) {
            ImGui::Text("%d", m_selectedUIEntity.getId());
        } else {
            ImGui::NewLine();
        }

    }

    void onSceneSelection(gaunlet::Scene::Entity entity) {

        // If we had an entity selected, and there was a gizmo, remove
        if (m_selectedSceneEntity && m_gizmoEntity) {
            m_selectedSceneEntity.destroyChild(m_gizmoEntity);
        }

        m_selectedSceneEntity = entity;

        // If we have an entity, add the gizmo
        if (m_selectedSceneEntity) {
            m_gizmoEntity = gaunlet::Editor::TranslationGizmo::create(m_scenePanel->getScene().getRegistry());
            m_selectedSceneEntity.addChild(m_gizmoEntity);
        }

    }

    void onUISelection(gaunlet::Scene::Entity entity) {
        std::cout << "new ui entity: " << entity.getId() << std::endl;
        m_selectedUIEntity = entity;
    }

private:
    ScenePanel* m_scenePanel = nullptr;
    gaunlet::Scene::Entity m_gizmoEntity;

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

        auto* scenePanel = new ScenePanel();

        m_editorLayer->pushPanel("Settings", new SettingsPanel(scenePanel));
        m_editorLayer->pushPanel("Tools", new ToolsPanel);
        m_editorLayer->pushPanel("Scene", scenePanel);

        scenePanel->getCamera()->setTranslation({3.0f, 0.0f, 10.0f});
        scenePanel->getCamera()->setRotation({0.0f, 10.0f, 0.0f});

        auto cup = scenePanel->getScene().getRegistry().createTaggedEntity<gaunlet::Editor::SceneEntityTag>();
        cup.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Model("assets/cup/cup.obj"));
        cup.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(0.0f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.5f)
        );
        cup.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

        auto triangle = scenePanel->getScene().getRegistry().createTaggedEntity<gaunlet::Editor::SceneEntityTag>();
        triangle.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Triangle2DModel());
        triangle.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(3.0f, -2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        triangle.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

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