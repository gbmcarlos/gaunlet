#pragma once

// GLEW needs to be included before GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gaunlet/pch.h"

namespace gaunlet::Core {

    class Window {
    private:
        std::string m_title;
        int m_width, m_height;
        int m_viewportWidth, m_viewportHeight;
        int m_framebufferWidth, m_framebufferHeight;

        GLFWwindow* m_windowContext;

        void init();

        static void windowCloseEventCallback(GLFWwindow* window);
        static void windowResizeEventCallback(GLFWwindow* window, int width, int height);
        static void keyboardEventCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
        static void mouseEventCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPositionCallback(GLFWwindow* window, double xPosition, double yPosition);
        static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

    public:
        explicit Window(const std::string& title);
        Window(unsigned int width, unsigned int height, const std::string& title);
        ~Window();

        void setTitle(const std::string& title);
        void swap();
        void pollEvents();
        void bind() const;
        void unbind() const;
        void close();

        bool isKeyPressed(int keyCode);
        unsigned int getMousePositionX();
        unsigned int getMousePositionY();

        inline const std::string& getTitle() { return m_title;}

        inline unsigned int getWidth() const { return (unsigned int) m_width;}
        inline unsigned int getHeight() const { return (unsigned int) m_height;}

        inline unsigned int getViewportWidth() const { return (unsigned int) m_viewportWidth;}
        inline unsigned int getViewportHeight() const { return (unsigned int) m_viewportHeight;}

        inline unsigned int getFramebufferWidth() const { return (unsigned int) m_framebufferWidth;}
        inline unsigned int getFramebufferHeight() const { return (unsigned int) m_framebufferHeight;}

        inline float getDPI() const {return m_framebufferWidth / m_viewportWidth; }

        inline GLFWwindow*& getContext() {return m_windowContext;}

        static Window* getCurrentInstance();

    };

}
