#pragma once

#include "Core.h"

#include "gaunlet/graphics/vertex-buffer/BufferLayout.h" // Depends on Core/RenderCommand
#include "gaunlet/graphics/vertex-buffer/VertexBuffer.h" // Depends on BufferLayout and RenderCommand
#include "gaunlet/graphics/index-buffer/IndexBuffer.h" // Depends on Core/RenderCommand
#include "gaunlet/graphics/vertex-array/VertexArray.h" // Depends on Core/RenderCommand, VertexBuffer, and IndexBuffer
#include "gaunlet/graphics/shader/Shader.h" // Depends on Core/RenderCommand
#include "gaunlet/graphics/shader/ShaderLibrary.h" // Depends on Shader
#include "gaunlet/graphics/texture/Texture.h" // Depends on STB and Core/RenderCommand
#include "gaunlet/graphics/texture/TextureImage2D.h" // Depends on Texture
#include "gaunlet/graphics/texture/TextureCubeMap.h" // Depends on Texture
#include "gaunlet/graphics/framebuffer/Framebuffer.h"
#include "gaunlet/graphics/Vertex.h"
