#pragma once

#include "graphics-engine/graphics/texture/Texture.h"
#include "graphics-engine/graphics/shader/Shader.h"
#include "graphics-engine/graphics/framebuffer/Framebuffer.h"
#include "graphics-engine/graphics/Vertex.h"

#include "graphics-engine/pch.h"

namespace engine::Scene {

    class ForwardRenderer {

    public:

        static void renderMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Core::Ref<Graphics::Texture>>& textures, const Core::Ref<Graphics::Shader>& shader, const Core::Ref<Graphics::Framebuffer>& framebuffer);

    };

}