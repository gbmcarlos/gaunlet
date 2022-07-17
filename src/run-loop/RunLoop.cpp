#include "RunLoop.h"

#include "TimeStep.h"
#include "../render/RenderCommand.h"
#include "../render/imgui/ImGuiRenderApi.h"
#include "../utils.h"

namespace engine {

    RunLoop::RunLoop(Window& window)
        : m_window(window), m_application(nullptr) {

        RenderCommand::init();

        ImGuiRenderApi::init(window);

    }

    void RunLoop::run(Application& applicationInstance) {

        m_application = &applicationInstance;

        // Register the events callback
        EventBus::getInstance().setKeyboardEventCallback(GE_BIND_CALLBACK_FN(RunLoop::onEvent));

        m_application->onReady();

        float lastFrameTime = 0;

        // Start the main loop
        while (m_window.shouldRun()) {

            float time = (float) glfwGetTime();
            TimeStep ts(time - lastFrameTime);
            lastFrameTime = time;

            // Start ImGUI rendering
            ImGuiRenderApi::newFrame();

            // Delegate the update to the m_application
            m_application->onUpdate(ts);

            // Delegate the GUI to the m_application
            m_application->onGuiRender();

            ImGuiRenderApi::render();

            m_window.swap();
            m_window.pollEvents();

        }

        ImGuiRenderApi::shutdown();

    }

    void RunLoop::onEvent(Event& event) {
        m_application->onEvent(event);
    }

}