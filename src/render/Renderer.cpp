#include "Renderer.h"

namespace engine {

    void Renderer::init() {
        OpenGLRenderApi::getInstance().init();
    }

    void Renderer::clear(glm::vec4 color) {
        OpenGLRenderApi::getInstance().setClearColor(color.r, color.g, color.b, color.a);
        OpenGLRenderApi::getInstance().clear();
    }

}