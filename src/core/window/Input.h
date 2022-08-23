#pragma once

#include "core/window/Window.h"

namespace engine::Core {

    class Input {

    public:

        static bool isKeyPressed(int keyCode) {
            return Window::getCurrentInstance()->isKeyPressed(keyCode);
        }

        static unsigned int getMousePositionX() {
            return Window::getCurrentInstance()->getMousePositionX();
        }

        static unsigned int getMousePositionY() {
            return Window::getCurrentInstance()->getMousePositionY();
        }

    };

}