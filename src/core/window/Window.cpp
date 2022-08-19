#include "Window.h"

#include "../input/EventBus.h"
#include "../input/events/WindowEvent.h"
#include "../input/events/KeyEvent.h"
#include "../input/events/MouseEvent.h"
#include "../input/events/CursorEvent.h"
#include "../input/events/ScrollEvent.h"

namespace engine {

    Window::Window(unsigned int width, unsigned int height, const std::string& title)
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

        if (!glfwInit()) {
            throw std::runtime_error("GLFW initialization failed");
        }

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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
        glfwGetFramebufferSize(m_windowContext, &m_framebufferWidth, &m_framebufferHeight);

        glfwSetWindowCloseCallback(m_windowContext, windowCloseEventCallback);
        glfwSetWindowSizeCallback(m_windowContext, windowResizeEventCallback);
        glfwSetKeyCallback(m_windowContext, keyboardEventCallback);
        glfwSetMouseButtonCallback(m_windowContext, mouseEventCallback);
        glfwSetCursorPosCallback(m_windowContext, cursorPositionCallback);
        glfwSetScrollCallback(m_windowContext, scrollCallback);

        bind();

        glfwSetWindowUserPointer(m_windowContext, this);

    }

    void Window::setTitle(const std::string& title) {
        m_title = std::move(title);
        glfwSetWindowTitle(m_windowContext, m_title.c_str());
    }

    void Window::swap() {
        glfwSwapBuffers(m_windowContext);
    }

    void Window::pollEvents() {
        GE_PROFILE_FUNCTION
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

    bool Window::isKeyPressed(int keyCode) {

        auto state = glfwGetKey(m_windowContext, keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;

    }

    void Window::windowCloseEventCallback(GLFWwindow *window) {

        WindowCloseEvent event;
        EventBus::getInstance().publishEvent(event);

    }

    void Window::windowResizeEventCallback(GLFWwindow *window, int width, int height) {

        Window* instance = getCurrentInstance();
        instance->m_width = width;
        instance->m_height = height;
        glfwGetWindowSize(instance->m_windowContext, &instance->m_viewportWidth, &instance->m_viewportHeight);
        glfwGetFramebufferSize(instance->m_windowContext, &instance->m_framebufferWidth, &instance->m_framebufferHeight);

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

    Window* Window::getCurrentInstance() {

        GLFWwindow* currentContext = glfwGetCurrentContext();
        auto* currentWindow = (Window*) glfwGetWindowUserPointer(currentContext);
        return currentWindow;

    }

}