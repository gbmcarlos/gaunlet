#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace engine {

    class Shader {

    private:
        unsigned int rendererId;
        GLint getUniformLocation(const std::string name);
        std::unordered_map<std::string, GLint> uniformLocations;
    public:
        Shader();
        ~Shader();
        void attach(unsigned int type, std::string shaderPath);
        void compile() const;
        void bind() const;
        void setUniform3f(const std::string name, glm::vec3);
        void setUniform4f(const std::string name, glm::vec4);
        void setUniformMat4f(const std::string name, glm::mat4 value);
    };

}