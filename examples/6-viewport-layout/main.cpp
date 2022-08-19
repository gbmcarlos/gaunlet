#include <Layout.h>

#include "glm/glm.hpp"

#include <array>
#include <utility>

class SceneLayer : public engine::Layer {

public:

    engine::Scene m_mainScene;
    std::shared_ptr<engine::PerspectiveCamera> m_camera;
    std::shared_ptr<engine::Framebuffer> m_framebuffer = nullptr;

public:

    SceneLayer(unsigned int viewportWidth, unsigned int viewportHeight, unsigned int framebufferWidth, unsigned int framebufferHeight) {

        GE_PROFILE_FUNCTION

        m_camera = std::make_shared<engine::PerspectiveCamera>(45.0f, (float) viewportWidth /(float) viewportHeight, 100, 1.0f, 100.0f);
        m_camera->setTranslation({0.0f, 2.0f, 10.0f});

        m_framebuffer = std::make_shared<engine::Framebuffer>(std::initializer_list<engine::FramebufferAttachmentSpec>{
            {engine::FramebufferAttachmentType::Color, engine::FramebufferDataFormat::RGBA},
            {engine::FramebufferAttachmentType::Depth, engine::FramebufferDataFormat::Depth}
        }, framebufferWidth, framebufferHeight);

        auto triangle1 = m_mainScene.createEntity();
        triangle1.addComponent<engine::PolygonModelComponent>(engine::Triangle2DModel());
        triangle1.addComponent<engine::TransformComponent>(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        triangle1.addComponent<engine::MaterialComponent>(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));

        m_mainScene.start();

    }

    void onUpdate(engine::TimeStep timeStep) override {
        m_mainScene.render(m_camera, m_framebuffer);
    }

};

class SettingsNode : public engine::GuiDockedNode {

    void onGuiRender() override {

        ImGui::Text("Window size: %d %d", getNodeWidth(), getNodeHeight());

        if (isHovered()) {
            ImGui::Text("%d %d", getMousePositionX(), getMousePositionY());
        }

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

    void onEvent() override {

    }

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
        m_editorLayer->pushNode("Settings", new SettingsNode);
        m_editorLayer->pushNode("Tools", new ToolsNode);
        m_editorLayer->pushNode("Scene", new SceneViewportNode, m_sceneLayer->m_camera, m_sceneLayer->m_framebuffer, 0);

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