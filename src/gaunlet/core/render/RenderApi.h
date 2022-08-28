#pragma once

#include "gaunlet/pch.h"

namespace gaunlet::Core {

    enum class PrimitiveDataType {
        Bool,
        Int,
        UInt,
        Float,
        UByte
    };

    enum class ShaderType {
        Vertex,
        Geometry,
        Fragment
    };

    enum class TextureDataFormat {
        RGB, RGBA,
        RedInteger32, RedInteger,
        Depth, Stencil
    };

    enum class TextureType {
        Image2D,
        CubeMap
    };

    enum class FramebufferAttachmentType {
        None,
        Color,
        Depth,
        Stencil
    };

    class RenderApi {

    public:

        virtual void init() = 0;

        virtual void clearColorBuffer(float red, float green, float blue, float alpha) = 0;
        virtual void clearDepthBuffer() = 0;

        virtual void getViewport(unsigned int& x0, unsigned int& y0, unsigned int& x1, unsigned int& y1) = 0;
        virtual void setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) = 0;

        virtual void createVertexBuffer(unsigned int& id, unsigned int size) = 0;
        virtual void createVertexBuffer(unsigned int& id, const void *data, unsigned int size) = 0;
        virtual void submitVertexBufferData(unsigned int id, const void *data, unsigned int size) = 0;
        virtual void deleteVertexBuffer(unsigned int id) = 0;

        virtual void createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count) = 0;
        virtual void deleteIndexBuffer(unsigned int id) = 0;

        virtual void createVertexArray(unsigned int& id) = 0;
        virtual void deleteVertexArray(unsigned int& id) = 0;

        virtual unsigned int createShaderProgram() = 0;
        virtual unsigned int compileShader(ShaderType type, const std::string& source) = 0;
        virtual void attachShader(unsigned programId, unsigned int shaderId) = 0;
        virtual void deleteShader(unsigned int id) = 0;
        virtual void compileShaderProgram(unsigned int id) = 0;

        virtual unsigned int getUniformLocation(unsigned int id, const std::string& name) = 0;
        virtual void setUniform1i(unsigned int id, int location, int value) = 0;
        virtual void setUniform3f(unsigned int id, int location, const glm::vec3& value) = 0;
        virtual void setUniform4f(unsigned int id, int location, const glm::vec4& value) = 0;
        virtual void setUniformMat3f(unsigned int id, int location, const glm::mat3& value) = 0;
        virtual void setUniformMat4f(unsigned int id, int location, const glm::mat4& value) = 0;

        virtual void createUniformBuffer(unsigned int& id, unsigned int size) = 0;
        virtual void createUniformBuffer(unsigned int& id, const void *data, unsigned int size) = 0;
        virtual void submitUniformBufferData(unsigned int id, const void *data, unsigned int size) = 0;
        virtual void deleteUniformBuffer(unsigned int id) = 0;

        virtual int getUniformBufferBindingIndex(unsigned int id, const std::string& name) = 0;
        virtual void bindUniformBufferToBindingPoint(unsigned int bufferId, unsigned int bindingPoint, unsigned int size) = 0;
        virtual void bindUniformBufferFromBindingPoint(unsigned int shaderId, int location, unsigned int bindingPoint) = 0;

        virtual unsigned int sizeOfPrimitiveDataType(PrimitiveDataType type) = 0;
        virtual void addVertexArrayAttribute(unsigned int vertexArrayId, unsigned int index, int count, PrimitiveDataType type, bool normalized, int stride, int offset) = 0;

        virtual void loadTexture(unsigned int& id, TextureType type, TextureDataFormat internalFormat, TextureDataFormat dataFormat, unsigned int width, unsigned int height, void* data) = 0;
        virtual void activateTexture(unsigned int id, TextureType type, unsigned int slot) = 0;
        virtual void deleteTexture(unsigned int& id) = 0;

        virtual void createFramebuffer(unsigned int& id) = 0;
        virtual void bindFramebuffer(unsigned int id) = 0;
        virtual void unbindFramebuffer() = 0;
        virtual void deleteFramebuffer(unsigned int& id) = 0;
        virtual void framebufferAttach(unsigned int id, TextureType textureType, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, unsigned int textureId) = 0;
        virtual void setDrawBuffers(unsigned int id, const std::vector<int>& drawBuffers) = 0;
        virtual void checkFramebufferCompleteness(unsigned int id) = 0;
        virtual void clearColorAttachment(unsigned int id, unsigned int colorAttachmentIndex, PrimitiveDataType dataType, void* value) = 0;
        virtual void clearDepthAttachment(unsigned int id) = 0;

        virtual void readFramebuffer(unsigned int id, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, TextureDataFormat internalFormat, PrimitiveDataType dataType, unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* data) = 0;

        virtual void drawIndexedTriangles(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int indexCount) = 0;
        virtual void drawIndexedLines(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int indexCount) = 0;

    };

}