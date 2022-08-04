#pragma once

#include "RenderApi.h"

#include <glm/glm.hpp>
#include <memory>

namespace engine {

    class RenderCommand {

    public:
        RenderCommand() = delete;
        RenderCommand(RenderCommand const&) = delete;
        void operator=(RenderCommand const&)  = delete;

    private:
        static RenderApi& m_apiInstance;
        static RenderApi& getApi() {return m_apiInstance;}

    public:
        static void init();
        static void clear(const glm::vec4& color);

        static void createVertexBuffer(unsigned int& id, unsigned int size);
        static void createVertexBuffer(unsigned int& id, const void *data, unsigned int size);
        static void bindVertexBuffer(unsigned int& id);
        static void submitVertexBufferData(const void *data, unsigned int size);
        static void unbindVertexBuffer();

        static void createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count);
        static void bindIndexBuffer(unsigned int&);
        static void unbindIndexBuffer();

        static void deleteBuffer(unsigned int&);

        static void createVertexArray(unsigned int& id);
        static void bindVertexArray(unsigned int& id);
        static void unbindVertexArray();
        static void deleteVertexArray(unsigned int& id);

        static unsigned int createShaderProgram();
        static unsigned int compileShader(ShaderType type, const std::string& source);
        static void attachShader(unsigned id, unsigned int shaderId);
        static void compileShaderProgram(unsigned int id);
        static void bindShader(unsigned int id);
        static void deleteShader(unsigned int id);

        static int getUniformLocation(unsigned int id, const std::string& name);
        static void setUniform1i(int location, int value);
        static void setUniform3f(int location, const glm::vec3& value);
        static void setUniform4f(int location, const glm::vec4& value);
        static void setUniformMat3f(int location, const glm::mat3& value);
        static void setUniformMat4f(int location, const glm::mat4& value);

        static unsigned int sizeOfLayoutElementType(VertexBufferLayoutElementType type);
        static void addVertexArrayAttribute(unsigned int index, int count, VertexBufferLayoutElementType type, bool normalized, int stride, int offset);

        static void loadTexture(unsigned int& id, TextureDataFormat internalFormat, TextureDataFormat dataFormat, unsigned int width, unsigned int height, void* data);
        static void bindTexture(unsigned int id, unsigned int slot);
        static void deleteTexture(unsigned int& id);

        static void createFramebuffer(unsigned int& id);
        static void bindFramebuffer(unsigned int id);
        static void unbindFramebuffer(unsigned int id);
        static void deleteFramebuffer(unsigned int& id);
        static void framebufferAttach(TextureType type, FramebufferAttachmentType attachment, unsigned int textureId);
        static void setDrawBuffers(const std::vector<FramebufferAttachmentType>& drawBuffers);
        static void checkFramebufferCompleteness(unsigned int id);

        static void drawIndexedTriangles(unsigned int indexCount);
        static void drawIndexedLines(unsigned int indexCount);

    };

}