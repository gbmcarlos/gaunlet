#pragma once

#include "gaunlet/core/render/RenderApi.h"

#include "gaunlet/pch.h"

namespace gaunlet::Core {

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

        static void clearColorBuffer(const glm::vec4& color);
        static void clearDepthStencilBuffer();

        static void setDepthFunction(DepthStencilFunction function);
        static void setStencilFunction(DepthStencilFunction function, unsigned int reference);
        static void setStencilOperation(bool enabled, StencilOperation stencilFailOperation, StencilOperation stencilPassDepthFailOperation, StencilOperation depthStencilPassOperation);

        static void getViewport(unsigned int& x0, unsigned int& y0, unsigned int& x1, unsigned int& y1);
        static void setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);

        static void createVertexBuffer(unsigned int& id, unsigned int size);
        static void createVertexBuffer(unsigned int& id, const void *data, unsigned int size);
        static void submitVertexBufferData(unsigned int id, const void *data, unsigned int size);
        static void deleteVertexBuffer(unsigned int id);

        static void createIndexBuffer(unsigned int& id, unsigned int* data, unsigned int count);
        static void deleteIndexBuffer(unsigned int id);

        static void createVertexArray(unsigned int& id);
        static void deleteVertexArray(unsigned int& id);

        static unsigned int sizeOfLayoutElementType(PrimitiveDataType type);
        static void addVertexArrayAttribute(unsigned int id, unsigned int index, int count, PrimitiveDataType type, bool normalized, int stride, int offset);

        static unsigned int createShaderProgram();
        static unsigned int compileShader(ShaderType type, const std::string& source);
        static void attachShader(unsigned programId, unsigned int shaderId);
        static void compileShaderProgram(unsigned int id);
        static void deleteShader(unsigned int id);

        static int getUniformLocation(unsigned int id, const std::string& name);
        static void setUniform1i(unsigned int id, int location, int value);
        static void setUniform1f(unsigned int id, int location, float value);
        static void setUniform2f(unsigned int id, int location, const glm::vec2& value);
        static void setUniform3f(unsigned int id, int location, const glm::vec3& value);
        static void setUniform4f(unsigned int id, int location, const glm::vec4& value);
        static void setUniformMat3f(unsigned int id, int location, const glm::mat3& value);
        static void setUniformMat4f(unsigned int id, int location, const glm::mat4& value);

        static void createUniformBuffer(unsigned int& id, unsigned int size);
        static void createUniformBuffer(unsigned int& id, const void *data, unsigned int size);
        static void submitUniformBufferData(unsigned int id, const void *data, unsigned int size);
        static void deleteUniformBuffer(unsigned int id);

        static int getUniformBufferBindingIndex(unsigned int id, const std::string& name);
        static void bindUniformBufferToBindingPoint(unsigned int bufferId, unsigned int bindingPoint, unsigned int size);
        static void bindUniformBufferFromBindingPoint(unsigned int shaderId, int location, unsigned int bindingPoint);

        static void loadTextureImage2d(unsigned int& id, TextureInternalFormat internalFormat, TextureExternalFormat dataFormat, PrimitiveDataType dataType, unsigned int width, unsigned int height, void* data);
        static void loadTextureCubeMap(unsigned int& id, TextureInternalFormat internalFormat, TextureExternalFormat dataFormat, unsigned int width, unsigned int height, std::vector<void *> imagesData);
        static void setTexturedFilteringParameter(unsigned int id, TextureType textureType, TextureFilteringParameter parameter, TextureFilteringParameterValue value);
        static void setTexturedWrappingParameter(unsigned int id, TextureType textureType, TextureWrappingParameter parameter, TextureWrappingParameterValue value);
        static void activateTexture(unsigned int id, TextureType type, unsigned int slot);
        static void deleteTexture(unsigned int& id);

        static void createFramebuffer(unsigned int& id);
        static void bindFramebuffer(unsigned int id);
        static void unbindFramebuffer();
        static void deleteFramebuffer(unsigned int& id);
        static void framebufferAttach(unsigned int id, TextureType textureType, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, unsigned int textureId);
        static void setDrawBuffers(unsigned int id, const std::vector<int>& drawBuffers);
        static void checkFramebufferCompleteness(unsigned int id);
        static void clearColorAttachment(unsigned int id, unsigned int colorAttachmentIndex, PrimitiveDataType dataType, void* value);
        static void clearDepthStencilAttachment(unsigned int id, float depthValue, int stencilValue);

        static void readFramebuffer(unsigned int id, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, TextureExternalFormat dataFormat, PrimitiveDataType type, unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* data);
        static void copyColorAttachment(unsigned int id, unsigned int attachmentIndex, unsigned int attachmentPixelX, unsigned int attachmentPixelY, unsigned int texturePixelX, unsigned int texturePixelY, unsigned int width, unsigned int height, unsigned int textureId);

        static void setPolygonMode(PolygonMode mode, float polygonOffsetFactor = 0.0f, float polygonOffsetUnits = 0.0f);
        static void drawIndexedTriangles(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int indexCount);
        static void drawIndexedQuads(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int vertexCount);

    };

}