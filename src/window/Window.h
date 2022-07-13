#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../input/EventBus.h"

namespace engine {

    class Window {
    private:
        std::string title;
        int width;
        int height;
        void init();

        static void keyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    public:
        Window(std::string title);
        Window(int width, int height, std::string title);
        ~Window();
        void setTitle(std::string title);
        void bind() const;
        void unbind() const;
        GLFWwindow* windowContext;
        inline std::string getTitle() { return title;}
        inline int getWidth() { return width;}
        inline int getHeight() { return height;}
    };

}
