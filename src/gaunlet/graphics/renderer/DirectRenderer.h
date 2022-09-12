#pragma once

#include "gaunlet/graphics/Vertex.h"
#include "gaunlet/graphics/texture/Texture.h"
#include "gaunlet/graphics/shader/Shader.h"

namespace gaunlet::Graphics {

    class DirectRenderer {

    public:

        static void renderIndexedTriangles(const std::vector<gaunlet::Graphics::Vertex>& vertices, const std::vector<unsigned int>& indices, const Core::Ref<Graphics::Texture>& texture, const Core::Ref<Graphics::Shader>& shader);

    };

}