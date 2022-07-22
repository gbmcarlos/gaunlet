#pragma once

#include "../graphics/buffer/BufferLayout.h" // Depends on RenderCommand
#include "../graphics/buffer/IndexBuffer.h" // Depends on RenderCommand
#include "../graphics/buffer/VertexBuffer.h" // Depends on BufferLayout and RenderCommand
#include "../graphics/buffer/VertexArray.h" // Depends on VertexBuffer, IndexBuffer and RenderCommand
#include "../graphics/shader/Shader.h" // Depends on RenderCommand
#include "../graphics/shader/ShaderLibrary.h" // Depends on Shader

#include "../graphics/scene/Scene.h" // Depends on ENTT
#include "../graphics/scene/Component.h" // Depends on PolygonMesh
#include "../graphics/scene/Entity.h" // Depends on Scene

#include "../graphics/camera/OrthographicCamera.h" // Doesn't depend on anything
#include "../graphics/mesh/PolygonMesh.h" // Doesn't depend on anything
#include "../graphics/mesh/2d/samples/TriangleMesh.h"
#include "../graphics/mesh/2d/samples/SquareMesh.h"
#include "../graphics/renderer/Renderer.h" // Depends on Shader, OrthographicCamera, VertexArray, and PolygonMesh
