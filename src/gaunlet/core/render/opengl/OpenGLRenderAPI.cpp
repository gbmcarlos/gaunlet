#include "gaunlet/core/render/opengl/OpenGLRenderAPI.h"
#include "gaunlet/core/render/opengl/opengl_utils.h"

namespace gaunlet::Core {

    void OpenGLRenderApi::init() {

        // Set up OpenGL, with GLEW
        if (glewInit() != GLEW_OK) {
            std::cout << "GLEW initialization failed" << std::endl;
        }

        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }

        // Enable and configure basic depth testing
        glEnable(GL_DEPTH_TEST);
        setDepthFunction(DepthStencilFunction::Less);

        // Enable and configure basic stencil testing
        glEnable(GL_STENCIL_TEST);
        setStencilFunction(DepthStencilFunction::GreaterOrEqual, 0); // Draw whenever it's empty
        setStencilOperation(true, StencilOperation::Keep, StencilOperation::Keep, StencilOperation::Replace); // Write to the stencil buffer whenever we actually draw

        std::cout << "OpenGL Specs:" << std::endl;
        std::cout << "openGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    }

    void OpenGLRenderApi::clearColorBuffer(float red, float green, float blue, float alpha) {
        glCall(glClearColor(red, green, blue, alpha));
        glCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void OpenGLRenderApi::clearDepthStencilBuffer() {
        glCall(glClearStencil(0));
        glCall(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    }

    void OpenGLRenderApi::setDepthFunction(DepthStencilFunction function) {
        glCall(glDepthFunc(convertDepthStencilFunction(function)));
    }

    void OpenGLRenderApi::setStencilFunction(DepthStencilFunction function, unsigned int reference) {
        glCall(glStencilFunc(convertDepthStencilFunction(function), reference, 0xFF));
    }

    void OpenGLRenderApi::setStencilOperation(bool enabled, StencilOperation stencilFailOperation, StencilOperation stencilPassDepthFailOperation, StencilOperation depthStencilPassOperation) {

        glCall(glStencilMask(enabled ? 0xFF : 0x00));
        glCall(glStencilOp(
            convertStencilOperation(stencilFailOperation),
            convertStencilOperation(stencilPassDepthFailOperation),
            convertStencilOperation(depthStencilPassOperation)
        ));

    }

    void OpenGLRenderApi::getViewport(unsigned int& x0, unsigned int& y0, unsigned int& x1, unsigned int& y1) {

        GLint viewport[4];
        glCall(glGetIntegerv(GL_VIEWPORT, (GLint*) &viewport));

        x0 = viewport[0];
        y0 = viewport[1];
        x1 = viewport[2];
        y1 = viewport[3];

    }

    void OpenGLRenderApi::setViewport(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) {
        glCall(glViewport(x0, y0, x1, y1));
    }

    void OpenGLRenderApi::createVertexBuffer(unsigned int& id, unsigned int size) {
        glCall(glGenBuffers(1, &id));
        bindVertexBuffer((id));
        glCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    void OpenGLRenderApi::createVertexBuffer(unsigned int& id, const void *data, unsigned int size) {
        glCall(glGenBuffers(1, &id));
        bindVertexBuffer((id));
        glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }

    void OpenGLRenderApi::bindVertexBuffer(unsigned int& id) {
        if (m_boundVertexBufferId != id) {
            glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
            m_boundVertexBufferId = id;
        }
    }

    void OpenGLRenderApi::submitVertexBufferData(unsigned int id, const void *data, unsigned int size) {
        bindVertexBuffer(id);
        glCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    }

    void OpenGLRenderApi::unbindVertexBuffer() {
        glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        m_boundVertexBufferId = 0;
    }

    void OpenGLRenderApi::deleteVertexBuffer(unsigned int id) {
        unbindVertexBuffer();
        glCall(glDeleteBuffers(1, &id));
    }

    void OpenGLRenderApi::createIndexBuffer(unsigned int& id, unsigned int *data, unsigned int count) {
        glCall(glGenBuffers(1, &id));
        bindIndexBuffer(id);
        glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
    }

    void OpenGLRenderApi::bindIndexBuffer(unsigned int& id) {
        if (m_boundIndexBufferId != id) {
            glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
            m_boundIndexBufferId = id;
        }
    }

    void OpenGLRenderApi::unbindIndexBuffer() {
        glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        m_boundIndexBufferId = 0;
    }

    void OpenGLRenderApi::deleteIndexBuffer(unsigned int id) {
        unbindIndexBuffer();
        glCall(glDeleteBuffers(1, &id));
    }

    void OpenGLRenderApi::createVertexArray(unsigned int& id) {
        glCall(glGenVertexArrays(1, &id));
    }

    void OpenGLRenderApi::bindVertexArray(unsigned int& id) {
        if (m_boundVertexArrayId != id) {
            glCall(glBindVertexArray(id));
            m_boundVertexArrayId = id;
        }
    }

    void OpenGLRenderApi::unbindVertexArray() {
        glCall(glBindVertexArray(0));
        m_boundVertexArrayId = 0;
    }

    void OpenGLRenderApi::deleteVertexArray(unsigned int& id) {
        unbindVertexArray();
        glCall(glDeleteVertexArrays(1, &id));
    }


    unsigned int OpenGLRenderApi::sizeOfPrimitiveDataType(PrimitiveDataType type) {
        return sizeof(convertPrimitiveDataType(type));
    }

    void OpenGLRenderApi::addVertexArrayAttribute(unsigned int vertexArrayId, unsigned int index, int count, PrimitiveDataType type, bool normalized, int stride, int offset) {

        bindVertexArray(vertexArrayId);

        glCall(glEnableVertexAttribArray(index));
        glCall(glVertexAttribPointer(
            index,
            count,
            convertPrimitiveDataType(type),
            normalized ? GL_TRUE : GL_FALSE,
            stride,
            (const void*) offset
        ));
    }

    unsigned int OpenGLRenderApi::createShaderProgram() {
        return (unsigned int) glCreateProgram();
    }

    unsigned int OpenGLRenderApi::compileShader(ShaderType type, const std::string& source) {

        unsigned int id = glCreateShader(convertShaderType(type));
        const char* src = source.c_str();
        glCall(glShaderSource(id, 1, &src, nullptr));
        glCall(glCompileShader(id));

        int result;
        glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
        if (result == GL_FALSE) {
            int length;
            glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
            char* message = (char*)alloca(length * sizeof(char));
            glCall(glGetShaderInfoLog(id, length, &length, message));
            std::cout << "Failed to compile " << (type == ShaderType::Vertex ? "vertex" : "fragment") << " shader. Error:" << std::endl;
            std::cout << message << std::endl;
            glCall(glDeleteShader(id));
            return 0;
        }

        return (unsigned int) id;

    }

    void OpenGLRenderApi::deleteShader(unsigned int id) {
        glCall(glDeleteShader(id));
    }

    void OpenGLRenderApi::attachShader(unsigned int programId, unsigned int shaderId) {
        glCall(glAttachShader(programId, shaderId));
    }

    void OpenGLRenderApi::compileShaderProgram(unsigned int id) {
        glCall(glLinkProgram(id));
    }

    void OpenGLRenderApi::bindShader(unsigned int id) {
        if (m_boundShaderId != id) {
            glCall(glUseProgram(id));
            m_boundShaderId = id;
        }
    }

    unsigned int OpenGLRenderApi::getUniformLocation(unsigned int id, const std::string& name) {
        return (unsigned int) glGetUniformLocation(id, name.c_str());
    }

    void OpenGLRenderApi::setUniform1i(unsigned int id, int location, int value) {
        bindShader(id);
        glCall(glUniform1i(location, value));
    }

    void OpenGLRenderApi::setUniform1f(unsigned int id, int location, float value) {
        bindShader(id);
        glCall(glUniform1f(location, value));
    }

    void OpenGLRenderApi::setUniform2f(unsigned int id, int location, const glm::vec2 &value) {
        bindShader(id);
        glCall(glUniform2f(location, value.x, value.y));
    }

    void OpenGLRenderApi::setUniform3f(unsigned int id, int location, const glm::vec3 &value) {
        bindShader(id);
        glCall(glUniform3f(location, value.x, value.y, value.z));
    }

    void OpenGLRenderApi::setUniform4f(unsigned int id, int location, const glm::vec4 &value) {
        bindShader(id);
        glCall(glUniform4f(location, value.x, value.y, value.z, value.w));
    }

    void OpenGLRenderApi::setUniformMat3f(unsigned int id, int location, const glm::mat3 &value) {
        bindShader(id);
        glCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void OpenGLRenderApi::setUniformMat4f(unsigned int id, int location, const glm::mat4 &value) {
        bindShader(id);
        glCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }


    void OpenGLRenderApi::createUniformBuffer(unsigned int& id, unsigned int size) {
        glCall(glGenBuffers(1, &id));
        bindUniformBuffer(id);
        glCall(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    void OpenGLRenderApi::createUniformBuffer(unsigned int& id, const void *data, unsigned int size) {
        glCall(glGenBuffers(1, &id));
        bindUniformBuffer(id);
        glCall(glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW));
    }

    void OpenGLRenderApi::bindUniformBuffer(unsigned int& id) {
        if (m_boundUniformBufferId != id) {
            glCall(glBindBuffer(GL_UNIFORM_BUFFER, id));
            m_boundUniformBufferId = id;
        }
    }

    void OpenGLRenderApi::submitUniformBufferData(unsigned int id, const void *data, unsigned int size) {
        bindUniformBuffer(id);
        glCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data));
    }

    void OpenGLRenderApi::unbindUniformBuffer() {
        glCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
        m_boundUniformBufferId = 0;
    }

    void OpenGLRenderApi::deleteUniformBuffer(unsigned int id) {
        unbindUniformBuffer();
        glCall(glDeleteBuffers(1, &id));
    }


    int OpenGLRenderApi::getUniformBufferBindingIndex(unsigned int id, const std::string &name) {
        return (int) glGetUniformBlockIndex(id, name.c_str());
    }

    void OpenGLRenderApi::bindUniformBufferToBindingPoint(unsigned int bufferId, unsigned int bindingPoint, unsigned int size) {
        glCall(glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, bufferId, 0, size));
    }

    void OpenGLRenderApi::bindUniformBufferFromBindingPoint(unsigned int shaderId, int location, unsigned int bindingPoint) {
        bindShader(shaderId);
        glCall(glUniformBlockBinding(shaderId, location, bindingPoint));
    }


    void OpenGLRenderApi::loadTextureImage2d(unsigned int& id, TextureInternalFormat internalFormat, TextureExternalFormat dataFormat, PrimitiveDataType dataType, unsigned int width, unsigned int height, void* data) {

        glCall(glGenTextures(1, &id));
        glCall(glBindTexture(GL_TEXTURE_2D, id));

        GLenum glInternalFormat = convertTextureInternalFormat(internalFormat);
        GLenum glDataFormat = convertTextureExternalFormat(dataFormat);
        GLenum glDataType = convertPrimitiveDataType(dataType);

        glCall(glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glDataFormat, glDataType, data));

    }

    void OpenGLRenderApi::loadTextureCubeMap(unsigned int& id, TextureInternalFormat internalFormat, TextureExternalFormat dataFormat, unsigned int width, unsigned int height, std::vector<void *> imagesData) {

        glCall(glGenTextures(1, &id));
        glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, id));

