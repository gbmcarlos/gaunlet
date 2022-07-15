#pragma once

#include <memory>

#include "../shader/Shader.h"
#include "../buffer/VertexArray.h"
#include "../camera/OrthographicCamera.h"

namespace engine {

    class Renderer {

    public:
        Renderer() = delete;
        Renderer(Renderer const&) = delete;
        void operator=(Renderer const&)  = delete;

    public:

        static void beginScene(const std::shared_ptr<OrthographicCamera>& orthographicCamera);
        static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    private:

        struct SceneData {
            glm::mat4 m_viewProjectionMatrix;
        };

        static SceneData* m_sceneData;

    };

}