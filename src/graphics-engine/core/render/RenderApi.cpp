#include "graphics-engine/core/render/RenderCommand.h"
#include "graphics-engine/core/render/opengl/OpenGLRenderAPI.h"

namespace engine::Core {

    RenderApi& RenderCommand::m_apiInstance = OpenGLRenderApi::getInstance();

}