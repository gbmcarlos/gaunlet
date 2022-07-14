#include <GL/glew.h>

#include <string>
#include <glm//glm.hpp>

#include "shader_utils.h"

#include "Shader.h"

namespace engine {

    Shader::Shader()
        : rendererId(0) {}

    Shader::~Shader() {
        if (rendererId) {
            glDeleteProgram(rendererId);
        }
    }

    void Shader::attach(unsigned int type, std::string shaderPath) {

        if (!rendererId) {
            rendererId = glCreateProgram();
        }

        std::string shaderSource = readFromFile(shaderPath);
        unsigned int shader = compileShaderProgram(type, shaderSource);
        glAttachShader(rendererId, shader);
        glDeleteShader(shader);

    }

    void Shader::compile() const {

        glLinkProgram(rendererId);
        glValidateProgram(rendererId);

    }

    void Shader::bind() const {
        if (!rendererId) {
            throw std::runtime_error("Shader must be compiled before binding it");
        }
        glUseProgram(rendererId);
    }

    void Shader::setUniform3f(const std::string name, glm::vec3 value) {

        GLint location = getUniformLocation(name);
        glUniform3f(location, value.x, value.y, value.z);

    }

    void Shader::setUniform4f(const std::string name, glm::vec4 value) {

        bind();
        GLint location = getUniformLocation(name);
        glUniform4f(location, value.x, value.y, value.z, value.w);

    }

    void Shader::setUniformMat4f(const std::string name, glm::mat4 value) {

        GLint location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);

    }

    GLint Shader::getUniformLocation(const std::string name) {

        if (uniformLocations.find(name) != uniformLocations.end()) {
            return uniformLocations[name];
        }

        GLint location = glGetUniformLocation(rendererId, name.c_str());
        uniformLocations[name] = location;
        return location;

    }

}