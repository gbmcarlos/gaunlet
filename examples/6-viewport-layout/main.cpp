#include "../include/Layout.h"

class SceneLayer : public gaunlet::Core::Layer {

public:

    gaunlet::Scene::Scene m_mainScene;
    gaunlet::Core::Ref<gaunlet::Scene::PerspectiveCamera> m_camera;
    gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer> m_framebuffer = nullptr;

public:

    SceneLayer(unsigned int viewportWidth, unsigned int viewportHeight, unsigned int framebufferWidth, unsigned int framebufferHeight) {

        GE_PROFILE_FUNCTION

        m_camera = gaunlet::Core::CreateRef<gaunlet::Scene::PerspectiveCamera>(45.0f, (float) viewportWidth /(float) viewportHeight, 100, 1.0f, 100.0f);
        m_camera->setTranslation({0.0f, 2.0f, 10.0f});

        m_framebuffer = gaunlet::Core::CreateRef<gaunlet::Graphics::Framebuffer>(std::initializer_list<gaunlet::Graphics::FramebufferAttachmentSpec>{
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::RGBA, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)},
            {gaunlet::Core::FramebufferAttachmentType::Color, gaunlet::Graphics::FramebufferDataFormat::Integer, -1},
            {gaunlet::Core::FramebufferAttachmentType::Depth, gaunlet::Graphics::FramebufferDataFormat::Depth}
        }, framebufferWidth, framebufferHeight);

        auto triangle = m_mainScene.createEntity();
        triangle.addComponent<gaunlet::Scene::ModelComponent>(gaunlet::Scene::Triangle2DModel());
        triangle.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(-2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        triangle.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        auto circle = m_mainScene.createEntity();
        circle.addComponent<gaunlet::Scene::CircleComponent>(0.1f, 0.01f);
        circle.addComponent<gaunlet::Scene::TransformComponent>(
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.8f, 0.8f, 1.0f)
        );
        circle.addComponent<gaunlet::Scene::MaterialComponent>(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

        m_mainScene.start();

    }

    void onUpdate(gaunlet::Core::TimeStep timeStep) override {
        m_mainScene.render(gaunlet::Scene::RenderMode::Faces, m_camera, m_framebuffer);
    }

};

class ToolsNode : public gaunlet::Layout::GuiPanel {

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getNodeWidth(), getNodeHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        }

    }

};

class SceneViewportNode : public gaunlet::Layout::RenderPanel {

public:
    int m_selectedEntityId = -1;

public:

    explicit SceneViewportNode(gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer> framebuffer) : m_framebuffer(std::move(framebuffer)) {}

    bool onEvent(gaunlet::Core::Event &event) override {

        gaunlet::Core::EventDispatcher dispatcher(event);
        dispatcher.dispatch<gaunlet::Core::MouseButtonPress>(GE_BIND_CALLBACK_FN(SceneViewportNode::onMouseButtonPressEvent));
        return true;

    }

    bool onMouseButtonPressEvent(gaunlet::Core::MouseButtonPress& event) {

        unsigned int pixelPositionX = getMousePositionX() * gaunlet::Core::Window::getCurrentInstance()->getDPI();
        unsigned int pixelPositionY = getMousePositionYInverted() * gaunlet::Core::Window::getCurrentInstance()->getDPI();

        m_selectedEntityId = m_framebuffer->readPixel(1, pixelPositionX, pixelPositionY);

        return false;

    }

private:

    gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer> m_framebuffer = nullptr;

};

class SettingsNode : public gaunlet::Layout::GuiPanel {

public:

    SettingsNode(SceneViewportNode* sceneNode) : m_sceneNode(sceneNode) {}

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
    SceneViewportNode* m_sceneNode = nullptr;

};

class ViewportLayoutApplication : public gaunlet::Core::Application {

public:
    explicit ViewportLayoutApplication(const std::string &name) : gaunlet::Core::Application(name) {}

    void onReady() override {

        // Create the layer that will render to a framebuffer
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(), m_window->getViewportHeight(), m_window->getFramebufferWidth(), m_window->getFramebufferHeight());

        // Create and set up the layer that will show the different dock nodes
        m_editorLayer = new gaunlet::Layout::LayoutLayer(m_window);
        m_editorLayer->setLayoutSpec({
{
                    {gaunlet::Layout::DockSpacePosition::Left, 0.25f,  {"Settings"}},
                    {gaunlet::Layout::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar},
                    {gaunlet::Layout::DockSpacePosition::Right, 0.3f,  {"Tools"}},
               }, m_window->getViewportWidth(), m_window->getViewportHeight()
        });

        auto* sceneViewportNode = new SceneViewportNode(m_sceneLayer->m_framebuffer);

        m_editorLayer->pushPanel("Settings", new SettingsNode(sceneViewportNode));
        m_editorLayer->pushPanel("Tools", new ToolsNode);
        m_editorLayer->pushPanel("Scene", sceneViewportNode, m_sceneLayer->m_camera, m_sceneLayer->m_framebuffer, 0);

        pushLayer(m_sceneLayer);
        pushLayer(m_editorLayer);
    }

private:
    SceneLayer* m_sceneLayer = nullptr;
    gaunlet::Layout::LayoutLayer* m_editorLayer = nullptr;

};

int main() {

    ViewportLayoutApplication app("Viewport Layout");
    gaunlet::Core::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}