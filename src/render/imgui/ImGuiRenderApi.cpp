#include "ImGuiRenderApi.h"

#include <imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace engine {

    void ImGuiRenderApi::init(const Window& window) {

        ::ImGui::CreateContext();
        ImGuiIO &io = ::ImGui::GetIO();
        io.IniFilename = "";
        ImGui_ImplGlfw_InitForOpenGL(window.windowContext, true);
        ImGui_ImplOpenGL3_Init();
        ::ImGui::StyleColorsDark();

    }

    void ImGuiRenderApi::newFrame() {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

    }

    void ImGuiRenderApi::render() {

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }

    void ImGuiRenderApi::shutdown() {

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

    }

}