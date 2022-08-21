#pragma once

#include "../pch.h"

#include "../model/Vertex.h"
#include "../../graphics/texture/Texture.h"
#include "../../graphics/shader/Shader.h"
#include "../../graphics/framebuffer/Framebuffer.h"

namespace engine {

    class ForwardRenderer {

    public:

        static void renderMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures, const Ref<Shader>& shader, const Ref<Framebuffer>& framebuffer);

    };

}