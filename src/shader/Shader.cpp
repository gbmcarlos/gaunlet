#include <GL/glew.h>

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_utils.h"

#include "Shader.h"

namespace engine {

    Shader::Shader()
        : m_rendererId(0) {}

    Shader::~Shader() {
        if (m_rendererId) {
            glDeleteProgram(m_rendererId);
        }
    }

    void Shader::attach(unsigned int type, const std::string& shaderPath) {

        if (!m_rendererId) {
            m_rendererId = glCreateProgram();
        }

        std::string shaderSource = readFromFile(shaderPath);
        unsigned int shader = compileShaderProgram(type, shaderSource);
        glAttachShader(m_rendererId, shader);
        glDeleteShader(shader);

    }

    void Shader::compile() const {

        glLinkProgram(m_rendererId);
        glValidateProgram(m_rendererId);

    }

    void Shader::bind() const {
        if (!m_rendererId) {
            throw std::runtime_error("Shader must be compiled before binding it");
        }
        glUseProgram(m_rendererId);
    }

    void Shader::setUniform3f(const std::string& name, const glm::vec3& value) {

        GLint location = getUniformLocation(name);
        glUniform3f(location, value.x, value.y, value.z);

    }

    void Shader::setUniform4f(const std::string& name, const glm::vec4& value) {

        bind();
        GLint location = getUniformLocation(name);
        glUniform4f(location, value.x, value.y, value.z, value.w);

    }

    void Shader::setUniformMat4f(const std::string& name, const glm::mat4& value) {

        bind();
        GLint location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

    }

    GLint Shader::getUniformLocation(const std::string& name) {

        if (m_uniformLocations.find(name) != m_uniformLocations.end()) {
            return m_uniformLocations[name];
        }

        GLint location = glGetUniformLocation(m_rendererId, name.c_str());
        m_uniformLocations[name] = location;
        return location;

    }

}