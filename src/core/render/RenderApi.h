#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace engine {

    enum class VertexBufferLayoutElementType {
        Bool,
        Int,
        Float
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

        virtual void setClearColor(float red, float green, float blue, float alpha) = 0;
        virtual void clear() = 0;

        virtual void getViewport(unsigned int& x0, unsigned int& y0, unsigned int& x1, unsigned int& y1) = 0;
        virtual void setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) = 0;

        virtual void createVertexBuffer(unsigned int& id, unsigned int size) = 0;
        virtual void createVertexBuffer(unsigned int& id, const void *data, unsigned int size) = 0;
        virtual void bindVertexBuffer(unsigned int& id) = 0;
        virtual void submitVertexBufferData(const void *data, unsigned int size) = 0;
        virtual void unbindVertexBuffer() = 0;

        virtual void createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count) = 0;
        virtual void bindIndexBuffer(unsigned int& id) = 0;
        virtual void unbindIndexBuffer() = 0;

        virtual void deleteBuffer(unsigned int& id) = 0;

        virtual void createVertexArray(unsigned int& id) = 0;
        virtual void bindVertexArray(unsigned int& id) = 0;
        virtual void unbindVertexArray() = 0;
        virtual void deleteVertexArray(unsigned int& id) = 0;

        virtual unsigned int createShaderProgram() = 0;
        virtual unsigned int compileShader(ShaderType type, const std::string& source) = 0;
        virtual void attachShader(unsigned id, unsigned int shaderId) = 0;
        virtual void deleteShader(unsigned int id) = 0;
        virtual void compileShaderProgram(unsigned int id) = 0;
        virtual void bindShader(unsigned int id) = 0;

        virtual unsigned int getUniformLocation(unsigned int id, const std::string& name) = 0;
        virtual void setUniform1i(int location, int value) = 0;
        virtual void setUniform3f(int location, const glm::vec3& value) = 0;
        virtual void setUniform4f(int location, const glm::vec4& value) = 0;
        virtual void setUniformMat3f(int location, const glm::mat3& value) = 0;
        virtual void setUniformMat4f(int location, const glm::mat4& value) = 0;

        virtual unsigned int sizeOfVertexBufferLayoutElementType(VertexBufferLayoutElementType type) = 0;
        virtual void addVertexArrayAttribute(unsigned int index, int count, VertexBufferLayoutElementType type, bool normalized, int stride, int offset) = 0;

        virtual void loadTexture(unsigned int& id, TextureDataFormat internalFormat, TextureDataFormat dataFormat, unsigned int width, unsigned int height, void* data) = 0;
        virtual void bindTexture(unsigned int id, unsigned int slot) = 0;
        virtual void deleteTexture(unsigned int& id) = 0;

        virtual void createFramebuffer(unsigned int& id) = 0;
        virtual void bindFramebuffer(unsigned int id) = 0;
        virtual void unbindFramebuffer() = 0;
        virtual void deleteFramebuffer(unsigned int& id) = 0;
        virtual void framebufferAttach(TextureType textureType, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, unsigned int textureId) = 0;
        virtual void setDrawBuffers(const std::vector<FramebufferAttachmentType>& drawBuffers) = 0;
        virtual void checkFramebufferCompleteness(unsigned int id) = 0;

        virtual void drawIndexedTriangles(unsigned int indexCount) = 0;
        virtual void drawIndexedLines(unsigned int indexCount) = 0;

    };

}