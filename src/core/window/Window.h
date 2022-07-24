#pragma once

// GLEW needs to be included before GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

namespace engine {

    class Window {
    private:
        std::string m_title;
        int m_width, m_height;
        int m_viewportWidth, m_viewportHeight;

        GLFWwindow* m_windowContext;

        void init();

        static void windowCloseEventCallback(GLFWwindow* window);
        static void windowResizeEventCallback(GLFWwindow* window, int width, int height);
        static void keyboardEventCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
        static void mouseEventCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPositionCallback(GLFWwindow* window, double xPosition, double yPosition);
        static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

    public:
        Window(const std::string& title);
        Window(int width, int height, const std::string& title);
        ~Window();

        void setTitle(const std::string& title);
        void swap();
        void pollEvents();
        void bind() const;
        void unbind() const;
        void close();

        bool isKeyPressed(int keyCode);

        inline const std::string& getTitle() { return m_title;}
        inline int getWidth() { return m_width;}
        inline int getHeight() { return m_height;}
        inline int getViewportWidth() { return m_viewportWidth;}
        inline int getViewportHeight() { return m_viewportHeight;}
        inline GLFWwindow*& getContext() {return m_windowContext;}

        static Window* getCurrentInstance();

    };

}
