#pragma once

#include "gaunlet/editor/panel/Panel.h"
#include "gaunlet/scene/entity/Entity.h"
#include "gaunlet/scene/camera/PerspectiveCamera.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"
#include "gaunlet/core/event/events/MouseEvent.h"
#include "gaunlet/core/application/TimeStep.h"
#include "gaunlet/editor/render-pipeline/RenderPipeline.h"

#include "gaunlet/pch.h"

namespace gaunlet::Editor {

    class RenderPanel : public Panel  {

        friend Workspace;

    public:

        const Core::Ref<Scene::Scene>& getScene();
        const Core::Ref<Scene::Camera>& getCamera();
        const Core::Ref<Editor::RenderPipeline>& getRenderPipeline();

    protected:

        // To be called by the LayoutLayer (friend), or overloaded
        virtual bool onEvent(Core::Event& event) {return true; }
        virtual void onUpdate(Core::TimeStep timeStep);
        virtual void resize();
        virtual const Core::Ref<Graphics::TextureImage2D>& getRenderTarget();

    private:

        const char* m_sceneId;
        const char* m_cameraId;
        const char* m_renderPipelineId;

    };

}