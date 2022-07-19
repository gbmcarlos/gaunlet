#pragma once

#include "buffer/BufferLayout.h" // Depends on RenderCommand
#include "buffer/IndexBuffer.h" // Depends on RenderCommand
#include "buffer/VertexBuffer.h" // Depends on BufferLayout and RenderCommand
#include "buffer/VertexArray.h" // Depends on VertexBuffer, IndexBuffer and RenderCommand
#include "shader/Shader.h" // Depends on RenderCommand

#include "camera/OrthographicCamera.h" // Doesn't depend on anything
#include "mesh/Mesh.h" // Doesn't depend on anything
#include "renderer/Renderer.h" // Depends on Shader, OrthographicCamera, VertexArray, and Mesh
