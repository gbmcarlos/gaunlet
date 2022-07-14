#include "RunLoop.h"

#include <iostream>
#include <sstream>

#include "../render/Renderer.h"
#include "TimeStep.h"
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

        float lastFrameTime = 0;

        // Start the main loop
        while (!glfwWindowShouldClose(window.windowContext)) {

            float time = (float) glfwGetTime();
            TimeStep ts(time - lastFrameTime);
            lastFrameTime = time;

            // Start ImGUI rendering
            ImGuiRenderApi::newFrame();

            // Delegate the update to the application
            application->onUpdate(ts);

            // Delegate the GUI to the application
            application->onGuiRender();

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