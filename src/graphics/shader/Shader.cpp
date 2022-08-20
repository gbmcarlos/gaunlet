#include "Shader.h"

#include "shader_utils.h"
#include "../../core/render/RenderCommand.h"

#include "../pch.h"

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

    void Shader::setUniform1i(const std::string& name, int value) {

        int location = getUniformLocation(name);
        RenderCommand::setUniform1i(m_rendererId, location, value);

    }

    void Shader::setUniform3f(const std::string& name, const glm::vec3& value) {

        int location = getUniformLocation(name);
        RenderCommand::setUniform3f(m_rendererId, location, value);

    }

    void Shader::setUniform4f(const std::string& name, const glm::vec4& value) {

        int location = getUniformLocation(name);
        RenderCommand::setUniform4f(m_rendererId, location, value);

    }

    void Shader::setUniformMat3f(const std::string& name, const glm::mat3& value) {

        int location = getUniformLocation(name);
        RenderCommand::setUniformMat3f(m_rendererId, location, value);

    }

    void Shader::setUniformMat4f(const std::string& name, const glm::mat4& value) {

        int location = getUniformLocation(name);
        RenderCommand::setUniformMat4f(m_rendererId, location, value);

    }

    void Shader::linkUniformBuffer(const std::shared_ptr<UniformBuffer>& uniformBuffer) {

        int location = getUniformBlockLocation(uniformBuffer->getName());
        RenderCommand::bindUniformBufferFromBindingPoint(
            m_rendererId,
            location,
            uniformBuffer->getBindingPoint()
        );

    }

    int Shader::getUniformLocation(const std::string& name) {

        if (m_uniformLocations.find(name) != m_uniformLocations.end()) {
            return m_uniformLocations[name];
        }

        int location = RenderCommand::getUniformLocation(m_rendererId, name);
        m_uniformLocations[name] = location;
        return location;

    }

    int Shader::getUniformBlockLocation(const std::string& name) {

        if (m_uniformBlockLocations.find(name) != m_uniformBlockLocations.end()) {
            return m_uniformBlockLocations[name];
        }

        int location = (int) RenderCommand::getUniformBufferBindingIndex(m_rendererId, name);
        m_uniformBlockLocations[name] = location;
        return location;

    }

}