#include "gaunlet/core/run-loop/RunLoop.h"

#include "gaunlet/core/render/RenderCommand.h"
#include "gaunlet/core/imgui/ImGuiRenderApi.h"

namespace gaunlet::Core {

    RunLoop::RunLoop(const Core::Ref<Core::Window>& window)
        : m_window(window) {

        RenderCommand::init();
        ImGuiRenderApi::init(window->getContext());

    }

    void RunLoop::run(Application& application) {

        application.onReady();

        float lastFrameTime = 0;

        while (application.isRunning()) {

            float time = (float) glfwGetTime();
            TimeStep ts(time - lastFrameTime);
            lastFrameTime = time;

            m_window->pollEvents();

            // Delegate the update to the m_application
            application.onUpdate(ts);

            // Start ImGUI rendering
            ImGuiRenderApi::newFrame();
            // Delegate the GUI to the application
            application.onGuiRender();

            ImGuiRenderApi::render();

            m_window->swap();

        }

        ImGuiRenderApi::shutdown();
        m_window->close();

    }

}