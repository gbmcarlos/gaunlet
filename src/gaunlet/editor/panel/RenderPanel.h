#pragma once

#include "gaunlet/editor/panel/Panel.h"
#include "gaunlet/scene/Scene.h"
#include "gaunlet/scene/camera/PerspectiveCamera.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"
#include "gaunlet/core/event/events/MouseEvent.h"

#include "gaunlet/pch.h"

namespace gaunlet::Editor {

    class RenderPanel : public Panel  {

        friend Workspace;

    public:

        RenderPanel();

        static const unsigned int SceneFramebufferAttachmentIndex = 0;
        static const unsigned int SceneEntityIdFramebufferAttachmentIndex = 1;
        static const unsigned int UIEntityIdFramebufferAttachmentIndex = 2;

        inline const char* getCameraId() {return m_cameraId; }
        inline const char* getSceneId() {return m_sceneId; }
        inline const char* getDirectionalLightId() {return m_directionalLightId; }
        inline const char* getSkyboxId() {return m_skyboxId; }
        inline Scene::RenderMode getRenderMove() {return m_renderMode; }

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
        Scene::RenderMode m_renderMode;

        gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer> m_framebuffer = nullptr;

        Scene::Entity m_selectedSceneEntity = {};
        Scene::Entity m_selectedUIEntity = {};
        std::function<void(Scene::Entity&)> m_sceneSelectionCallback;
        std::function<void(Scene::Entity&)> m_uiSelectionCallback;

    };

}