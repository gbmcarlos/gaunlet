#include "gaunlet/editor/panel/RenderPanel.h"

#include "gaunlet/core/window/Window.h"
#include "gaunlet/editor/workspace/Workspace.h"

namespace gaunlet::Editor {

    void RenderPanel::onUpdate(Core::TimeStep timeStep) {

        getRenderPipeline()->run(
            getScene(),
            getCamera()
        );

    }

    void RenderPanel::resize() {

        getWorkspace()->getCamera(m_cameraId)->resize(
            getWidth(),
            getHeight()
        );

        getWorkspace()->getRenderPipeline(m_renderPipelineId)->resize(
            getWidth() * Core::Window::getCurrentInstance()->getDPI(),
            getHeight() * Core::Window::getCurrentInstance()->getDPI()
        );

    }

    const Core::Ref<Graphics::TextureImage2D>& RenderPanel::getRenderTarget() {
        return getWorkspace()->getRenderPipeline(m_renderPipelineId)->getRenderTarget();
    }

    const Core::Ref<Scene::Scene>& RenderPanel::getScene() {
        return getWorkspace()->getScene(m_sceneId);
    }

    const Core::Ref<Scene::Camera>& RenderPanel::getCamera() {
        return getWorkspace()->getCamera(m_cameraId);
    }

    const Core::Ref<Editor::RenderPipeline>& RenderPanel::getRenderPipeline() {
        return getWorkspace()->getRenderPipeline(m_renderPipelineId);
    }

}