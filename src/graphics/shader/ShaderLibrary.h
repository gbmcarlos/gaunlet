#pragma once

#include "Shader.h"

#include "../pch.h"

namespace engine {

    class ShaderLibrary {

    public:
        ShaderLibrary() = default;

        Ref<Shader> load(const std::string& name, const std::map<ShaderType, std::string>& shaderSources);
        Ref<Shader> add(const std::string& name, const Ref<Shader>& shader);
        const Ref<Shader>& get(const std::string& name);

    private:
        std::unordered_map<std::string, Ref<Shader> > m_shaders;

    };

}