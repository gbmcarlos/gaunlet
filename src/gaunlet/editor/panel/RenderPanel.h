#pragma once

#include "gaunlet/editor/panel/Panel.h"
#include "gaunlet/scene/entity/Entity.h"
#include "gaunlet/scene/camera/PerspectiveCamera.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"
#include "gaunlet/core/event/events/MouseEvent.h"
#include "gaunlet/core/application/TimeStep.h"

#include "gaunlet/pch.h"

namespace gaunlet::Editor {

    class RenderPanel : public Panel  {

        friend Workspace;

    public:

        inline const char* getCameraId() {return m_cameraId; }
        inline const char* getSceneId() {return m_sceneId; }
        inline const char* getDirectionalLightId() {return m_directionalLightId; }
        inline const char* getSkyboxId() {return m_skyboxId; }
        inline const char* getRenderPipelineId() {return m_renderPipelineId; }

    protected:

        // To be called by the LayoutLayer (friend), or overriden
        virtual bool onEvent(Core::Event& event) {return true; }
        virtual void onUpdate(Core::TimeStep timeStep);
        virtual void resize();
        virtual const Core::Ref<Graphics::Texture>& getRenderedTexture();

    private:

        const char* m_cameraId;
        const char* m_sceneId;
        const char* m_directionalLightId;
        const char* m_skyboxId;
        const char* m_renderPipelineId;

    };

}