#pragma once

#include "gaunlet/layout/panel/Panel.h"
#include "gaunlet/scene/Scene.h"
#include "gaunlet/scene/camera/PerspectiveCamera.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"

#include "gaunlet/pch.h"

namespace gaunlet::Layout {

    class RenderPanel : public Panel  {

        friend LayoutLayer;

    public:

        static const unsigned int SceneFramebufferAttachmentIndex = 0;
        static const unsigned int EntityIdFramebufferAttachmentIndex = 1;

        inline gaunlet::Scene::Scene& getScene() {return m_scene; }
        inline gaunlet::Core::Ref<gaunlet::Scene::PerspectiveCamera>& getCamera() {return m_camera; }
        inline gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer>& getFramebuffer() {return m_framebuffer; }
        inline gaunlet::Scene::RenderMode getRenderMode() {return m_renderMode; }

        void setRenderMode(gaunlet::Scene::RenderMode renderMode) {m_renderMode = renderMode; }

        virtual bool onEvent(Core::Event& event) = 0;

    private:

        void onUpdate(gaunlet::Core::TimeStep);

        gaunlet::Scene::Scene m_scene;
        gaunlet::Core::Ref<gaunlet::Scene::PerspectiveCamera> m_camera;
        gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer> m_framebuffer = nullptr;
        gaunlet::Scene::RenderMode m_renderMode = gaunlet::Scene::RenderMode::Faces;

    };

}