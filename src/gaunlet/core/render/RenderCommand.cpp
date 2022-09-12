#include "gaunlet/core/render/RenderCommand.h"

namespace gaunlet::Core {

    void RenderCommand::init() {
        getApi().init();
    }

    void RenderCommand::clearColorBuffer(const glm::vec4 &color) {
        getApi().clearColorBuffer(color.r, color.g, color.b, color.a);
    }

    void RenderCommand::clearDepthStencilBuffer() {
        getApi().clearDepthStencilBuffer();
    }

    void RenderCommand::setDepthFunction(DepthStencilFunction function) {
        getApi().setDepthFunction(function);
    }

    void RenderCommand::setStencilFunction(DepthStencilFunction function, unsigned int reference) {
        getApi().setStencilFunction(function, reference);
    }

    void RenderCommand::setStencilOperation(bool enabled, StencilOperation stencilFailOperation, StencilOperation stencilPassDepthFailOperation, StencilOperation depthStencilPassOperation) {
        getApi().setStencilOperation(enabled, stencilFailOperation, stencilPassDepthFailOperation, depthStencilPassOperation);
    }

    void RenderCommand::getViewport(unsigned int& x0, unsigned int& y0, unsigned int& x1, unsigned int& y1) {
        getApi().getViewport(x0, y0, x1, y1);
    }

