#include "RenderCommand.h"
#include "opengl/OpenGLRenderAPI.h"

namespace engine {

    RenderApi& RenderCommand::m_apiInstance = OpenGLRenderApi::getInstance();

}