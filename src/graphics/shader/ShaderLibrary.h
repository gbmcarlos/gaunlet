#pragma once

#include "graphics/shader/Shader.h"

#include "pch.h"

namespace engine::Graphics {

    class ShaderLibrary {

    public:
        ShaderLibrary() = default;

        Core::Ref<Shader> load(const std::string& name, const std::map<Core::ShaderType, std::string>& shaderSources);
        Core::Ref<Shader> add(const std::string& name, const Core::Ref<Shader>& shader);
        const Core::Ref<Shader>& get(const std::string& name);

    private:
        std::unordered_map<std::string, Core::Ref<Shader> > m_shaders;

    };

}