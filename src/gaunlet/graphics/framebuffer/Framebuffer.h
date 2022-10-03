#pragma once

#include "gaunlet/core/render/RenderApi.h"
#include "gaunlet/graphics/texture/Texture.h"

#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

    enum class FramebufferDataFormat {
        // Color
            R_Color, RG_Color, RGB_Color, RGBA_Color,
        // HDR
            R_HDR, RG_HDR, RGB_HDR, RGBA_HDR,
        // Signed Integer
            R_Integer, RG_Integer, RGB_Integer, RGBA_Integer,
        // Float
            R_Float, RG_Float, RGB_Float, RGBA_Float,
        // Depth & Stencil
            DepthStencil
    };

    struct FramebufferAttachmentSpec {

        FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, int clearColorValue);
        FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, float clearColorValue);
        FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, const glm::vec2& clearColorValue);
        FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, const glm::vec3& clearColorValue);
        FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat, const glm::vec4& clearColorValue);
        FramebufferAttachmentSpec(Core::FramebufferAttachmentType attachmentType, FramebufferDataFormat framebufferDataFormat);

        Core::FramebufferAttachmentType m_attachmentType;
        FramebufferDataFormat m_dataFormat;
        int m_clearColorIntValue = 0;
        float m_clearColorFloatValue = 0;
        glm::vec2 m_clearColorVec2Value = {};
        glm::vec3 m_clearColorVec3Value = {};
        glm::vec4 m_clearColorVec4Value = {};

    };

    class Framebuffer {

    public:
        Framebuffer(const std::initializer_list<FramebufferAttachmentSpec>& attachmentSpecs, unsigned int width, unsigned int height);
        ~Framebuffer();

        void bind();
        void unbind();
        void clear();
        void clearColorAttachment(unsigned int index);
        void clearDepthStencilAttachment();
        void resize(unsigned int width, unsigned int height);
        void setDrawBuffers(const std::vector<int>& drawBuffers);
        void deAttachColor(unsigned int colorAttachmentIndex);

        inline unsigned int getRendererId() const {return m_rendererId; }
        inline unsigned int getWidth() {return m_width; }
        inline unsigned int getHeight() {return m_height; }
        inline Core::Ref<Texture>& getColorAttachment(unsigned int index) {return m_textures[index]; }

        template<typename T>
        T readPixel(unsigned int colorAttachmentIndex, unsigned int x, unsigned int y);

    private:
        unsigned int m_width, m_height;
        unsigned int m_lastViewportX0, m_lastViewportY0, m_lastViewportX1, m_lastViewportY1;
        unsigned int m_rendererId = 0;

        std::vector<FramebufferAttachmentSpec> m_colorAttachmentSpecs = {};
        FramebufferAttachmentSpec m_depthStencilAttachmentSpec = {Core::FramebufferAttachmentType::None, FramebufferDataFormat::DepthStencil};

        std::vector<Core::Ref<Texture>> m_textures;

        void recreate();
        void attachColor(FramebufferAttachmentSpec colorAttachmentSpec, unsigned int index);
        void attachDepthStencil(FramebufferAttachmentSpec depthStencilAttachmentSpec);
        std::tuple<Core::TextureInternalFormat, Core::TextureExternalFormat> convertFramebufferFormat(FramebufferDataFormat format);

    };

}