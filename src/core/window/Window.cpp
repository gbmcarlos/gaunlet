#include "Window.h"

#include "../input/EventBus.h"
#include "../input/events/WindowEvent.h"
#include "../input/events/KeyEvent.h"
#include "../input/events/MouseEvent.h"
#include "../input/events/CursorEvent.h"
#include "../input/events/ScrollEvent.h"

#include <stdexcept>

namespace engine {

    Window::Window(int width, int height, const std::string& title)
        : m_width(width), m_height(height), m_viewportWidth(0), m_viewportHeight(0), m_title(title) {
        init();
    }

    Window::Window(const std::string& title)
        : m_width(0), m_height(0), m_viewportWidth(0), m_viewportHeight(0), m_title(title) {
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

        glfwGetWindowSize(m_windowContext, &m_viewportWidth, &m_viewportHeight);

        glfwSetWindowCloseCallback(m_windowContext, windowCloseEventCallback);
        glfwSetWindowSizeCallback(m_windowContext, windowResizeEventCallback);
        glfwSetKeyCallback(m_windowContext, keyboardEventCallback);
        glfwSetMouseButtonCallback(m_windowContext, mouseEventCallback);
        glfwSetCursorPosCallback(m_windowContext, cursorPositionCallback);
        glfwSetScrollCallback(m_windowContext, scrollCallback);

        bind();

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

    void Window::close() {
        glfwDestroyWindow(m_windowContext);
    }

    void Window::windowCloseEventCallback(GLFWwindow *window) {

        WindowCloseEvent event;
        EventBus::getInstance().publishEvent(event);

    }

    void Window::windowResizeEventCallback(GLFWwindow *window, int width, int height) {

        WindowResizeEvent event(width, height);
        EventBus::getInstance().publishEvent(event);

    }

    void Window::keyboardEventCallback(GLFWwindow *window, int key, int scanCode, int action, int mods) {

        if (action == GLFW_PRESS) {
            KeyPressEvent event(key);
            EventBus::getInstance().publishEvent(event);
        } else if (action == GLFW_REPEAT) {
            KeyRepeatEvent event(key);
            EventBus::getInstance().publishEvent(event);
        } else if (action == GLFW_RELEASE) {
            KeyReleaseEvent event(key);
            EventBus::getInstance().publishEvent(event);
        }

    }

    void Window::mouseEventCallback(GLFWwindow* window, int button, int action, int mods) {

        if (action == GLFW_PRESS) {
            MouseButtonPress event(button);
            EventBus::getInstance().publishEvent(event);
        } else if (action == GLFW_RELEASE) {
            MouseButtonRelease event(button);
            EventBus::getInstance().publishEvent(event);
        }

    }

    void Window::cursorPositionCallback(GLFWwindow *window, double xPosition, double yPosition) {
        CursorMoveEvent event((float) xPosition, (float) yPosition);
        EventBus::getInstance().publishEvent(event);
    }

    void Window::scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
        ScrollEvent event((float) xOffset, (float) yOffset);
        EventBus::getInstance().publishEvent(event);
    }

}