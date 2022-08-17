#pragma once

#include <GL/glew.h>

#include "../RenderCommand.h"
#include "../RenderApi.h"

#include "../pch.h"

namespace engine {

    class OpenGLRenderApi : public RenderApi {

    private:
        OpenGLRenderApi() {};

    public:
        static RenderApi& getInstance(){
            static OpenGLRenderApi m_instance;
            return m_instance;
        }
        OpenGLRenderApi(OpenGLRenderApi const&) = delete;
        void operator=(OpenGLRenderApi const&)  = delete;

    public:

        void init();

        void setClearColor(float red, float green, float blue, float alpha);
        void clear();

        void getViewport(unsigned int& x0, unsigned int& y0, unsigned int& x1, unsigned int& y1);
        void setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);

        void createVertexBuffer(unsigned int& id, unsigned int size);
        void createVertexBuffer(unsigned int& id, const void *data, unsigned int size);
        void bindVertexBuffer(unsigned int& id);
        void submitVertexBufferData(const void *data, unsigned int size);
        void unbindVertexBuffer();

        void createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count);
        void bindIndexBuffer(unsigned int& id);
        void unbindIndexBuffer();

        void deleteBuffer(unsigned int& id);

        void createVertexArray(unsigned int& id);
        void bindVertexArray(unsigned int& id);
        void unbindVertexArray();
        void deleteVertexArray(unsigned int& id);

        unsigned int createShaderProgram();
        unsigned int compileShader(ShaderType type, const std::string& source);
        void attachShader(unsigned id, unsigned int shaderId);
        void deleteShader(unsigned int id);
        void compileShaderProgram(unsigned int id);
        void bindShader(unsigned int id);

        unsigned int getUniformLocation(unsigned int id, const std::string& name);
        void setUniform1i(int location, int value);
        void setUniform3f(int location, const glm::vec3& value);
        void setUniform4f(int location, const glm::vec4& value);
        void setUniformMat3f(int location, const glm::mat3& value);
        void setUniformMat4f(int location, const glm::mat4& value);

        unsigned int sizeOfVertexBufferLayoutElementType(VertexBufferLayoutElementType type);
        void addVertexArrayAttribute(unsigned int index, int count, VertexBufferLayoutElementType type, bool normalized, int stride, int offset);

        void loadTexture(unsigned int& id, TextureDataFormat internalFormat, TextureDataFormat dataFormat, unsigned int width, unsigned int height, void* data);
        void bindTexture(unsigned int id, unsigned int slot);
        void deleteTexture(unsigned int& id);

        void createFramebuffer(unsigned int& id);
        void bindFramebuffer(unsigned int id);
        void unbindFramebuffer();
        void deleteFramebuffer(unsigned int& id);
        void framebufferAttach(TextureType textureType, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, unsigned int textureId);
        void setDrawBuffers(const std::vector<FramebufferAttachmentType>& drawBuffers);
        void checkFramebufferCompleteness(unsigned int id);

        void drawIndexedTriangles(unsigned int indexCount);
        void drawIndexedLines(unsigned int indexCount);

    private:
        GLenum convertVertexBufferLayoutElementType(VertexBufferLayoutElementType type);
        GLenum convertShaderType(ShaderType type);
        GLenum convertTextureDataFormat(TextureDataFormat format);
        GLenum convertTextureType(TextureType type);
        GLenum convertFramebufferAttachmentType(FramebufferAttachmentType type);

    };

}