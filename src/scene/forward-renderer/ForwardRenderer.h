#pragma once

#include "graphics/texture/Texture.h"
#include "graphics/shader/Shader.h"
#include "graphics/framebuffer/Framebuffer.h"
#include "scene/model/Vertex.h"

#include "pch.h"

namespace engine::Scene {

    class ForwardRenderer {

    public:

        static void renderMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Core::Ref<Graphics::Texture>>& textures, const Core::Ref<Graphics::Shader>& shader, const Core::Ref<Graphics::Framebuffer>& framebuffer);

    };

}