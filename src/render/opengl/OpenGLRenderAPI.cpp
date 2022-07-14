#include "OpenGLRenderAPI.h"
#include "opengl_utils.h"

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

    void OpenGLRenderApi::createVertexBuffer(unsigned int& id, unsigned int size) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    void OpenGLRenderApi::createVertexBuffer(unsigned int& id, const void *data, unsigned int size) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void OpenGLRenderApi::bindVertexBuffer(unsigned int& id) {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }


    void OpenGLRenderApi::submitVertexBufferData(const void *data, unsigned int size) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    void OpenGLRenderApi::unbindVertexBuffer() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLRenderApi::createIndexBuffer(unsigned int& id, unsigned int *data, unsigned int count) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }

    void OpenGLRenderApi::bindIndexBuffer(unsigned int& id) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void OpenGLRenderApi::unbindIndexBuffer() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OpenGLRenderApi::deleteBuffer(unsigned int& id) {
        glDeleteBuffers(1, &id);
    }

    void OpenGLRenderApi::createVertexArray(unsigned int& id) {
        glGenVertexArrays(1, &id);
    }

    void OpenGLRenderApi::bindVertexArray(unsigned int& id) {
        glBindVertexArray(id);
    }

    void OpenGLRenderApi::unbindVertexArray() {
        glBindVertexArray(0);
    }

    void OpenGLRenderApi::deleteVertexArray(unsigned int& id) {
        glDeleteVertexArrays(1, &id);
    }

    void OpenGLRenderApi::addVertexArrayAttribute(unsigned int index, int count, GLenum type, bool normalized, int stride, int offset) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
                index,
                count,
                type,
                normalized ? GL_TRUE : GL_FALSE,
                stride,
                (const void*) offset
        );
    }

}