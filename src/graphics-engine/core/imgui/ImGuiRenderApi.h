#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

namespace engine::Core {

    class ImGuiRenderApi {

    public:

        static void init(GLFWwindow* windowContext);
        static void newFrame();
        static void render();
        static void shutdown();

    };

}