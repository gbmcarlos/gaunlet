#include <iostream>
#include <sstream>

#include <imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../render/Renderer.h"
#include "RunLoop.h"

namespace engine {

    RunLoop::RunLoop(const Window& window)
        : window(window), application(nullptr) {

        Renderer::init();

        // Set up the GUI, with ImGUI
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = "";
        ImGui_ImplGlfw_InitForOpenGL(window.windowContext, true);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();

    }

    void RunLoop::run(Application* applicationInstance) {

        application = applicationInstance;

        // Register the events callback
        EventBus::getInstance().setKeyboardEventCallback(std::bind(&RunLoop::onEvent, this, std::placeholders::_1));

        application->onReady();

        // Prepare the variables to keep track of time and frames
        static double limitFPS = 1.0 / 60.0;
        double lastTime = glfwGetTime(), timer = lastTime;
        double deltaTime = 0, nowTime = 0;
        int frames = 0 , updates = 0;

        // Start the main loop
        while (!glfwWindowShouldClose(window.windowContext)) {

            // Start ImGUI rendering
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            nowTime = glfwGetTime();
            deltaTime += (nowTime - lastTime) / limitFPS;
            lastTime = nowTime;

            while (deltaTime >= 1.0){
                // Delegate the update to the application
                application->onUpdate();
                updates++;
                deltaTime--;
            }

            // Delegate the rendering to the application
            application->onRender();
            frames++;

            // Delegate the GUI to the application
            application->onGuiRender();

            // Consolidate the updates and frames per second
            if (glfwGetTime() - timer > 1.0) {
                timer ++;
                std::stringstream title; title << window.getTitle().c_str() << " - FPS: " << frames << " Updates:" << updates;
                window.setTitle(title.str());
                updates = 0, frames = 0;
            }

            // Render ImGUI
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            window.swap();
            window.pollEvents();

        }

        // Close ImGUI
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

    }

    void RunLoop::onEvent(Event& event) {
        application->onEvent(event);
    }

}