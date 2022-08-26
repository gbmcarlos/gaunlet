#include "gaunlet/graphics/shader/Shader.h"

#include "gaunlet/graphics/shader/shader_utils.h"
#include "gaunlet/core/render/RenderCommand.h"

namespace gaunlet::Graphics {

    Shader::Shader()
        : m_rendererId(0) {}

    Shader::~Shader() {
        if (m_rendererId) {
            Core::RenderCommand::deleteShader(m_rendererId);
        }
    }

    void Shader::attach(Core::ShaderType type, const std::string& shaderPath) {

        if (!m_rendererId) {
            m_rendererId = Core::RenderCommand::createShaderProgram();
        }

        std::string shaderSource = readFromFile(shaderPath);
        unsigned int shaderId = Core::RenderCommand::compileShader(type, shaderSource);
        Core::RenderCommand::attachShader(m_rendererId, shaderId);
        Core::RenderCommand::deleteShader(shaderId);

    }

    void Shader::compile() const {
        Core::RenderCommand::compileShaderProgram(m_rendererId);
    }

    void Shader::setUniform1i(const std::string& name, int value) {

        int location = getUniformLocation(name);
        Core::RenderCommand::setUniform1i(m_rendererId, location, value);

    }

    void Shader::setUniform3f(const std::string& name, const glm::vec3& value) {

        int location = getUniformLocation(name);
        Core::RenderCommand::setUniform3f(m_rendererId, location, value);

    }

    void Shader::setUniform4f(const std::string& name, const glm::vec4& value) {

        int location = getUniformLocation(name);
        Core::RenderCommand::setUniform4f(m_rendererId, location, value);

    }

    void Shader::setUniformMat3f(const std::string& name, const glm::mat3& value) {

        int location = getUniformLocation(name);
        Core::RenderCommand::setUniformMat3f(m_rendererId, location, value);

    }

    void Shader::setUniformMat4f(const std::string& name, const glm::mat4& value) {

        int location = getUniformLocation(name);
        Core::RenderCommand::setUniformMat4f(m_rendererId, location, value);

    }

    void Shader::linkUniformBuffer(const Core::Ref<UniformBuffer>& uniformBuffer) {

        int location = getUniformBlockLocation(uniformBuffer->getName());
        Core::RenderCommand::bindUniformBufferFromBindingPoint(
            m_rendererId,
            location,
            uniformBuffer->getBindingPoint()
        );

    }

    int Shader::getUniformLocation(const std::string& name) {

        if (m_uniformLocations.find(name) != m_uniformLocations.end()) {
            return m_uniformLocations[name];
        }

        int location = Core::RenderCommand::getUniformLocation(m_rendererId, name);
        m_uniformLocations[name] = location;
        return location;

    }

    int Shader::getUniformBlockLocation(const std::string& name) {

        if (m_uniformBlockLocations.find(name) != m_uniformBlockLocations.end()) {
            return m_uniformBlockLocations[name];
        }

        int location = (int) Core::RenderCommand::getUniformBufferBindingIndex(m_rendererId, name);
        m_uniformBlockLocations[name] = location;
        return location;

    }

}