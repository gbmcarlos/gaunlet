#include "ShaderLibrary.h"

#include "exception"

namespace engine {

    Ref<Shader> ShaderLibrary::load(const std::string& name, const std::map<ShaderType, std::string>& shaderSources) {

        if (m_shaders.find(name) != m_shaders.end()) {
            return m_shaders[name];
        }

        Ref<Shader> shader = CreateRef<engine::Shader>();

        for (const auto& shaderSource : shaderSources) {
            shader->attach(shaderSource.first, shaderSource.second);
        }

        shader->compile();

        add(name, shader);

        return shader;

    }

    Ref<Shader> ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader) {

        if (m_shaders.find(name) != m_shaders.end()) {
            throw std::runtime_error("Duplicate shader name");
        }

        m_shaders[name] = shader;

        return shader;

    }

    const Ref<Shader>& ShaderLibrary::get(const std::string &name) {

        if (m_shaders.find(name) == m_shaders.end()) {
            throw std::runtime_error("Shader doesn't exist");
        }

        return m_shaders[name];

    }

}