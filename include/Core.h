#pragma once

#include "graphics-engine/pch.h"
#include "graphics-engine/core/utils.h" // Doesn't depend on anything

// Application
#include "graphics-engine/core/application/Application.h"
#include "graphics-engine/core/application/Layer.h"

// Events
#include "graphics-engine/core/event/Event.h" // Doesn't depend on anything
#include "graphics-engine/core/event/events/WindowEvent.h" // Depends on Event
#include "graphics-engine/core/event/events/KeyEvent.h" // Depends on Event
#include "graphics-engine/core/event/events/MouseEvent.h" // Depends on Event
#include "graphics-engine/core/event/events/CursorEvent.h" // Depends on Event
#include "graphics-engine/core/event/events/ScrollEvent.h" // Depends on Event

// Window
#include "graphics-engine/core/window/KeyCodes.h" // Defines
#include "graphics-engine/core/window/MouseButtonCodes.h" // Defines
#include "graphics-engine/core/window/Window.h" // Depends on GLFW and Events
#include "graphics-engine/core/window/Input.h" // Depends on Window

#include "graphics-engine/core/render/RenderCommand.h" // Depends on RenderApi
#include "graphics-engine/core/run-loop/RunLoop.h" // Depends on Window, Application, ImGuiRenderApi, and RenderCommand

#include <imgui.h>
#include <imgui_internal.h>
