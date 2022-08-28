#pragma once

#include "gaunlet/layout/panel/Panel.h"
#include "gaunlet/scene/Scene.h"
#include "gaunlet/scene/camera/PerspectiveCamera.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"

#include "gaunlet/pch.h"

namespace gaunlet::Layout {

    struct SceneEntityTag {
        SceneEntityTag() = default;
    };

    struct UIEntityTag {
        UIEntityTag() = default;
    };

    class RenderPanel : public Panel  {

        friend LayoutLayer;

    public:

        static const unsigned int SceneFramebufferAttachmentIndex = 0;
        static const unsigned int SceneEntityIdFramebufferAttachmentIndex = 1;
        static const unsigned int UIEntityIdFramebufferAttachmentIndex = 2;

        inline gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer>& getFramebuffer() {return m_framebuffer; }
        inline gaunlet::Scene::RenderMode getRenderMode() {return m_renderMode; }

        void setRenderMode(gaunlet::Scene::RenderMode renderMode) {m_renderMode = renderMode; }
        void startScene() {m_scene.start(); }

        virtual bool onEvent(Core::Event& event) = 0;

        virtual gaunlet::Scene::Entity createSceneEntity();
        virtual gaunlet::Scene::Entity createUIEntity();

    protected:

        virtual void onUpdate(gaunlet::Core::TimeStep);

    private:

        gaunlet::Scene::Scene m_scene;
        gaunlet::Core::Ref<gaunlet::Scene::PerspectiveCamera> m_camera;
        gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer> m_framebuffer = nullptr;
        gaunlet::Scene::RenderMode m_renderMode = gaunlet::Scene::RenderMode::Faces;

    };

}