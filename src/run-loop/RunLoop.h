#pragma once

#include <string>
#include <memory>

#include "../application/Application.h"
#include "../window/Window.h"

namespace engine {

    class RunLoop {
    private:
        Application* application;
        Window window;
    public:
        RunLoop(const Window& window);
        void run(Application* application);
        void onEvent(Event& event);
    };

}