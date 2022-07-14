#pragma once

#include "../../window/Window.h"

namespace engine {

    class ImGuiRenderApi {

    public:

        static void init(const Window& window);
        static void newFrame();
        static void render();
        static void shutdown();

    };

}