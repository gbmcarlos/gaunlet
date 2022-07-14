#pragma once

#include <string>
#include <memory>

#include "../window/Window.h"
#include "../application/Application.h"

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