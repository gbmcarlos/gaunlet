#include "Renderer.h"

namespace engine {

    void Renderer::init() {
        getApi().init();
    }


    void Renderer::clear(glm::vec4 color) {
        getApi().setClearColor(color.r, color.g, color.b, color.a);
        getApi().clear();
    }


    void Renderer::createVertexBuffer(unsigned int& id, unsigned int size) {
        getApi().createVertexBuffer(id, size);
    }

    void Renderer::createVertexBuffer(unsigned int& id, const void *data, unsigned int size) {
        getApi().createVertexBuffer(id, data, size);
    }

    void Renderer::bindVertexBuffer(unsigned int& id) {
        getApi().bindVertexBuffer(id);
    }

    void Renderer::submitVertexBufferData(const void *data, unsigned int size) {
        getApi().submitVertexBufferData(data, size);
    }

    void Renderer::unbindVertexBuffer() {
        getApi().unbindVertexBuffer();
    }


    void Renderer::createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count) {
        getApi().createIndexBuffer(id, data, count);
    }

    void Renderer::bindIndexBuffer(unsigned int& id) {
        getApi().bindIndexBuffer(id);
    }

    void Renderer::unbindIndexBuffer() {
        getApi().unbindIndexBuffer();
    }


    void Renderer::deleteBuffer(unsigned int& id) {
        getApi().deleteBuffer(id);
    }


    void Renderer::createVertexArray(unsigned int& id) {
        getApi().createVertexArray(id);
    }

    void Renderer::bindVertexArray(unsigned int& id) {
        getApi().bindVertexArray(id);
    }

    void Renderer::unbindVertexArray() {
        getApi().unbindVertexArray();
    }

    void Renderer::deleteVertexArray(unsigned int& id) {
        getApi().deleteVertexArray(id);
    }

    void Renderer::addVertexArrayAttribute(unsigned int index, int count, GLenum type, bool normalized, int stride, int offset) {
        getApi().addVertexArrayAttribute(index, count, type, normalized, stride, offset);
    }

}