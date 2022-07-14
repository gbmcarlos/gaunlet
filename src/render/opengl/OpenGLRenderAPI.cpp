#include "OpenGLRenderAPI.h"
#include "opengl_utils.h"

#include <iostream>

#include <GL/glew.h>

namespace engine {

    void OpenGLRenderApi::init() {

        // Set up OpenGL, with GLEW
        if (glewInit() != GLEW_OK) {
            std::cout << "GLEW initialization failed" << std::endl;
        }

        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }

    }

    void OpenGLRenderApi::setClearColor(float red, float green, float blue, float alpha) {
        glClearColor(red, green, blue, alpha);
    }

    void OpenGLRenderApi::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

}