    void RenderCommand::setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) {
        getApi().setViewport(x0, y0, x1, y1);
    }


    void RenderCommand::createVertexBuffer(unsigned int& id, unsigned int size) {
        getApi().createVertexBuffer(id, size);
    }

    void RenderCommand::createVertexBuffer(unsigned int& id, const void *data, unsigned int size) {
        getApi().createVertexBuffer(id, data, size);
    }

    void RenderCommand::submitVertexBufferData(unsigned int id, const void *data, unsigned int size) {
        getApi().submitVertexBufferData(id, data, size);
    }

    void RenderCommand::deleteVertexBuffer(unsigned int id) {
        getApi().deleteVertexBuffer(id);
    }


    void RenderCommand::createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count) {
        getApi().createIndexBuffer(id, data, count);
    }

    void RenderCommand::deleteIndexBuffer(unsigned int id) {
        getApi().deleteIndexBuffer(id);
    }


    void RenderCommand::createVertexArray(unsigned int& id) {
        getApi().createVertexArray(id);
    }

    void RenderCommand::deleteVertexArray(unsigned int& id) {
        getApi().deleteVertexArray(id);
    }


    unsigned int RenderCommand::sizeOfLayoutElementType(PrimitiveDataType type) {
        return getApi().sizeOfPrimitiveDataType(type);
    }

    void RenderCommand::addVertexArrayAttribute(unsigned int id, unsigned int index, int count, PrimitiveDataType type, bool normalized, int stride, int offset) {
        getApi().addVertexArrayAttribute(id, index, count, type, normalized, stride, offset);
    }


    unsigned int RenderCommand::createShaderProgram() {
        return (unsigned int) getApi().createShaderProgram();
    }

    unsigned int RenderCommand::compileShader(ShaderType type, const std::string &source) {
        return (unsigned int) getApi().compileShader(type, source);
    }

    void RenderCommand::attachShader(unsigned int programId, unsigned int shaderId) {
        getApi().attachShader(programId, shaderId);
    }

    void RenderCommand::compileShaderProgram(unsigned int id) {
        getApi().compileShaderProgram(id);
    }

    void RenderCommand::deleteShader(unsigned int id) {
        getApi().deleteShader(id);
    }


    int RenderCommand::getUniformLocation(unsigned int id, const std::string& name) {
        return (int) getApi().getUniformLocation(id, name);
    }

    void RenderCommand::setUniform1i(unsigned int id, int location, int value) {
        getApi().setUniform1i(id, location, value);
    }

    void RenderCommand::setUniform1f(unsigned int id, int location, float value) {
        getApi().setUniform1f(id, location, value);
    }

    void RenderCommand::setUniform3f(unsigned int id, int location, const glm::vec3& value) {
        getApi().setUniform3f(id, location, value);
    }

    void RenderCommand::setUniform4f(unsigned int id, int location, const glm::vec4& value) {
        getApi().setUniform4f(id, location, value);
    }

    void RenderCommand::setUniformMat3f(unsigned int id, int location, const glm::mat3& value) {
        getApi().setUniformMat3f(id, location, value);
    }

    void RenderCommand::setUniformMat4f(unsigned int id, int location, const glm::mat4& value) {
        getApi().setUniformMat4f(id, location, value);
    }


    void RenderCommand::createUniformBuffer(unsigned int& id, unsigned int size) {
        getApi().createUniformBuffer(id, size);
    }

    void RenderCommand::createUniformBuffer(unsigned int& id, const void *data, unsigned int size) {
        getApi().createUniformBuffer(id, data, size);
    }

    void RenderCommand::submitUniformBufferData(unsigned int id, const void *data, unsigned int size) {
        getApi().submitUniformBufferData(id, data, size);
    }

    void RenderCommand::deleteUniformBuffer(unsigned int id) {
        getApi().deleteUniformBuffer(id);
    }


    int RenderCommand::getUniformBufferBindingIndex(unsigned int id, const std::string& name) {
        return getApi().getUniformBufferBindingIndex(id, name);
    }


    void RenderCommand::bindUniformBufferToBindingPoint(unsigned int bufferId, unsigned int bindingPoint, unsigned int size) {
        getApi().bindUniformBufferToBindingPoint(bufferId, bindingPoint, size);
    }

    void RenderCommand::bindUniformBufferFromBindingPoint(unsigned int shaderId, int location, unsigned int bindingPoint) {
        getApi().bindUniformBufferFromBindingPoint(shaderId, location, bindingPoint);
    }


    void RenderCommand::loadTextureImage2d(unsigned int& id, TextureDataFormat internalFormat, TextureDataFormat dataFormat, PrimitiveDataType dataType, unsigned int width, unsigned int height, void* data) {
        getApi().loadTextureImage2d(id, internalFormat, dataFormat, dataType, width, height, data);
    }

    void RenderCommand::loadTextureCubeMap(unsigned int &id, TextureDataFormat internalFormat, TextureDataFormat dataFormat, unsigned int width, unsigned int height, std::vector<void *> imagesData) {
        getApi().loadTextureCubeMap(id, internalFormat, dataFormat, width, height, std::move(imagesData));
    }

    void RenderCommand::activateTexture(unsigned int id, TextureType type, unsigned int slot) {
        getApi().activateTexture(id, type, slot);
    }

    void RenderCommand::deleteTexture(unsigned int &id) {
        getApi().deleteTexture(id);
    }

    void RenderCommand::createFramebuffer(unsigned int &id) {
        getApi().createFramebuffer(id);
    }

    void RenderCommand::bindFramebuffer(unsigned int id) {
        getApi().bindFramebuffer(id);
    }

    void RenderCommand::unbindFramebuffer() {
        getApi().unbindFramebuffer();
    }

    void RenderCommand::deleteFramebuffer(unsigned int& id) {
        getApi().deleteFramebuffer(id);
    }

    void RenderCommand::framebufferAttach(unsigned int id, TextureType textureType, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, unsigned int textureId) {
        getApi().framebufferAttach(id, textureType, attachmentType, attachmentIndex, textureId);
    }

    void RenderCommand::setDrawBuffers(unsigned int id, const std::vector<int>& drawBuffers) {
        getApi().setDrawBuffers(id, drawBuffers);
    }

    void RenderCommand::checkFramebufferCompleteness(unsigned int id) {
        getApi().checkFramebufferCompleteness(id);
    }

    void RenderCommand::clearColorAttachment(unsigned int id, unsigned int colorAttachmentIndex, PrimitiveDataType dataType, void *value) {
        getApi().clearColorAttachment(id, colorAttachmentIndex, dataType, value);
    }

    void RenderCommand::clearDepthStencilAttachment(unsigned int id, float depthValue, int stencilValue) {
        getApi().clearDepthStencilAttachment(id, depthValue, stencilValue);
    }

    void RenderCommand::readFramebuffer(unsigned int id, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, TextureDataFormat internalFormat, PrimitiveDataType dataType, unsigned int x, unsigned int y, unsigned int width, unsigned int height, void *data) {
        getApi().readFramebuffer(id, attachmentType, attachmentIndex, internalFormat, dataType, x, y, width, height, data);
    }

    void RenderCommand::drawIndexedTriangles(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int indexCount) {
        getApi().drawIndexedTriangles(vertexBufferId, indexBufferId, vertexArrayId, shaderId, indexCount);
    }

    void RenderCommand::drawIndexedLines(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int indexCount) {
        getApi().drawIndexedLines(vertexBufferId, indexBufferId, vertexArrayId, shaderId, indexCount);
    }

}