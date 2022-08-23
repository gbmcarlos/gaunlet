#pragma once


#include "../../src/pch.h"
#include "../../src/pch.h"
#include "../../src/core/utils.h" // Doesn't depend on anything

// Application
#include "../../src/core/application/Application.h"
#include "../../src/core/application/Layer.h"

// Events
#include "../../src/core/event/Event.h" // Doesn't depend on anything
#include "../../src/core/event/events/WindowEvent.h" // Depends on Event
#include "../../src/core/event/events/KeyEvent.h" // Depends on Event
#include "../../src/core/event/events/MouseEvent.h" // Depends on Event
#include "../../src/core/event/events/CursorEvent.h" // Depends on Event
#include "../../src/core/event/events/ScrollEvent.h" // Depends on Event

// Window
#include "../../src/core/window/KeyCodes.h" // Defines
#include "../../src/core/window/MouseButtonCodes.h" // Defines
#include "../../src/core/window/Window.h" // Depends on GLFW and Events
#include "../../src/core/application/Application.h" // Depends on Window and EventBus
#include "../../src/core/render/RenderCommand.h" // Depends on RenderApi
#include "../../src/core/window/Input.h" // Depends on Window

#include "../../src/core/run-loop/RunLoop.h" // Depends on Window, Application, ImGuiRenderApi, and RenderCommand

#include <imgui.h>
#include <imgui_internal.h>
