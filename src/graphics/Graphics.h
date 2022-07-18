#pragma once

#include "buffer/BufferLayout.h" // Doesn't depend on RenderCommand
#include "buffer/IndexBuffer.h" // Depends in RenderCommand
#include "buffer/VertexBuffer.h" // Depends on BufferLayout and RenderCommand
#include "buffer/VertexArray.h" // Depends in VertexBuffer, IndexBuffer and RenderCommand
#include "shader/Shader.h" // Depends on RenderCommand

#include "camera/OrthographicCamera.h" // Depends on Core/Input
#include "mesh/Mesh.h"
#include "renderer/Renderer.h" // Depends on Shader, OrthographicCamera, VertexArray, and Mesh
