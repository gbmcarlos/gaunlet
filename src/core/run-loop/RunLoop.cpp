#include "RunLoop.h"

#include "../render/RenderCommand.h"
#include "../imgui/ImGuiRenderApi.h"

namespace engine {

    RunLoop::RunLoop(Application& application)
        : m_application(application) {

        RenderCommand::init();

        ImGuiRenderApi::init(m_application.getWindow()->getContext());

    }

    void RunLoop::run() {

        m_application.onReady();

        float lastFrameTime = 0;

        // Start the main loop
        while (m_application.isRunning()) {

            float time = (float) glfwGetTime();
            TimeStep ts(time - lastFrameTime);
            lastFrameTime = time;

            // Start ImGUI rendering
            ImGuiRenderApi::newFrame();

            // Delegate the update to the m_application
            m_application.onUpdate(ts);

            // Delegate the GUI to the m_application
            m_application.onGuiRender();

            ImGuiRenderApi::render();

            m_application.getWindow()->swap();
            m_application.getWindow()->pollEvents();

        }

        ImGuiRenderApi::shutdown();
        m_application.getWindow()->close();

    }

}