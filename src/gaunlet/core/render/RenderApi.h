#pragma once

#include "gaunlet/pch.h"

namespace gaunlet::Core {

    enum class PrimitiveDataType {
        Bool,
        Int,
        UInt,
        Float,
        UByte,
        UInt24_8
    };

    enum class ShaderType {
        Vertex,
        TessellationControl,
        TessellationEvaluation,
        Geometry,
        Fragment
    };

    enum class PolygonMode {
        Fill, Line
    };

    enum class DepthStencilFunction {
        Less, LessOrEqual,
        Greater, GreaterOrEqual,
        Equal, NotEqual, Always
    };

    enum class StencilOperation {
        Keep, Replace
    };

    enum class TextureInternalFormat {
        // Unsigned Normalized Integers
            // Red
            Red_8_UNI, Red_16_UNI,
            // RG
            RG_8_UNI, RG_16_UNI,
            // RGB
            RGB_4_UNI, RGB_5_UNI, RGB_8_UNI, RGB_10_UNI, RGB_12_UNI, RGB_16_UNI,
            // RGBA
            RGBA_2_UNI, RGBA_4_UNI, RGBA_8_UNI, RGBA_12_UNI, RGBA_16_UNI,
        // Singed Normalized Integers
            // Red
            Red_8_SNI, Red_16_SNI,
            // RG
            RG_8_SNI, RG_16_SNI,
            // RGB
            RGB_8_SNI, RGB_16_SNI,
            // RGBA
            RGBA_8_SNI, RGBA_16_SNI,
        // Unsigned Integral
            // Red
            Red_8_UI, Red_16_UI, Red_32_UI,
            // RG
            RG_8_UI, RG_16_UI, RG_32_UI,
            // RGB
            RGB_8_UI, RGB_16_UI, RGB_32_UI,
            // RGBA
            RGBA_8_UI, RGBA_16_UI, RGBA_32_UI,
        // Signed Integral
            // Red
            Red_8_SI, Red_16_SI, Red_32_SI,
            // RG
            RG_8_SI, RG_16_SI, RG_32_SI,
            // RGB
            RGB_8_SI, RGB_16_SI, RGB_32_SI,
            // RGBA
            RGBA_8_SI, RGBA_16_SI, RGBA_32_SI,
        // Float
            // Red
            Red_16_F, Red_32_F,
            // RG
            RG_16_F, RG_32_F,
            // RGB
            RGB_16_F, RGB_32_F,
            // RGBA
            RGBA_16_F, RGBA_32_F,
        // Depth & Stencil
            Depth_24_Stencil_8
    };

    enum class TextureExternalFormat {
        Red, RG, RGB, RGBA,
        RedInteger, RGInteger, RGBInteger, RGBAInteger,
        DepthStencil
    };

    enum class TextureType {
        Image2D,
        CubeMap
    };

    enum class FramebufferAttachmentType {
        None,
        Color,
        DepthStencil
    };

    class RenderApi {

    public:

        virtual void init() = 0;

        virtual void clearColorBuffer(float red, float green, float blue, float alpha) = 0;
        virtual void clearDepthStencilBuffer() = 0;

        virtual void setDepthFunction(DepthStencilFunction depthFunction) = 0;
        virtual void setStencilFunction(DepthStencilFunction stencilFunction, unsigned int reference) = 0;
        virtual void setStencilOperation(bool enabled, StencilOperation stencilFailOperation, StencilOperation stencilPassDepthFailOperation, StencilOperation depthStencilPassOperation) = 0;

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
        virtual void setUniform1f(unsigned int id, int location, float value) = 0;
        virtual void setUniform2f(unsigned int id, int location, const glm::vec2& value) = 0;
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

        virtual void loadTextureImage2d(unsigned int& id, TextureInternalFormat internalFormat, TextureExternalFormat dataFormat, PrimitiveDataType dataType, unsigned int width, unsigned int height, void* data) = 0;
        virtual void loadTextureCubeMap(unsigned int& id, TextureInternalFormat internalFormat, TextureExternalFormat dataFormat, unsigned int width, unsigned int height, std::vector<void *> imagesData) = 0;
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
        virtual void clearDepthStencilAttachment(unsigned int id, float depthValue, int stencilValue) = 0;

        virtual void readFramebuffer(unsigned int id, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, TextureExternalFormat dataFormat, PrimitiveDataType dataType, unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* data) = 0;
        virtual void copyColorAttachment(unsigned int id, unsigned int attachmentIndex, unsigned int attachmentPixelX, unsigned int attachmentPixelY, unsigned int texturePixelX, unsigned int texturePixelY, unsigned int width, unsigned int height, unsigned int textureId) = 0;

        virtual void setPolygonMode(PolygonMode mode, float polygonOffsetFactor, float polygonOffsetUnits) = 0;
        virtual void drawIndexedTriangles(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int indexCount) = 0;
        virtual void drawIndexedQuads(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int vertexCount) = 0;

    };

}