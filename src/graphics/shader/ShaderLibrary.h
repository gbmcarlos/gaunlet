#pragma once

#include "Shader.h"

#include <map>

namespace engine {

    class ShaderLibrary {

    public:
        ShaderLibrary() = default;

        std::shared_ptr<Shader> load(const std::string& name, const std::map<ShaderType, std::string>& shaderSources);
        std::shared_ptr<Shader> add(const std::string& name, const std::shared_ptr<Shader>& shader);
        std::shared_ptr<Shader> get(const std::string& name);

    private:
        std::unordered_map<std::string, std::shared_ptr<Shader> > m_shaders;

    };

}