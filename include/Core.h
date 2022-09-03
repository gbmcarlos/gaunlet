#pragma once

#include "gaunlet/pch.h"

// Application
#include "gaunlet/core/application/Application.h"

// Events
#include "gaunlet/core/event/Event.h" // Doesn't depend on anything
#include "gaunlet/core/event/events/WindowEvent.h" // Depends on Event
#include "gaunlet/core/event/events/KeyEvent.h" // Depends on Event
#include "gaunlet/core/event/events/MouseEvent.h" // Depends on Event
#include "gaunlet/core/event/events/CursorEvent.h" // Depends on Event
#include "gaunlet/core/event/events/ScrollEvent.h" // Depends on Event

// Window
#include "gaunlet/core/window/KeyCodes.h" // Defines
#include "gaunlet/core/window/MouseButtonCodes.h" // Defines
#include "gaunlet/core/window/Window.h" // Depends on GLFW and Events
#include "gaunlet/core/window/Input.h" // Depends on Window

#include "gaunlet/core/render/RenderCommand.h" // Depends on RenderApi
#include "gaunlet/core/run-loop/RunLoop.h" // Depends on Window, Application, ImGuiRenderApi, and RenderCommand

#include <imgui.h>
#include <imgui_internal.h>
