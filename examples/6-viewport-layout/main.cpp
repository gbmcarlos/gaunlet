#include <Layout.h>

#include "glm/glm.hpp"

#include <array>
#include <utility>

class SceneLayer : public engine::Layer {

public:

    engine::Scene m_mainScene;
    engine::Ref<engine::PerspectiveCamera> m_camera;
    engine::Ref<engine::Framebuffer> m_framebuffer = nullptr;

public:

    SceneLayer(unsigned int viewportWidth, unsigned int viewportHeight, unsigned int framebufferWidth, unsigned int framebufferHeight) {

        GE_PROFILE_FUNCTION

        m_camera = engine::CreateRef<engine::PerspectiveCamera>(45.0f, (float) viewportWidth /(float) viewportHeight, 100, 1.0f, 100.0f);
        m_camera->setTranslation({0.0f, 2.0f, 10.0f});

        m_framebuffer = engine::CreateRef<engine::Framebuffer>(std::initializer_list<engine::FramebufferAttachmentSpec>{
            {engine::FramebufferAttachmentType::Color, engine::FramebufferDataFormat::RGBA, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)},
            {engine::FramebufferAttachmentType::Color, engine::FramebufferDataFormat::Integer, -1},
            {engine::FramebufferAttachmentType::Depth, engine::FramebufferDataFormat::Depth}
        }, framebufferWidth, framebufferHeight);

        auto triangle1 = m_mainScene.createEntity();
        triangle1.addComponent<engine::PolygonModelComponent>(engine::Triangle2DModel());
        triangle1.addComponent<engine::TransformComponent>(
            glm::vec3(-2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        triangle1.addComponent<engine::MaterialComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        auto triangle2 = m_mainScene.createEntity();
        triangle2.addComponent<engine::PolygonModelComponent>(engine::Triangle2DModel());
        triangle2.addComponent<engine::TransformComponent>(
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        triangle2.addComponent<engine::MaterialComponent>(glm::vec4(0.2f, 0.8f, 0.0f, 1.0f));

        m_mainScene.start();

    }

    void onUpdate(engine::TimeStep timeStep) override {
        m_mainScene.render(m_camera, m_framebuffer);
    }

};

class ToolsNode : public engine::GuiDockedNode {

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getNodeWidth(), getNodeHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        }

    }

};

class SceneViewportNode : public engine::RenderDockedNode {

public:
    int m_selectedEntityId = -1;

public:

    explicit SceneViewportNode(engine::Ref<engine::Framebuffer> framebuffer) : m_framebuffer(std::move(framebuffer)) {}

    bool onEvent(engine::Event &event) override {

        engine::EventDispatcher dispatcher(event);
        dispatcher.dispatch<engine::MouseButtonPress>(GE_BIND_CALLBACK_FN(SceneViewportNode::onMouseButtonPressEvent));
        return true;

    }

    bool onMouseButtonPressEvent(engine::MouseButtonPress& event) {

        unsigned int pixelPositionX = getMousePositionX() * engine::Window::getCurrentInstance()->getDPI();
        unsigned int pixelPositionY = getMousePositionYInverted() * engine::Window::getCurrentInstance()->getDPI();

        m_selectedEntityId = m_framebuffer->readPixel(1, pixelPositionX, pixelPositionY);

        return false;

    }

private:

    engine::Ref<engine::Framebuffer> m_framebuffer = nullptr;

};

class SettingsNode : public engine::GuiDockedNode {

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

class ViewportLayoutApplication : public engine::Application {

public:
    explicit ViewportLayoutApplication(const std::string &name) : engine::Application(name) {}

    void onReady() override {

        // Create the layer that will render to a framebuffer
        m_sceneLayer = new SceneLayer(m_window->getViewportWidth(), m_window->getViewportHeight(), m_window->getFramebufferWidth(), m_window->getFramebufferHeight());

        // Create and set up the layer that will show the different dock nodes
        m_editorLayer = new engine::LayoutLayer(m_window);
        m_editorLayer->setLayoutSpec({
{
                    {engine::DockSpacePosition::Left, 0.25f,  {"Settings"}},
                    {engine::DockSpacePosition::Center, 0.0f,  {"Scene"}, ImGuiDockNodeFlags_NoTabBar},
                    {engine::DockSpacePosition::Right, 0.3f,  {"Tools"}},
               }, m_window->getViewportWidth(), m_window->getViewportHeight()
        });

        auto* sceneViewportNode = new SceneViewportNode(m_sceneLayer->m_framebuffer);

        m_editorLayer->pushNode("Settings", new SettingsNode(sceneViewportNode));
        m_editorLayer->pushNode("Tools", new ToolsNode);
        m_editorLayer->pushNode("Scene", sceneViewportNode, m_sceneLayer->m_camera, m_sceneLayer->m_framebuffer, 0);

        pushLayer(m_sceneLayer);
        pushLayer(m_editorLayer);
    }

private:
    SceneLayer* m_sceneLayer = nullptr;
    engine::LayoutLayer* m_editorLayer = nullptr;

};

int main() {

    ViewportLayoutApplication app("Viewport Layout");
    engine::RunLoop runLoop(app);

    runLoop.run();

    return 0;

}