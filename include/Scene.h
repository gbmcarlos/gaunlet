#pragma once

#include "Graphics.h"

#include "gaunlet/scene/deferred-renderer/DeferredRenderer.h" // Depends on Graphics and GraphicsComponents
#include "gaunlet/scene/Scene.h" // Depends on ENTT

#include "gaunlet/scene/camera/Camera.h" // Doesn't depend on anything
#include "gaunlet/scene/camera/PerspectiveCamera.h" // Doesn't depend on anything
#include "gaunlet/scene/camera/OrthographicCamera.h" // Doesn't depend on anything

#include "gaunlet/scene/model/Model.h"
#include "gaunlet/scene/model/Sprites.h"

#include "gaunlet/scene/components/GraphicsComponents.h" // Depends on PolygonMesh and CircleMesh
#include "gaunlet/scene/components/ScriptComponents.h" // Depends on Entity
#include "gaunlet/scene/components/LightingComponents.h"
