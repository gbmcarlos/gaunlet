#include "RenderCommand.h"
#include "opengl/OpenGLRenderAPI.h"

namespace engine::Core {

    RenderApi& RenderCommand::m_apiInstance = OpenGLRenderApi::getInstance();

}