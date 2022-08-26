#include "gaunlet/core/render/RenderCommand.h"
#include "gaunlet/core/render/opengl/OpenGLRenderAPI.h"

namespace gaunlet::Core {

    RenderApi& RenderCommand::m_apiInstance = OpenGLRenderApi::getInstance();

}