#pragma once

#include "../core/render/RenderCommand.h"
#include "../texture/Texture.h"

#include <vector>
#include <memory>

namespace engine {

    enum class FramebufferDataFormat {
        RGBA, Integer, Depth
    };

    struct FramebufferAttachmentSpec {

        FramebufferAttachmentSpec(FramebufferDataFormat framebufferDataFormat, FramebufferAttachmentType attachmentType);

        FramebufferDataFormat m_dataFormat;
        FramebufferAttachmentType m_attachmentType;

    };

    class Framebuffer {

    public:
        Framebuffer(const std::initializer_list<FramebufferAttachmentSpec>& attachmentSpecs, unsigned int width, unsigned int height);
        ~Framebuffer();

        void bind();
        void unbind();
        void resize(unsigned int width, unsigned int height);

        inline unsigned int getWidth() {return m_width; }
        inline unsigned int getHeight() {return m_height; }
        inline std::shared_ptr<Texture>& getColorAttachment(unsigned int index) {return m_textures[index]; }

    private:
        unsigned int m_width, m_height;
        unsigned int m_lastViewportX0, m_lastViewportY0, m_lastViewportX1, m_lastViewportY1;
        unsigned int m_rendererId = 0;

        std::vector<FramebufferAttachmentSpec> m_colorAttachmentSpecs = {};
        FramebufferAttachmentSpec m_depthAttachmentSpec = {FramebufferDataFormat::Depth, FramebufferAttachmentType::None};

        std::vector<std::shared_ptr<Texture> > m_textures;

        void recreate();
        void attachColor(FramebufferAttachmentSpec colorAttachmentSpec, unsigned int index);
        void attachDepth(FramebufferAttachmentSpec depthAttachmentSpec);

    };

}