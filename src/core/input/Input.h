#pragma once

#include "../window/Window.h"

namespace engine {

    class Input {

    public:

        static bool isKeyPressed(int keyCode) {
            return Window::getCurrentInstance()->isKeyPressed(keyCode);
        }

    };

}