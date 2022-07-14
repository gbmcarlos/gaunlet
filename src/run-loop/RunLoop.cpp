#include "RunLoop.h"

#include <iostream>
#include <sstream>

#include "../render/Renderer.h"
#include "../render/imgui/ImGuiRenderApi.h"

namespace engine {

    RunLoop::RunLoop(const Window& window)
        : window(window), application(nullptr) {

        Renderer::init();

        ImGuiRenderApi::init(window);

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
            ImGuiRenderApi::newFrame();

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

            ImGuiRenderApi::render();

            window.swap();
            window.pollEvents();

        }

        ImGuiRenderApi::shutdown();

    }

    void RunLoop::onEvent(Event& event) {
        application->onEvent(event);
    }

}