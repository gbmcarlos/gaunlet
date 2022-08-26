#pragma once

#include "graphics-engine/graphics/texture/Texture.h"
#include "graphics-engine/graphics/shader/Shader.h"
#include "graphics-engine/graphics/framebuffer/Framebuffer.h"
#include "graphics-engine/graphics/Vertex.h"

#include "graphics-engine/pch.h"

namespace engine::Graphics {

    class ForwardRenderer {

    public:

        static void renderMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Core::Ref<Texture>>& textures, const Core::Ref<Shader>& shader, const Core::Ref<Framebuffer>& framebuffer);

    };

}