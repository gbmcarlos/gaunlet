#include "Renderer.h"

namespace engine {

    void Renderer::init() {
        OpenGLRenderApi::getInstance().init();
    }

    void Renderer::clear(glm::vec4 color) {
        OpenGLRenderApi::getInstance().setClearColor(color.r, color.g, color.b, color.a);
        OpenGLRenderApi::getInstance().clear();
    }


    void Renderer::createVertexBuffer(unsigned int& id, unsigned int size) {
        OpenGLRenderApi::getInstance().createVertexBuffer(id, size);
    }

    void Renderer::createVertexBuffer(unsigned int& id, const void *data, unsigned int size) {
        OpenGLRenderApi::getInstance().createVertexBuffer(id, data, size);
    }

    void Renderer::bindVertexBuffer(unsigned int& id) {
        OpenGLRenderApi::getInstance().bindVertexBuffer(id);
    }

    void Renderer::submitVertexBufferData(const void *data, unsigned int size) {
        OpenGLRenderApi::getInstance().submitVertexBufferData(data, size);
    }

    void Renderer::unbindVertexBuffer() {
        OpenGLRenderApi::getInstance().unbindVertexBuffer();
    }


    void Renderer::createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count) {
        OpenGLRenderApi::getInstance().createIndexBuffer(id, data, count);
    }

    void Renderer::bindIndexBuffer(unsigned int& id) {
        OpenGLRenderApi::getInstance().bindIndexBuffer(id);
    }

    void Renderer::unbindIndexBuffer() {
        OpenGLRenderApi::getInstance().unbindIndexBuffer();
    }


    void Renderer::deleteBuffer(unsigned int& id) {
        OpenGLRenderApi::getInstance().deleteBuffer(id);
    }


    void Renderer::createVertexArray(unsigned int& id) {
        OpenGLRenderApi::getInstance().createVertexArray(id);
    }

    void Renderer::bindVertexArray(unsigned int& id) {
        OpenGLRenderApi::getInstance().bindVertexArray(id);
    }

    void Renderer::unbindVertexArray() {
        OpenGLRenderApi::getInstance().unbindVertexArray();
    }

    void Renderer::deleteVertexArray(unsigned int& id) {
        OpenGLRenderApi::getInstance().deleteVertexArray(id);
    }

    void Renderer::addVertexArrayAttribute(unsigned int index, int count, GLenum type, bool normalized, int stride, int offset) {
        OpenGLRenderApi::getInstance().addVertexArrayAttribute(index, count, type, normalized, stride, offset);
    }

}