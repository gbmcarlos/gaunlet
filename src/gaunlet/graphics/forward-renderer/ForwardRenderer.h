#pragma once

#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/graphics/shader/Shader.h"
#include "gaunlet/graphics/framebuffer/Framebuffer.h"
#include "gaunlet/graphics/Vertex.h"

#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

    class ForwardRenderer {

    public:

        static void renderMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Core::Ref<Texture>>& textures, const Core::Ref<Shader>& shader, const Core::Ref<Framebuffer>& framebuffer);

    };

}