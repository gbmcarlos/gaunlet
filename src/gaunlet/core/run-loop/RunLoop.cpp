#include "gaunlet/core/run-loop/RunLoop.h"

#include "gaunlet/core/render/RenderCommand.h"
#include "gaunlet/core/imgui/ImGuiRenderApi.h"

namespace gaunlet::Core {

    RunLoop::RunLoop(Application& application)
        : m_application(application) {

        GE_PROFILE_BEGIN_SESSION("main", ASSETS_PATH"/result.json")

        GE_PROFILE_SCOPE("init");

        RenderCommand::init();

        ImGuiRenderApi::init(m_application.getWindow()->getContext());

    }

    void RunLoop::run() {

        m_application.onReady();

        float lastFrameTime = 0;

        // Start the main loop
        while (m_application.isRunning()) {

            GE_PROFILE_SCOPE("loop");

            float time = (float) glfwGetTime();
            TimeStep ts(time - lastFrameTime);
            lastFrameTime = time;

            m_application.getWindow()->pollEvents();

            // Delegate the update to the m_application
            m_application.onUpdate(ts);

            // Start ImGUI rendering
            ImGuiRenderApi::newFrame();
            // Delegate the GUI to the application
            m_application.onGuiRender();

            ImGuiRenderApi::render();

            m_application.getWindow()->swap();

        }

        ImGuiRenderApi::shutdown();
        m_application.getWindow()->close();

        GE_PROFILE_END_SESSION();

    }

}