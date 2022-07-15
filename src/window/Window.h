#pragma once

#include <string>

// GlEW needs to be included before GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../input/EventBus.h"

namespace engine {

    class Window {
    private:
        std::string m_title;
        int m_width;
        int m_height;

        GLFWwindow* m_windowContext;

        void init();

        static void keyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    public:
        Window(std::string title);
        Window(int width, int height, std::string title);
        ~Window();

        bool shouldRun();
        void setTitle(std::string title);
        void swap();
        void pollEvents();
        void bind() const;
        void unbind() const;

        inline std::string getTitle() { return m_title;}
        inline int getWidth() { return m_width;}
        inline int getHeight() { return m_height;}
        inline GLFWwindow*& getContext() {return m_windowContext;}

    };

}
