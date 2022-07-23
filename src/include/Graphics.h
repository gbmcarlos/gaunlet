#pragma once

#include "Core.h"

#include "../graphics/buffer/BufferLayout.h" // Depends on Core/RenderCommand
#include "../graphics/buffer/VertexBuffer.h" // Depends on BufferLayout and RenderCommand
#include "../graphics/buffer/IndexBuffer.h" // Depends on Core/RenderCommand
#include "../graphics/buffer/VertexArray.h" // Depends on Core/RenderCommand, VertexBuffer, and IndexBuffer
#include "../graphics/shader/Shader.h" // Depends on Core/RenderCommand
#include "../graphics/shader/ShaderLibrary.h" // Depends on Shader
#include "../graphics/texture/Texture.h" // Depends on STB and Core/RenderCommand
