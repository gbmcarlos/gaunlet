#pragma once

#include <GL/glew.h>

#include "graphics-engine/core/render/RenderApi.h"

#include "graphics-engine/pch.h"

namespace engine::Core {

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
        void submitVertexBufferData(unsigned int id, const void *data, unsigned int size);
        void deleteVertexBuffer(unsigned int id);

        void createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count);
        void deleteIndexBuffer(unsigned int id);

        void createVertexArray(unsigned int& id);
        void deleteVertexArray(unsigned int& id);

        unsigned int sizeOfPrimitiveDataType(PrimitiveDataType type);
        void addVertexArrayAttribute(unsigned int vertexArrayId, unsigned int index, int count, PrimitiveDataType type, bool normalized, int stride, int offset);

        unsigned int createShaderProgram();
        unsigned int compileShader(ShaderType type, const std::string& source);
        void attachShader(unsigned programId, unsigned int shaderId);
        void deleteShader(unsigned int id);
        void compileShaderProgram(unsigned int id);

        unsigned int getUniformLocation(unsigned int id, const std::string& name);
        void setUniform1i(unsigned int id, int location, int value);
        void setUniform3f(unsigned int id, int location, const glm::vec3& value);
        void setUniform4f(unsigned int id, int location, const glm::vec4& value);
        void setUniformMat3f(unsigned int id, int location, const glm::mat3& value);
        void setUniformMat4f(unsigned int id, int location, const glm::mat4& value);

        void createUniformBuffer(unsigned int& id, unsigned int size);
        void createUniformBuffer(unsigned int& id, const void *data, unsigned int size);
        void submitUniformBufferData(unsigned int id, const void *data, unsigned int size);
        void deleteUniformBuffer(unsigned int id);

        int getUniformBufferBindingIndex(unsigned int id, const std::string& name);
        void bindUniformBufferToBindingPoint(unsigned int bufferId, unsigned int bindingPoint, unsigned int size);
        void bindUniformBufferFromBindingPoint(unsigned int shaderId, int location, unsigned int bindingPoint);

        void loadTexture(unsigned int& id, TextureType type, TextureDataFormat internalFormat, TextureDataFormat dataFormat, unsigned int width, unsigned int height, void* data);
        void activateTexture(unsigned int id, TextureType type, unsigned int slot);
        void deleteTexture(unsigned int& id);

        void createFramebuffer(unsigned int& id);
        void bindFramebuffer(unsigned int id);
        void unbindFramebuffer();
        void deleteFramebuffer(unsigned int& id);
        void framebufferAttach(unsigned int id, TextureType textureType, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, unsigned int textureId);
        void setDrawBuffers(unsigned int id, const std::vector<FramebufferAttachmentType>& drawBuffers);
        void checkFramebufferCompleteness(unsigned int id);
        void clearColorAttachment(unsigned int id, unsigned int colorAttachmentIndex, PrimitiveDataType dataType, void* value);
        void clearDepthAttachment(unsigned int id);

        void readFramebuffer(unsigned int id, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, TextureDataFormat internalFormat, PrimitiveDataType dataType, unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* data);

        void drawIndexedTriangles(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int indexCount);
        void drawIndexedLines(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int indexCount);

    private:

        void bindVertexBuffer(unsigned int& id);
        void unbindVertexBuffer();

        void bindIndexBuffer(unsigned int& id);
        void unbindIndexBuffer();

        void bindVertexArray(unsigned int& id);
        void unbindVertexArray();

        void bindShader(unsigned int id);

        void bindUniformBuffer(unsigned int& id);
        void unbindUniformBuffer();

        GLenum convertPrimitiveDataType(PrimitiveDataType type);
        GLenum convertShaderType(ShaderType type);
        GLenum convertTextureDataFormat(TextureDataFormat format);
        GLenum convertTextureType(TextureType type);
        GLenum convertFramebufferAttachmentType(FramebufferAttachmentType type);

        unsigned int m_boundVertexBufferId = 0;
        unsigned int m_boundIndexBufferId = 0;
        unsigned int m_boundVertexArrayId = 0;
        unsigned int m_boundUniformBufferId = 0;
        std::array<unsigned int, 31> m_activeTextureIds = {};
        unsigned int m_boundShaderId = 0;
        unsigned int m_boundFramebufferId = 0;

    };

}