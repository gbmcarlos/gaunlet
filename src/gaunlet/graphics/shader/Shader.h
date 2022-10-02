#pragma once

#include "gaunlet/core/render/RenderApi.h"
#include "gaunlet/graphics/uniform-buffer/UniformBuffer.h"

#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

    class Shader {

    private:
        unsigned int m_rendererId;
        int getUniformLocation(const std::string& name);
        int getUniformBlockLocation(const std::string& name);
        std::unordered_map<std::string, int> m_uniformLocations;
        std::unordered_map<std::string, int> m_uniformBlockLocations;
    public:
        Shader();
        ~Shader();

        inline unsigned int getRendererId() const {return m_rendererId; }

        void attach(Core::ShaderType type, const std::string& shaderPath);
        void compile() const;

        Shader* setUniform1i(const std::string& name, int value);
        Shader* setUniform1f(const std::string& name, float value);
        Shader* setUniform2f(const std::string& name, const glm::vec2& value);
        Shader* setUniform3f(const std::string& name, const glm::vec3& value);
        Shader* setUniform4f(const std::string& name, const glm::vec4& value);
        Shader* setUniformMat3f(const std::string& name, const glm::mat3& value);
        Shader* setUniformMat4f(const std::string& name, const glm::mat4& value);

        Shader* linkUniformBuffer(const Core::Ref<UniformBuffer>& uniformBuffer);
    };

}