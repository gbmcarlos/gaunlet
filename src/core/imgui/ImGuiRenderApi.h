#pragma once

#include <GLFW/glfw3.h>

namespace engine {

    class ImGuiRenderApi {

    public:

        static void init(GLFWwindow* windowContext);
        static void newFrame();
        static void render();
        static void shutdown();

    };

}