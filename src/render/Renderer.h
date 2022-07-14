#pragma once

#include "memory"

#include <glm/glm.hpp>

#include "opengl/OpenGLRenderAPI.h"

namespace engine {

    class Renderer {

    public:
        Renderer() = delete;
        Renderer(Renderer const&) = delete;
        void operator=(Renderer const&)  = delete;

    public:
        static void init();
        static void clear(glm::vec4 color);

    };

}