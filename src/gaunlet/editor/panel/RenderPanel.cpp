#include "gaunlet/editor/panel/RenderPanel.h"

#include "gaunlet/core/window/Window.h"
#include "gaunlet/editor/workspace/Workspace.h"

namespace gaunlet::Editor {

    void RenderPanel::onUpdate(Core::TimeStep timeStep) {

        getWorkspace()->getRenderPipeline(m_renderPipelineId)->run(
            getWorkspace()->getScene(m_sceneId),
            getWorkspace()->getCamera(m_cameraId),
            getWorkspace()->getDirectionalLight(m_directionalLightId),
            getWorkspace()->getSkybox(m_skyboxId)
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

    const Core::Ref<Graphics::Texture>& RenderPanel::getRenderedTexture() {
        return getWorkspace()->getRenderPipeline(m_renderPipelineId)->getRenderedTexture();
    }

}