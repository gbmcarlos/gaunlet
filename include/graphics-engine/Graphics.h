#pragma once

#include "Core.h"

#include "../../src/graphics/vertex-buffer/BufferLayout.h" // Depends on Core/RenderCommand
#include "../../src/graphics/vertex-buffer/VertexBuffer.h" // Depends on BufferLayout and RenderCommand
#include "../../src/graphics/index-buffer/IndexBuffer.h" // Depends on Core/RenderCommand
#include "../../src/graphics/vertex-array/VertexArray.h" // Depends on Core/RenderCommand, VertexBuffer, and IndexBuffer
#include "../../src/graphics/shader/Shader.h" // Depends on Core/RenderCommand
#include "../../src/graphics/shader/ShaderLibrary.h" // Depends on Shader
#include "../../src/graphics/texture/Texture.h" // Depends on STB and Core/RenderCommand
#include "../../src/graphics/texture/TextureImage2D.h" // Depends on Texture
#include "../../src/graphics/framebuffer/Framebuffer.h"
