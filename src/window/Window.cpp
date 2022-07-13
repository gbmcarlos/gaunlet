#include <stdexcept>

#include "Window.h"

#include <GL/glew.h>

namespace engine {

    Window::Window(int width, int height, std::string title)
        : width(width), height(height), title(std::move(title)) {
        init();
    }

    Window::Window(std::string title)
        : width(0), height(0), title(std::move(title)) {
        init();
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

        if (!width || !height) {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            width = mode->width;
            height = mode->height;
        }

        windowContext = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (!windowContext) {
            glfwTerminate();
            throw std::runtime_error("GLFW Window context creation failed");
        }

        glfwSetKeyCallback(windowContext, keyboardEventCallback);

        bind();

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

    Window::~Window() {
        glfwTerminate();
    }

    void Window::setTitle(std::string title) {
        glfwSetWindowTitle(windowContext, title.c_str());
    }

    void Window::bind() const {

        if (!windowContext) {
            throw std::runtime_error("Window must be initialized before binding it");
        }

        glfwMakeContextCurrent(windowContext);

    }

    void Window::unbind() const {
        glfwMakeContextCurrent(nullptr);
    }

}