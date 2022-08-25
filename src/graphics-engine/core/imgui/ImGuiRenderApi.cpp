#include "graphics-engine/core/imgui/ImGuiRenderApi.h"

namespace engine::Core {

    void ImGuiRenderApi::init(GLFWwindow* windowContext) {

        ::ImGui::CreateContext();
        ImGuiIO &io = ::ImGui::GetIO();
//        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.IniFilename = "";
        ImGui_ImplGlfw_InitForOpenGL(windowContext, true);
        ImGui_ImplOpenGL3_Init();
        ::ImGui::StyleColorsDark();

    }

    void ImGuiRenderApi::newFrame() {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

    }

    void ImGuiRenderApi::render() {

        ImGuiIO &io = ::ImGui::GetIO();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

    }

    void ImGuiRenderApi::shutdown() {

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

    }

}