#include <stdexcept>

#include "Window.h"

#include <GL/glew.h>

namespace engine {

    Window::Window(int width, int height, const std::string& title)
        : m_width(width), m_height(height), m_title(title) {
        init();
    }

    Window::Window(const std::string& title)
        : m_width(0), m_height(0), m_title(title) {
        init();
    }

    Window::~Window() {
        glfwTerminate();
    }

    void Window::init() {

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

        if (!glfwInit()) {
            throw std::runtime_error("GLFW initialization failed");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        if (!m_width || !m_height) {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            m_width = mode->width;
            m_height = mode->height;
        }

        m_windowContext = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

        if (!m_windowContext) {
            glfwTerminate();
            throw std::runtime_error("GLFW Window context creation failed");
        }

        glfwSetKeyCallback(m_windowContext, keyboardEventCallback);

        bind();

    }

    bool Window::shouldRun() {
        return !glfwWindowShouldClose(m_windowContext);
    }

    void Window::setTitle(const std::string& title) {
        m_title = std::move(title);
        glfwSetWindowTitle(m_windowContext, m_title.c_str());
    }

    void Window::swap() {
        glfwSwapBuffers(m_windowContext);
    }

    void Window::pollEvents() {
        glfwPollEvents();
    }

    void Window::bind() const {

        if (!m_windowContext) {
            throw std::runtime_error("Window must be initialized before binding it");
        }

        glfwMakeContextCurrent(m_windowContext);

    }

    void Window::unbind() const {
        glfwMakeContextCurrent(nullptr);
    }

    void Window::keyboardEventCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

        if (action == GLFW_PRESS) {
            KeyPressEvent event(key);
            EventBus::getInstance().dispatchRawEvent(event);
        } else if (action == GLFW_REPEAT) {
            KeyRepeatEvent event(key);
            EventBus::getInstance().dispatchRawEvent(event);
        } else if (action == GLFW_RELEASE) {
            KeyReleaseEvent event(key);
            EventBus::getInstance().dispatchRawEvent(event);
        }

    }

}