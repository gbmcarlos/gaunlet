#pragma once

#include "../pch.h"

#include "../../graphics/vertex-buffer/VertexBuffer.h"
#include "../../graphics/index-buffer/IndexBuffer.h"
#include "../../graphics/vertex-array/VertexArray.h"
#include "../../graphics/texture/Texture.h"
#include "../../graphics/shader/Shader.h"
#include "../../graphics/framebuffer/Framebuffer.h"
#include "../model/polygon-mesh/PolygonVertex.h"
#include "../model/circle-mesh/CircleVertex.h"

namespace engine {

    class ForwardRenderer {

    public:

        static void renderPolygons(const std::vector<PolygonVertex>& polygonVertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures, const Ref<Shader>& shader, const Ref<Framebuffer>& framebuffer);

        static void renderCircles(const std::vector<CircleVertex>& polygonVertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures, const Ref<Shader>& shader, const Ref<Framebuffer>& framebuffer);

    private:

        template<typename T>
        static void renderTriangles(BufferLayout& bufferLayout, const std::vector<T>& polygonVertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures, const Ref<Shader>& shader, const Ref<Framebuffer>& framebuffer);

    };

}