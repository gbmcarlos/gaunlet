#include "Shader.h"

#include "shader_utils.h"
#include "../../core/render/RenderCommand.h"

#include <string>
#include <glm/glm.hpp>

namespace engine {

    Shader::Shader()
        : m_rendererId(0) {}

    Shader::~Shader() {
        if (m_rendererId) {
            RenderCommand::deleteShader(m_rendererId);
        }
    }

    void Shader::attach(ShaderType type, const std::string& shaderPath) {

        if (!m_rendererId) {
            m_rendererId = RenderCommand::createShaderProgram();
        }

        std::string shaderSource = readFromFile(shaderPath);
        unsigned int shaderId = RenderCommand::compileShader(type, shaderSource);
        RenderCommand::attachShader(m_rendererId, shaderId);
        RenderCommand::deleteShader(shaderId);

    }

    void Shader::compile() const {
        RenderCommand::compileShaderProgram(m_rendererId);
    }

    void Shader::bind() const {
        if (!m_rendererId) {
            throw std::runtime_error("Shader must be compiled before binding it");
        }
        RenderCommand::bindShader(m_rendererId);
    }

    void Shader::setUniform1i(const std::string& name, int value) {

        bind();
        int location = getUniformLocation(name);
        RenderCommand::setUniform1i(location, value);

    }

    void Shader::setUniform3f(const std::string& name, const glm::vec3& value) {

        bind();
        int location = getUniformLocation(name);
        RenderCommand::setUniform3f(location, value);

    }

    void Shader::setUniform4f(const std::string& name, const glm::vec4& value) {

        bind();
        int location = getUniformLocation(name);
        RenderCommand::setUniform4f(location, value);

    }

    void Shader::setUniformMat3f(const std::string& name, const glm::mat3& value) {

        bind();
        int location = getUniformLocation(name);
        RenderCommand::setUniformMat3f(location, value);

    }

    void Shader::setUniformMat4f(const std::string& name, const glm::mat4& value) {

        bind();
        int location = getUniformLocation(name);
        RenderCommand::setUniformMat4f(location, value);

    }

    int Shader::getUniformLocation(const std::string& name) {

        if (m_uniformLocations.find(name) != m_uniformLocations.end()) {
            return m_uniformLocations[name];
        }

        int location = RenderCommand::getUniformLocation(m_rendererId, name);
        m_uniformLocations[name] = location;
        return location;

    }

}