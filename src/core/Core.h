#pragma once

#include "utils.h" // Doesn't depend on anything

#include "input/Event.h" // Doesn't depend on anything
#include "input/EventBus.h" // Depends on Event
#include "input/events/WindowEvent.h" // Depends on Event
#include "input/events/KeyEvent.h" // Depends on Event
#include "input/events/MouseEvent.h" // Depends on Event
#include "input/events/CursorEvent.h" // Depends on Event
#include "input/events/ScrollEvent.h" // Depends on Event

#include "window/KeyCodes.h" // Defines
#include "window/MouseButtonCodes.h" // Defines
#include "window/Window.h" // Depends on GLFW and Input
#include "application/Application.h" // Depends on Window and EventBus
#include "run-loop/RunLoop.h" // Depends on Application
#include "render/RenderApi.h" // Doesn't depend on anything (it requires a cpp implementation to bind a RenderApi implementation to RenderCommand)
#include "render/opengl/OpenGLRenderAPI.h" // Depends on GLEW and RenderApi
#include "render/RenderCommand.h" // Depends on RenderApi

//#include "input/Input.h" // Depends on GLFW

#include "imgui/ImGuiRenderApi.h" // Depends on GLFW and ImGui
#include "application/Application.h" // Depends on Input and TimeStep
#include "run-loop/RunLoop.h" // Depends on Window, Application, ImGuiRenderApi, and RenderCommand