        GLenum glInternalFormat = convertTextureInternalFormat(internalFormat);
        GLenum glDataFormat = convertTextureExternalFormat(dataFormat);

        for (unsigned int index = 0; index < imagesData.size(); index++) {
            void* imageData = imagesData[index];
            glCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, glInternalFormat, width, height, 0, glDataFormat, GL_UNSIGNED_BYTE, imageData));
        }

    }

    void OpenGLRenderApi::setTexturedFilteringParameter(unsigned int id, TextureType textureType, TextureFilteringParameter parameter, TextureFilteringParameterValue value) {

        glCall(glBindTexture(convertTextureType(textureType), id));

        glCall(glTexParameteri(
            convertTextureType(textureType),
            convertTextureFilteringParameter(parameter),
            convertTextureFilteringParameterValue(value)
        ));

    }

    void OpenGLRenderApi::setTexturedWrappingParameter(unsigned int id, TextureType textureType, TextureWrappingParameter parameter, TextureWrappingParameterValue value) {

        glCall(glBindTexture(convertTextureType(textureType), id));

        glCall(glTexParameteri(
            convertTextureType(textureType),
            convertTextureWrappingParameter(parameter),
            convertTextureWrappingParameterValue(value)
        ));

    }

    void OpenGLRenderApi::activateTexture(unsigned int id, TextureType type, unsigned int slot) {
        glCall(glActiveTexture(GL_TEXTURE0 + slot));
        glCall(glBindTexture(convertTextureType(type), id));
    }

    void OpenGLRenderApi::deleteTexture(unsigned int& id) {
        glCall(glDeleteTextures(1, &id));
    }

    void OpenGLRenderApi::createFramebuffer(unsigned int &id) {
        glCall(glGenFramebuffers(1, &id));
    }

    void OpenGLRenderApi::bindFramebuffer(unsigned int id) {
        if (m_boundFramebufferId != id) {
            glCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
            m_boundFramebufferId = id;
        }
    }

    void OpenGLRenderApi::unbindFramebuffer() {
        glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        m_boundFramebufferId = 0;
    }

    void OpenGLRenderApi::deleteFramebuffer(unsigned int& id) {
        unbindFramebuffer();
        glCall(glDeleteFramebuffers(1, &id));
    }

    void OpenGLRenderApi::framebufferAttach(unsigned int id, TextureType textureType, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, unsigned int textureId) {

        GLenum glTextureType = convertTextureType(textureType);
        GLenum glAttachmentType = convertFramebufferAttachmentType(attachmentType);

        if (attachmentType == FramebufferAttachmentType::Color) {
            glAttachmentType += attachmentIndex;
        }

        bindFramebuffer(id);
        glCall(glBindTexture(GL_TEXTURE_2D, textureId));

        glCall(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, glAttachmentType, glTextureType, textureId, 0));

    }

    void OpenGLRenderApi::setDrawBuffers(unsigned int id, const std::vector<int>& drawBuffers) {

        bindFramebuffer(id);

        std::vector<GLenum> glBufferAttachments = {};

        for (unsigned int i = 0; i < drawBuffers.size(); i++) {

            // Convert the attachment type (aka buffer mode)
            int drawBuffer = drawBuffers[i];

            if (drawBuffer >= 0) {
                glBufferAttachments.push_back(GL_COLOR_ATTACHMENT0 + drawBuffer);
            } else {
                glBufferAttachments.push_back(GL_NONE);
            }

        }

        glCall(glDrawBuffers(drawBuffers.size(), (const unsigned int*) &glBufferAttachments[0]));
    }

    void OpenGLRenderApi::checkFramebufferCompleteness(unsigned int id) {
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Framebuffer is not complete");
        }
    }

    void OpenGLRenderApi::clearColorAttachment(unsigned int id, unsigned int colorAttachmentIndex, PrimitiveDataType dataType, void* value) {

        bindFramebuffer(id);

        if (dataType == PrimitiveDataType::Float) {
            glCall(glClearBufferfv(GL_COLOR, (int) colorAttachmentIndex, static_cast<float*>(value)));
        } else if (dataType == PrimitiveDataType::Int) {
            glCall(glClearBufferiv(GL_COLOR, (int) colorAttachmentIndex, static_cast<int*>(value)));
        } else {
            throw std::runtime_error("Unsupported data type for clear buffer color");
        }

    }

    void OpenGLRenderApi::clearDepthStencilAttachment(unsigned int id, float depthValue, int stencilValue) {

        bindFramebuffer(id);
        glCall(glClearBufferfi(GL_DEPTH_STENCIL, 0, depthValue, stencilValue));

    }

    void OpenGLRenderApi::readFramebuffer(unsigned int id, FramebufferAttachmentType attachmentType, unsigned int attachmentIndex, TextureExternalFormat dataFormat, PrimitiveDataType dataType, unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* data) {

        bindFramebuffer(id);

        GLenum glBufferMode = convertFramebufferAttachmentType(attachmentType);
        if (attachmentType == FramebufferAttachmentType::Color) {
            glBufferMode += attachmentIndex;
        }

        glCall(glReadBuffer(glBufferMode));
        glCall(glReadPixels(
            x, y,
            width, height,
            convertTextureExternalFormat(dataFormat),
            convertPrimitiveDataType(dataType),
            data
        ));

    }

    void OpenGLRenderApi::copyColorAttachment(unsigned int id, unsigned int attachmentIndex, unsigned int attachmentPixelX, unsigned int attachmentPixelY, unsigned int texturePixelX, unsigned int texturePixelY, unsigned int width, unsigned int height, unsigned int textureId) {

        GLenum glBufferMode = convertFramebufferAttachmentType(FramebufferAttachmentType::Color) + attachmentIndex;

        glCall(glReadBuffer(glBufferMode));
        glCall(glBindTexture(GL_TEXTURE_2D, textureId));
        glCall(glCopyTexSubImage2D(
            GL_TEXTURE_2D, 0,
            texturePixelX, texturePixelY,
            attachmentPixelX, attachmentPixelY,
            width, height
        ));

    }

    void OpenGLRenderApi::setPolygonMode(PolygonMode mode, float polygonOffsetFactor, float polygonOffsetUnits) {

        glCall(glPolygonOffset(polygonOffsetFactor,polygonOffsetUnits));
        glCall(glPolygonMode(GL_FRONT_AND_BACK, convertPolygonMode(mode)));

    }

    void OpenGLRenderApi::drawIndexedTriangles(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int indexCount) {

        m_boundIndexBufferId = 0; // Make sure the index buffer gets bound

        bindVertexBuffer(vertexBufferId);
        bindIndexBuffer(indexBufferId);
        bindVertexArray(vertexArrayId);
        bindShader(shaderId);

        glCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));

    }

    void OpenGLRenderApi::drawIndexedQuads(unsigned int vertexBufferId, unsigned int indexBufferId, unsigned int vertexArrayId, unsigned int shaderId, unsigned int vertexCount) {

        m_boundIndexBufferId = 0; // Make sure the index buffer gets bound
        m_boundVertexArrayId = 0;

        bindVertexBuffer(vertexBufferId);
        bindIndexBuffer(indexBufferId);
        bindVertexArray(vertexArrayId);
        bindShader(shaderId);

        glPatchParameteri(GL_PATCH_VERTICES, 4);

        glCall(glDrawArrays(GL_PATCHES, 0, vertexCount));

    }


    GLenum OpenGLRenderApi::convertPrimitiveDataType(PrimitiveDataType type) {

        switch (type) {
            case PrimitiveDataType::Bool:       return GL_BOOL;
            case PrimitiveDataType::Int:        return GL_INT;
            case PrimitiveDataType::UInt:       return GL_UNSIGNED_INT;
            case PrimitiveDataType::Float:      return GL_FLOAT;
            case PrimitiveDataType::UByte:      return GL_UNSIGNED_BYTE;
            case PrimitiveDataType::UInt24_8:   return GL_UNSIGNED_INT_24_8;
        }

        throw std::runtime_error("Unknown layout element type");

    }

    GLenum OpenGLRenderApi::convertShaderType(ShaderType type) {

        switch (type) {
            case ShaderType::Vertex:                    return GL_VERTEX_SHADER;
            case ShaderType::TessellationControl:       return GL_TESS_CONTROL_SHADER;
            case ShaderType::TessellationEvaluation:    return GL_TESS_EVALUATION_SHADER;
            case ShaderType::Geometry:                  return GL_GEOMETRY_SHADER;
            case ShaderType::Fragment:                  return GL_FRAGMENT_SHADER;
        }

        throw std::runtime_error("Unknown shader type");

    }

    GLenum OpenGLRenderApi::convertDepthStencilFunction(DepthStencilFunction function) {

        switch (function) {
            case DepthStencilFunction::Less:            return GL_LESS;
            case DepthStencilFunction::LessOrEqual:     return GL_LEQUAL;
            case DepthStencilFunction::Greater:         return GL_GREATER;
            case DepthStencilFunction::GreaterOrEqual:  return GL_GEQUAL;
            case DepthStencilFunction::Equal:           return GL_EQUAL;
            case DepthStencilFunction::NotEqual:        return GL_NOTEQUAL;
            case DepthStencilFunction::Always:          return GL_ALWAYS;
        }

        throw std::runtime_error("Unknown depth function");

    }

    GLenum OpenGLRenderApi::convertStencilOperation(StencilOperation operation) {

        switch (operation) {
            case StencilOperation::Keep:        return GL_KEEP;
            case StencilOperation::Replace:     return GL_REPLACE;
        }

        throw std::runtime_error("Unknown depth function");

    }

    GLenum OpenGLRenderApi::convertTextureInternalFormat(TextureInternalFormat format) {

        switch (format) {
            // Unsigned Normalized Integers
                // Red
                case TextureInternalFormat::Red_8_UNI:              return GL_R8;
                case TextureInternalFormat::Red_16_UNI:             return GL_R16;
                // RG
                case TextureInternalFormat::RG_8_UNI:               return GL_RG8;
                case TextureInternalFormat::RG_16_UNI:              return GL_RG16;
                // RGB
                case TextureInternalFormat::RGB_4_UNI:              return GL_RGB4;
                case TextureInternalFormat::RGB_5_UNI:              return GL_RGB5;
                case TextureInternalFormat::RGB_8_UNI:              return GL_RGB8;
                case TextureInternalFormat::RGB_10_UNI:             return GL_RGB10;
                case TextureInternalFormat::RGB_12_UNI:             return GL_RGB12;
                case TextureInternalFormat::RGB_16_UNI:             return GL_RGB16;
                // RGBA
                case TextureInternalFormat::RGBA_2_UNI:             return GL_RGBA2;
                case TextureInternalFormat::RGBA_4_UNI:             return GL_RGBA4;
                case TextureInternalFormat::RGBA_8_UNI:             return GL_RGBA8;
                case TextureInternalFormat::RGBA_12_UNI:            return GL_RGBA12;
                case TextureInternalFormat::RGBA_16_UNI:            return GL_RGBA16;
            // Singed Normalized Integers
                // Red
                case TextureInternalFormat::Red_8_SNI:              return GL_R8_SNORM;
                case TextureInternalFormat::Red_16_SNI:             return GL_R16_SNORM;
                // RG
                case TextureInternalFormat::RG_8_SNI:               return GL_RG8_SNORM;
                case TextureInternalFormat::RG_16_SNI:              return GL_RG16_SNORM;
                // RGB
                case TextureInternalFormat::RGB_8_SNI:              return GL_RGB8_SNORM;
                case TextureInternalFormat::RGB_16_SNI:             return GL_RGB16_SNORM;
                // RGBA
                case TextureInternalFormat::RGBA_8_SNI:             return GL_RGBA8_SNORM;
                case TextureInternalFormat::RGBA_16_SNI:            return GL_RGBA16_SNORM;
            // Unsigned Integral
                // Red
                case TextureInternalFormat::Red_8_UI:               return GL_R8UI;
                case TextureInternalFormat::Red_16_UI:              return GL_R16UI;
                case TextureInternalFormat::Red_32_UI:              return GL_R32UI;
                // RG
                case TextureInternalFormat::RG_8_UI:                return GL_RG8UI;
                case TextureInternalFormat::RG_16_UI:               return GL_RG16UI;
                case TextureInternalFormat::RG_32_UI:               return GL_RG32UI;
                // RGB
                case TextureInternalFormat::RGB_8_UI:               return GL_RGB8UI;
                case TextureInternalFormat::RGB_16_UI:              return GL_RGB16UI;
                case TextureInternalFormat::RGB_32_UI:              return GL_RGB32UI;
                // RGBA
                case TextureInternalFormat::RGBA_8_UI:              return GL_RGBA8UI;
                case TextureInternalFormat::RGBA_16_UI:             return GL_RGBA16UI;
                case TextureInternalFormat::RGBA_32_UI:             return GL_RGBA32UI;
            // Signed Integral
                // Red
                case TextureInternalFormat::Red_8_SI:               return GL_R8I;
                case TextureInternalFormat::Red_16_SI:              return GL_R16I;
                case TextureInternalFormat::Red_32_SI:              return GL_R32I;
                // RG
                case TextureInternalFormat::RG_8_SI:                return GL_RG8I;
                case TextureInternalFormat::RG_16_SI:               return GL_RG16I;
                case TextureInternalFormat::RG_32_SI:               return GL_RG32I;
                // RGB
                case TextureInternalFormat::RGB_8_SI:               return GL_RGB8I;
                case TextureInternalFormat::RGB_16_SI:              return GL_RGB16I;
                case TextureInternalFormat::RGB_32_SI:              return GL_RGB32I;
                // RGBA
                case TextureInternalFormat::RGBA_8_SI:              return GL_RGBA8I;
                case TextureInternalFormat::RGBA_16_SI:             return GL_RGBA16I;
                case TextureInternalFormat::RGBA_32_SI:             return GL_RGBA32I;
            // Float
                // Red
                case TextureInternalFormat::Red_16_F:               return GL_R16F;
                case TextureInternalFormat::Red_32_F:               return GL_R32F;
                // RG
                case TextureInternalFormat::RG_16_F:                return GL_RG16F;
                case TextureInternalFormat::RG_32_F:                return GL_RG32F;
                // RGB
                case TextureInternalFormat::RGB_16_F:               return GL_RGB16F;
                case TextureInternalFormat::RGB_32_F:               return GL_RGB32F;
                // RGBA
                case TextureInternalFormat::RGBA_16_F:              return GL_RGBA16F;
                case TextureInternalFormat::RGBA_32_F:              return GL_RGBA32F;
            // Depth & Stencil
                case TextureInternalFormat::Depth_24_Stencil_8:     return GL_DEPTH24_STENCIL8;
        }

        throw std::runtime_error("Unknown texture format");

    }

    GLenum OpenGLRenderApi::convertTextureExternalFormat(TextureExternalFormat format) {

        switch (format) {
            case TextureExternalFormat::Red:                return GL_RED;
            case TextureExternalFormat::RG:                 return GL_RG;
            case TextureExternalFormat::RGB:                return GL_RGB;
            case TextureExternalFormat::RGBA:               return GL_RGBA;
            case TextureExternalFormat::RedInteger:         return GL_RED_INTEGER;
            case TextureExternalFormat::RGInteger:          return GL_RG_INTEGER;
            case TextureExternalFormat::RGBInteger:         return GL_RGB_INTEGER;
            case TextureExternalFormat::RGBAInteger:        return GL_RGBA_INTEGER;
            case TextureExternalFormat::DepthStencil:       return GL_DEPTH_STENCIL;
        }

    }

    GLenum OpenGLRenderApi::convertTextureType(TextureType type) {

        switch (type) {
            case TextureType::Image2D:  return GL_TEXTURE_2D;
            case TextureType::CubeMap:  return GL_TEXTURE_CUBE_MAP;
        }

        throw std::runtime_error("Unknown texture format");

    }

    GLenum OpenGLRenderApi::convertTextureFilteringParameter(TextureFilteringParameter parameter) {

        switch (parameter) {
            case TextureFilteringParameter::Minifying:     return GL_TEXTURE_MIN_FILTER;
            case TextureFilteringParameter::Magnifying:    return GL_TEXTURE_MAG_FILTER;
        }

        throw std::runtime_error("Unknown texture filtering parameter");

    }

    GLenum OpenGLRenderApi::convertTextureFilteringParameterValue(TextureFilteringParameterValue value) {

        switch (value) {
            case TextureFilteringParameterValue::Linear:    return GL_LINEAR;
            case TextureFilteringParameterValue::Nearest:   return GL_NEAREST;
        }

        throw std::runtime_error("Unknown texture filtering parameter value");

    }

    GLenum OpenGLRenderApi::convertTextureWrappingParameter(TextureWrappingParameter parameter) {

        switch (parameter) {
            case TextureWrappingParameter::S:   return GL_TEXTURE_WRAP_S;
            case TextureWrappingParameter::T:   return GL_TEXTURE_WRAP_T;
            case TextureWrappingParameter::R:   return GL_TEXTURE_WRAP_R;
        }

        throw std::runtime_error("Unknown texture wrapping parameter");

    }

    GLenum OpenGLRenderApi::convertTextureWrappingParameterValue(TextureWrappingParameterValue value) {

        switch (value) {
            case TextureWrappingParameterValue::Repeat:             return GL_REPEAT;
            case TextureWrappingParameterValue::MirroredRepeat:     return GL_MIRRORED_REPEAT;
            case TextureWrappingParameterValue::ClampToEdge:        return GL_CLAMP_TO_EDGE;
        }

    }

    GLenum OpenGLRenderApi::convertFramebufferAttachmentType(FramebufferAttachmentType type) {

        switch (type) {
            case FramebufferAttachmentType::None:           return GL_NONE;
            case FramebufferAttachmentType::Color:          return GL_COLOR_ATTACHMENT0;
            case FramebufferAttachmentType::DepthStencil:   return GL_DEPTH_STENCIL_ATTACHMENT;
        }

        throw std::runtime_error("Unknown texture format");

    }

    GLenum OpenGLRenderApi::convertPolygonMode(PolygonMode mode) {

        switch (mode) {
            case PolygonMode::Fill:     return GL_FILL;
            case PolygonMode::Line:     return GL_LINE;
        }

        throw std::runtime_error("Unknown polygon mode");

    }

}