#pragma once

#include "input/Input.h" // Doesn't depend on anything
#include "window/Window.h" // Depends on GLFW and Input
#include "render/RenderApi.h" // Doesn't depend on anything (it requires a cpp implementation to bind a RenderApi implementation to RenderCommand)
#include "render/opengl/OpenGLRenderAPI.h" // Depends on GLEW and RenderApi
#include "render/RenderCommand.h" // Depends on RenderApi
#include "imgui/ImGuiRenderApi.h" // Depends on GLFW and ImGui
#include "application/Application.h" // Depends on Input and TimeStep
#include "run-loop/RunLoop.h" // Depends on Window, Application, ImGuiRenderApi, and RenderCommand
