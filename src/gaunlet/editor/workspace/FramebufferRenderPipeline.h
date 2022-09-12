#pragma once

#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/scene/components/LightingComponents.h"

namespace gaunlet::Editor {

    enum class FramebufferLayer {
        SceneEntity, UIEntity
    };

    class FramebufferRenderPipeline {

    public:

        virtual void run(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera, const Core::Ref<Scene::DirectionalLightComponent>& directionalLight, const Core::Ref<Scene::SkyboxComponent>& skybox) = 0;
        virtual void resize(unsigned int width, unsigned int height) = 0;

        virtual const Core::Ref<Graphics::Texture>& getRenderedTexture() = 0;
        virtual int readFramebuffer(FramebufferLayer layer, unsigned int x, unsigned int y) = 0;

    };

}