#pragma once

#include "gaunlet/core/render/RenderApi.h"
#include "gaunlet/graphics/texture/Texture.h"

#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

    enum class FramebufferDataFormat {
        RGBA, Integer, Depth
    };

    struct FramebufferAttachmentSpec {

        FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, const glm::vec4& clearColorValue);
        FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, int clearColorValue);
        FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat);

        Core::FramebufferAttachmentType m_attachmentType;
        FramebufferDataFormat m_dataFormat;
        int m_clearColorIntValue = 0;
        glm::vec4 m_clearColorVec4Value = {};

    };

    class Framebuffer {

    public:
        Framebuffer(const std::initializer_list<FramebufferAttachmentSpec>& attachmentSpecs, unsigned int width, unsigned int height);
        ~Framebuffer();

        void bind();
        void unbind();
        void clear();
        void resize(unsigned int width, unsigned int height);

        inline unsigned int getRendererId() const {return m_rendererId; }
        inline unsigned int getWidth() {return m_width; }
        inline unsigned int getHeight() {return m_height; }
        inline Core::Ref<Texture>& getColorAttachment(unsigned int index) {return m_textures[index]; }

        int readPixel(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y);

    private:
        unsigned int m_width, m_height;
        unsigned int m_lastViewportX0, m_lastViewportY0, m_lastViewportX1, m_lastViewportY1;
        unsigned int m_rendererId = 0;

        std::vector<FramebufferAttachmentSpec> m_colorAttachmentSpecs = {};
        FramebufferAttachmentSpec m_depthAttachmentSpec = {Core::FramebufferAttachmentType::None, FramebufferDataFormat::Depth};

        std::vector<Core::Ref<Texture> > m_textures;

        void recreate();
        void attachColor(FramebufferAttachmentSpec colorAttachmentSpec, unsigned int index);
        void attachDepth(FramebufferAttachmentSpec depthAttachmentSpec);

    };

}