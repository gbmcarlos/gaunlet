#pragma once

#include "gaunlet/graphics/shader/ShaderLibrary.h"
#include "gaunlet/prefab/sprites/Sprites.h"
#include "gaunlet/graphics/texture/TextureCubeMap.h"
#include "gaunlet/graphics/render-pass/SimpleRenderPass.h"

namespace gaunlet::Prefab::ObjectRenderers {

    class SkyboxRenderer {

    public:

        SkyboxRenderer() {
            loadShaders();
        }

        void render(const Core::Ref<Graphics::TextureCubeMap>& cubeMap, const Core::Ref<Graphics::Shader>& shader) {

            // Activate the cubeMap texture
            cubeMap->activate(0);

            Graphics::SimpleRenderPass::renderIndexedVertices(
                m_cube.getMeshes()[0].getVertices(),
                m_cube.getMeshes()[0].getIndices(),
                shader,
                Graphics::RenderMode::Triangle
            );

        }

        static unsigned int getUniformBufferCount() {return 0; }

        inline Graphics::ShaderLibrary& getShaders() {return m_shaderLibrary; }

    private:

        Prefab::Sprites::CubeModel m_cube;

        void loadShaders() {

            std::map<Core::ShaderType, std::string> sources {
                {Core::ShaderType::Vertex, PREFABS_PATH"/object-renderers/skybox-renderer/shaders/vertex.glsl"},
                {Core::ShaderType::Fragment, PREFABS_PATH"/object-renderers/skybox-renderer/shaders/fragment.glsl"}
            };

            auto shader = m_shaderLibrary.load("skybox", sources);

            // Set a single "skybox" texture
            shader->setUniform1i("skybox", 0);

        }

        Graphics::ShaderLibrary m_shaderLibrary;

    };

}