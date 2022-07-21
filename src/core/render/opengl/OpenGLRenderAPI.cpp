#include "OpenGLRenderAPI.h"
#include "opengl_utils.h"

#include <glm/gtc/type_ptr.hpp>

namespace engine {

    void OpenGLRenderApi::init() {

        // Set up OpenGL, with GLEW
        if (glewInit() != GLEW_OK) {
            std::cout << "GLEW initialization failed" << std::endl;
        }

        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }

    }

    GLenum OpenGLRenderApi::convertVertexBufferLayoutElementType(VertexBufferLayoutElementType type) {

        switch (type) {
            case VertexBufferLayoutElementType::Bool:       return GL_BOOL;
            case VertexBufferLayoutElementType::Int:        return GL_INT;
            case VertexBufferLayoutElementType::Float:      return GL_FLOAT;
        }

        throw std::runtime_error("Unknown layout element type");

    }

    GLenum OpenGLRenderApi::convertShaderType(ShaderType type) {

        switch (type) {
            case ShaderType::Vertex:    return GL_VERTEX_SHADER;
            case ShaderType::Fragment:    return GL_FRAGMENT_SHADER;
        }

        throw std::runtime_error("Unknown shader type");

    }

    void OpenGLRenderApi::setClearColor(float red, float green, float blue, float alpha) {
        glClearColor(red, green, blue, alpha);
    }

    void OpenGLRenderApi::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderApi::createVertexBuffer(unsigned int& id, unsigned int size) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    void OpenGLRenderApi::createVertexBuffer(unsigned int& id, const void *data, unsigned int size) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void OpenGLRenderApi::bindVertexBuffer(unsigned int& id) {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }


    void OpenGLRenderApi::submitVertexBufferData(const void *data, unsigned int size) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    void OpenGLRenderApi::unbindVertexBuffer() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLRenderApi::createIndexBuffer(unsigned int& id, unsigned int *data, unsigned int count) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }

    void OpenGLRenderApi::bindIndexBuffer(unsigned int& id) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void OpenGLRenderApi::unbindIndexBuffer() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OpenGLRenderApi::deleteBuffer(unsigned int& id) {
        glDeleteBuffers(1, &id);
    }

    void OpenGLRenderApi::createVertexArray(unsigned int& id) {
        glGenVertexArrays(1, &id);
    }

    void OpenGLRenderApi::bindVertexArray(unsigned int& id) {
        glBindVertexArray(id);
    }

    void OpenGLRenderApi::unbindVertexArray() {
        glBindVertexArray(0);
    }

    void OpenGLRenderApi::deleteVertexArray(unsigned int& id) {
        glDeleteVertexArrays(1, &id);
    }

    unsigned int OpenGLRenderApi::createShaderProgram() {
        return (unsigned int) glCreateProgram();
    }

    unsigned int OpenGLRenderApi::compileShader(ShaderType type, const std::string& source) {

        unsigned int id = glCreateShader(convertShaderType(type));
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile " << (type == ShaderType::Vertex ? "vertex" : "fragment") << " m_shader. Error:" << std::endl;
            std::cout << message << std::endl;
            glDeleteShader(id);
            return 0;
        }

        return (unsigned int) id;

    }

    void OpenGLRenderApi::deleteShader(unsigned int id) {
        glDeleteShader(id);
    }

    void OpenGLRenderApi::attachShader(unsigned int id, unsigned int shaderId) {
        glAttachShader(id, shaderId);
    }

    void OpenGLRenderApi::compileShaderProgram(unsigned int id) {
        glLinkProgram(id);
        glValidateProgram(id);
    }

    void OpenGLRenderApi::bindShader(unsigned int id) {
        glUseProgram(id);
    }

    unsigned int OpenGLRenderApi::getUniformLocation(unsigned int id, const std::string& name) {
        return (unsigned int) glGetUniformLocation(id, name.c_str());
    }

    void OpenGLRenderApi::setUniform3f(int location, const glm::vec3 &value) {
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLRenderApi::setUniform4f(int location, const glm::vec4 &value) {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLRenderApi::setUniformMat3f(int location, const glm::mat3 &value) {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLRenderApi::setUniformMat4f(int location, const glm::mat4 &value) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    unsigned int OpenGLRenderApi::sizeOfVertexBufferLayoutElementType(VertexBufferLayoutElementType type) {
        return sizeof(convertVertexBufferLayoutElementType(type));
    }

    void OpenGLRenderApi::addVertexArrayAttribute(unsigned int index, int count, VertexBufferLayoutElementType type, bool normalized, int stride, int offset) {

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            count,
            convertVertexBufferLayoutElementType(type),
                normalized ? GL_TRUE : GL_FALSE,
            stride,
            (const void*) offset
        );
    }

    void OpenGLRenderApi::drawIndexedTriangles(unsigned int indexCount) {
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRenderApi::drawIndexedLines(unsigned int indexCount) {
        glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, nullptr);
    }

}