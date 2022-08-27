#include "../include/Layout.h"

class ToolsPanel : public gaunlet::Layout::GuiPanel {

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getNodeWidth(), getNodeHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        }

    }

};

class ScenePanel : public gaunlet::Layout::RenderPanel {

public:
    int m_selectedEntityId = -1;

public:

    bool onEvent(gaunlet::Core::Event &event) override {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GE_BIND_CALLBACK_FN(ScenePanel::onMouseButtonPressEvent));
        return true;

    }

    bool onMouseButtonPressEvent(gaunlet::Core::MouseButtonPress& event) {

        unsigned int pixelPositionX = getMousePositionX() * gaunlet::Core::Window::getCurrentInstance()->getDPI();
        unsigned int pixelPositionY = getMousePositionYInverted() * gaunlet::Core::Window::getCurrentInstance()->getDPI();

        m_selectedEntityId = getFramebuffer()->readPixel(
            gaunlet::Layout::RenderPanel::EntityIdFramebufferAttachmentIndex,
            pixelPositionX,
            pixelPositionY
        );

        return false;

    }

};

class SettingsPanel : public gaunlet::Layout::GuiPanel {

public:

    SettingsPanel(ScenePanel* sceneNode) : m_sceneNode(sceneNode) {}

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getNodeWidth(), getNodeHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        } else {
            ImGui::NewLine();
        }

        ImGui::Text("Scene layer:");

        if (m_sceneNode->isHovered()) {
            ImGui::Text("%d %d", m_sceneNode->getMousePositionX(), m_sceneNode->getMousePositionYInverted());
        } else {
            ImGui::NewLine();
        }

        ImGui::Text("Selected Entity:");

        if (m_sceneNode->m_selectedEntityId > -1) {
            ImGui::Text("%d", m_sceneNode->m_selectedEntityId);
        } else {
            ImGui::NewLine();
        }

    }

private:
    ScenePanel* m_sceneNode = nullptr;

};

class ViewportLayoutApplication : public gaunlet::Core::Application {

public:
    explicit ViewportLayoutApplication(const std::string &name) : gaunlet::Core::Application(name) {}

    void onReady() override {

        // Create and set up the layer that will show the different dock nodes
        m_editorLayer = new gaunlet::Layout::LayoutLayer(m_window);
        m_editorLayer->setLayoutSpec({
{
                    {gaunlet::Layout::DockSpacePosition::Left, 0.25f,  {"Settings"}},
                    {gaunlet::Layout::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar},
                    {gaunlet::Layout::DockSpacePosition::Right, 0.3f,  {"Tools"}},
               }, m_window->getViewportWidth(), m_window->getViewportHeight()
        });

        auto* scenePanel = new ScenePanel();

        m_editorLayer->pushPanel("Settings", new SettingsPanel(scenePanel));
        m_editorLayer->pushPanel("Tools", new ToolsPanel);
        m_editorLayer->pushPanel("Scene", scenePanel);

        auto triangle = scenePanel->getScene().createEntity();
        triangle.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Triangle2DModel());
        triangle.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        triangle.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        auto circle = scenePanel->getScene().createEntity();
        circle.addComponent<gaunlet::Scene::CircleComponent>(0.1f, 0.01f);
        circle.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        circle.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

        scenePanel->getScene().start();

        pushLayer(m_editorLayer);
    }

private:
    gaunlet::Layout::LayoutLayer* m_editorLayer = nullptr;

};

int main() {

    ViewportLayoutApplication app("Viewport Layout");
    gaunlet::Core::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}