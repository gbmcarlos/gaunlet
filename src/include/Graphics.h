#pragma once

#include "Core.h"

#include "../graphics/vertex-buffer/BufferLayout.h" // Depends on Core/RenderCommand
#include "../graphics/vertex-buffer/VertexBuffer.h" // Depends on BufferLayout and RenderCommand
#include "../graphics/index-buffer/IndexBuffer.h" // Depends on Core/RenderCommand
#include "../graphics/vertex-array/VertexArray.h" // Depends on Core/RenderCommand, VertexBuffer, and IndexBuffer
#include "../graphics/shader/Shader.h" // Depends on Core/RenderCommand
#include "../graphics/shader/ShaderLibrary.h" // Depends on Shader
#include "../graphics/texture/Texture.h" // Depends on STB and Core/RenderCommand
#include "../graphics/texture/TextureImage2D.h" // Depends on Texture
#include "../graphics/framebuffer/Framebuffer.h"
