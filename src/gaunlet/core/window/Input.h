#pragma once

#include "gaunlet/core/window/Window.h"

namespace gaunlet::Core {

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

        static bool isMouseButtonPressed(int buttonCode) {
            return Window::getCurrentInstance()->isMouseButtonPressed(buttonCode);
        }

    };

}