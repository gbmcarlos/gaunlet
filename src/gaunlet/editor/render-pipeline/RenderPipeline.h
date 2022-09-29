#pragma once

#include "gaunlet/scene/entity/Entity.h"
#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/scene/camera/Camera.h"
#include "gaunlet/editor/render-pipeline/RenderPipelineExtension.h"

#include "gaunlet/pch.h"

namespace gaunlet::Editor {

    class RenderPipeline {

    public:

        virtual void run(const Core::Ref<Scene::Scene>& scene, const Core::Ref<Scene::Camera>& camera) = 0;
        virtual void resize(unsigned int width, unsigned int height) = 0;
        virtual const Core::Ref<Graphics::Texture>& getRenderTarget() = 0;

        template<typename T>
        bool hasExtension() {
            return (m_extensions.find(typeid(T)) != m_extensions.end());
        }

        template<typename T>
        Core::Ref<T> getExtension() {
            static_assert(std::is_base_of<RenderPipelineExtension, T>::value, "T must derive from Extension");
            auto& extension = m_extensions[typeid(T)];
            return std::dynamic_pointer_cast<T>(extension);
        }

    protected:

        template<typename T>
        Core::Ref<T> addExtension(const Core::Ref<T>& extension) {
            static_assert(std::is_base_of<RenderPipelineExtension, T>::value, "T must derive from Extension");
            m_extensions[typeid(T)] = extension;
            return extension;
        }

    private:

        std::unordered_map<std::type_index, Core::Ref<RenderPipelineExtension>> m_extensions;

    };

}