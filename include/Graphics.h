#pragma once

#include "Core.h"

#include "graphics-engine/graphics/vertex-buffer/BufferLayout.h" // Depends on Core/RenderCommand
#include "graphics-engine/graphics/vertex-buffer/VertexBuffer.h" // Depends on BufferLayout and RenderCommand
#include "graphics-engine/graphics/index-buffer/IndexBuffer.h" // Depends on Core/RenderCommand
#include "graphics-engine/graphics/vertex-array/VertexArray.h" // Depends on Core/RenderCommand, VertexBuffer, and IndexBuffer
#include "graphics-engine/graphics/shader/Shader.h" // Depends on Core/RenderCommand
#include "graphics-engine/graphics/shader/ShaderLibrary.h" // Depends on Shader
#include "graphics-engine/graphics/texture/Texture.h" // Depends on STB and Core/RenderCommand
#include "graphics-engine/graphics/texture/TextureImage2D.h" // Depends on Texture
#include "graphics-engine/graphics/framebuffer/Framebuffer.h"
