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

    private:
//        static std::shared_ptr<OrthographicCamera> m_orthographicCamera;

    public:

//        static void beginScene(std::shared_ptr<OrthographicCamera> orthographicCamera);
        static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    };

}