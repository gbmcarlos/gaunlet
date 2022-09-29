#pragma once

#include "gaunlet/editor/render-pipeline/RenderPipeline.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"

namespace gaunlet::Prefab::RenderPipelineExtensions {

    class EntitySelectionExtension : public Editor::Extension {

    public:

        enum class EntityLayer {
            SceneLayer, UILayer
        };

        EntitySelectionExtension(Core::Ref<Graphics::Framebuffer> framebuffer, unsigned int sceneEntityIdAttachmentIndex, unsigned int uiEntityIdAttachmentIndex)
            : m_framebuffer(std::move(framebuffer)), m_sceneEntityIdAttachmentIndex(sceneEntityIdAttachmentIndex), m_uiEntityIdAttachmentIndex(uiEntityIdAttachmentIndex) {}

        const char* getName() override {
            return "Entity Selection";
        }

        int getEntityId(EntityLayer layer, unsigned int pixelX, unsigned int pixelY) {

            unsigned int attachmentIndex;
            switch (layer) {
                case EntityLayer::SceneLayer:   attachmentIndex = m_sceneEntityIdAttachmentIndex; break;
                case EntityLayer::UILayer:      attachmentIndex = m_uiEntityIdAttachmentIndex; break;
            }

            if (!m_framebuffer) {
                return -1;
            }

            return m_framebuffer->readPixel<int>(attachmentIndex, pixelX, pixelY);
        }

    private:

        unsigned int m_sceneEntityIdAttachmentIndex = 0;
        unsigned int m_uiEntityIdAttachmentIndex = 1;
        Core::Ref<Graphics::Framebuffer> m_framebuffer;

    };

}