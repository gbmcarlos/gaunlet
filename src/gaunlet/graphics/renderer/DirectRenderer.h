#pragma once

#include "gaunlet/graphics/Vertex.h"
#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/graphics/shader/Shader.h"

namespace gaunlet::Graphics {

    enum class RenderMode {
        Triangle, Quad
    };

    class DirectRenderer {

    public:

        static void renderIndexedVertices(const std::vector<gaunlet::Graphics::Vertex>& vertices, const std::vector<unsigned int>& indices, const Core::Ref<Graphics::Shader>& shader, RenderMode mode);

    };

}