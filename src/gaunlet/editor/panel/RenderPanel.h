#pragma once

#include "gaunlet/editor/panel/Panel.h"
#include "gaunlet/scene/Scene.h"
#include "gaunlet/scene/camera/PerspectiveCamera.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"
#include "gaunlet/core/event/events/MouseEvent.h"

#include "gaunlet/pch.h"

namespace gaunlet::Editor {

    class RenderPanel : public Panel  {

        friend LayoutLayer;

    public:

        virtual void onReady() {}

        static const unsigned int SceneFramebufferAttachmentIndex = 0;
        static const unsigned int SceneEntityIdFramebufferAttachmentIndex = 1;
        static const unsigned int UIEntityIdFramebufferAttachmentIndex = 2;

        inline gaunlet::Scene::Scene& getScene() {return m_scene; }
        inline gaunlet::Core::Ref<gaunlet::Scene::PerspectiveCamera>& getCamera() {return m_camera; }
        inline Scene::RenderMode getRenderMode() {return m_renderMode; }

        void setRenderMode(gaunlet::Scene::RenderMode renderMode) {m_renderMode = renderMode; }
        void startScene() {m_scene.start(); }

        void selectSceneEntity(Scene::Entity entity);
        void selectUIEntity(Scene::Entity entity);

        void setSceneSelectionCallback(const std::function<void(Scene::Entity&)>& callback);
        void setUISelectionCallback(const std::function<void(Scene::Entity&)>& callback);

    protected:

        // To be called by the LayoutLayer (friend), or overriden
        virtual bool onEvent(Core::Event& event) {return true;}
        virtual void onUpdate(gaunlet::Core::TimeStep);

        inline Core::Ref<gaunlet::Graphics::Framebuffer>& getFramebuffer() {return m_framebuffer; }
        void mousePickEntity(unsigned int mousePositionX, unsigned int mousePositionY);

    private:

        gaunlet::Scene::Scene m_scene;
        gaunlet::Core::Ref<gaunlet::Scene::PerspectiveCamera> m_camera;
        gaunlet::Core::Ref<gaunlet::Graphics::Framebuffer> m_framebuffer = nullptr;
        gaunlet::Scene::RenderMode m_renderMode = gaunlet::Scene::RenderMode::Faces;
        gaunlet::Scene::DirectionalLightComponent m_directionalLight;

        Scene::Entity m_selectedSceneEntity = {};
        Scene::Entity m_selectedUIEntity = {};
        std::function<void(Scene::Entity&)> m_sceneSelectionCallback;
        std::function<void(Scene::Entity&)> m_uiSelectionCallback;

    };

}