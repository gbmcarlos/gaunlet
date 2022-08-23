#include "graphics/shader/ShaderLibrary.h"

namespace engine::Graphics {

    Core::Ref<Shader> ShaderLibrary::load(const std::string& name, const std::map<Core::ShaderType, std::string>& shaderSources) {

        if (m_shaders.find(name) != m_shaders.end()) {
            return m_shaders[name];
        }

        Core::Ref<Shader> shader = Core::CreateRef<Shader>();

        for (const auto& shaderSource : shaderSources) {
            shader->attach(shaderSource.first, shaderSource.second);
        }

        shader->compile();

        add(name, shader);

        return shader;

    }

    Core::Ref<Shader> ShaderLibrary::add(const std::string& name, const Core::Ref<Shader>& shader) {

        if (m_shaders.find(name) != m_shaders.end()) {
            throw std::runtime_error("Duplicate shader name");
        }

        m_shaders[name] = shader;

        return shader;

    }

    const Core::Ref<Shader>& ShaderLibrary::get(const std::string &name) {

        if (m_shaders.find(name) == m_shaders.end()) {
            throw std::runtime_error("Shader doesn't exist");
        }

        return m_shaders[name];

    }

}