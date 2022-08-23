#pragma once

#include "../pch.h"

#include "../model/Vertex.h"
#include "../../graphics/texture/Texture.h"
#include "../../graphics/shader/Shader.h"
#include "../../graphics/framebuffer/Framebuffer.h"

namespace engine::Scene {

    class ForwardRenderer {

    public:

        static void renderMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Core::Ref<Graphics::Texture>>& textures, const Core::Ref<Graphics::Shader>& shader, const Core::Ref<Graphics::Framebuffer>& framebuffer);

    };

}