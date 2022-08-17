#pragma once

#include "../../core/render/RenderApi.h"

#include "../pch.h"

namespace engine {

    class Shader {

    private:
        unsigned int m_rendererId;
        int getUniformLocation(const std::string& name);
        std::unordered_map<std::string, int> m_uniformLocations;
    public:
        Shader();
        ~Shader();
        void attach(ShaderType type, const std::string& shaderPath);
        void compile() const;
        void bind() const;
        void setUniform1i(const std::string& name, int value);
        void setUniform3f(const std::string& name, const glm::vec3& value);
        void setUniform4f(const std::string& name, const glm::vec4& value);
        void setUniformMat3f(const std::string& name, const glm::mat3& value);
        void setUniformMat4f(const std::string& name, const glm::mat4& value);
    };

}