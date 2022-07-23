#pragma once

#include "../core/utils.h" // Doesn't depend on anything

#include "../core/input/Event.h" // Doesn't depend on anything
#include "../core/input/events/WindowEvent.h" // Depends on Event
#include "../core/input/events/KeyEvent.h" // Depends on Event
#include "../core/input/events/MouseEvent.h" // Depends on Event
#include "../core/input/events/CursorEvent.h" // Depends on Event
#include "../core/input/events/ScrollEvent.h" // Depends on Event

#include "../core/window/KeyCodes.h" // Defines
#include "../core/window/MouseButtonCodes.h" // Defines
#include "../core/window/Window.h" // Depends on GLFW and Input
#include "../core/application/Application.h" // Depends on Window and EventBus
#include "../core/render/RenderCommand.h" // Depends on RenderApi

#include "../core/run-loop/RunLoop.h" // Depends on Window, Application, ImGuiRenderApi, and RenderCommand
