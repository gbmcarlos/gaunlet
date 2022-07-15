#pragma once

#include <memory>

#include "../shader/Shader.h"
#include "../buffer/VertexArray.h"

namespace engine {

    class Renderer {

    public:
        Renderer() = delete;
        Renderer(Renderer const&) = delete;
        void operator=(Renderer const&)  = delete;

    public:
        static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    };

}