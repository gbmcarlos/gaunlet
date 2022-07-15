#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace engine {

    class Shader {

    private:
        unsigned int m_rendererId;
        GLint getUniformLocation(const std::string& name);
        std::unordered_map<std::string, GLint> m_uniformLocations;
    public:
        Shader();
        ~Shader();
        void attach(unsigned int type, const std::string& shaderPath);
        void compile() const;
        void bind() const;
        void setUniform3f(const std::string& name, const glm::vec3& value);
        void setUniform4f(const std::string& name, const glm::vec4& value);
        void setUniformMat4f(const std::string& name, const glm::mat4& value);
    };

}