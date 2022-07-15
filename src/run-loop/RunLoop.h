#pragma once

#include <string>
#include <memory>

#include "../window/Window.h"
#include "../application/Application.h"

namespace engine {

    class RunLoop {
    private:
        Application* m_application;
        Window m_window;
    public:
        RunLoop(Window& window);
        void run(Application& application);
        void onEvent(Event& event);
    };

}