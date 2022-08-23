#include "core/render/RenderCommand.h"
#include "core/render/opengl/OpenGLRenderAPI.h"

namespace engine::Core {

    RenderApi& RenderCommand::m_apiInstance = OpenGLRenderApi::getInstance();

}