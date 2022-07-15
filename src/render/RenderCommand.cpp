#include "RenderCommand.h"

namespace engine {

    void RenderCommand::init() {
        getApi().init();
    }


    void RenderCommand::clear(const glm::vec4& color) {
        getApi().setClearColor(color.r, color.g, color.b, color.a);
        getApi().clear();
    }


    void RenderCommand::createVertexBuffer(unsigned int& id, unsigned int size) {
        getApi().createVertexBuffer(id, size);
    }

    void RenderCommand::createVertexBuffer(unsigned int& id, const void *data, unsigned int size) {
        getApi().createVertexBuffer(id, data, size);
    }

    void RenderCommand::bindVertexBuffer(unsigned int& id) {
        getApi().bindVertexBuffer(id);
    }

    void RenderCommand::submitVertexBufferData(const void *data, unsigned int size) {
        getApi().submitVertexBufferData(data, size);
    }

    void RenderCommand::unbindVertexBuffer() {
        getApi().unbindVertexBuffer();
    }


    void RenderCommand::createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count) {
        getApi().createIndexBuffer(id, data, count);
    }

    void RenderCommand::bindIndexBuffer(unsigned int& id) {
        getApi().bindIndexBuffer(id);
    }

    void RenderCommand::unbindIndexBuffer() {
        getApi().unbindIndexBuffer();
    }


    void RenderCommand::deleteBuffer(unsigned int& id) {
        getApi().deleteBuffer(id);
    }


    void RenderCommand::createVertexArray(unsigned int& id) {
        getApi().createVertexArray(id);
    }

    void RenderCommand::bindVertexArray(unsigned int& id) {
        getApi().bindVertexArray(id);
    }

    void RenderCommand::unbindVertexArray() {
        getApi().unbindVertexArray();
    }

    void RenderCommand::deleteVertexArray(unsigned int& id) {
        getApi().deleteVertexArray(id);
    }

    void RenderCommand::addVertexArrayAttribute(unsigned int index, int count, GLenum type, bool normalized, int stride, int offset) {
        getApi().addVertexArrayAttribute(index, count, type, normalized, stride, offset);
    }

}