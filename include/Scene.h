#pragma once

#include "Graphics.h"

#include <box2d/box2d.h> // Expose box2d to the client

#include "gaunlet/scene/deferred-renderer/DeferredRenderer.h" // Depends on Graphics and GraphicsComponents
#include "gaunlet/scene/Scene.h" // Depends on ENTT
#include "gaunlet/scene/entity/Entity.h" // Depends on Scene

#include "gaunlet/scene/camera/Camera.h" // Doesn't depend on anything
#include "gaunlet/scene/camera/PerspectiveCamera.h" // Doesn't depend on anything
#include "gaunlet/scene/camera/OrthographicCamera.h" // Doesn't depend on anything

#include "gaunlet/scene/model/Model.h"
#include "gaunlet/scene/model/Sprites.h"

#include "gaunlet/scene/entity/GraphicsComponents.h" // Depends on PolygonMesh and CircleMesh
#include "gaunlet/scene/entity/PhysicsComponents.h" // Depends on box2d
#include "gaunlet/scene/entity/ScriptComponents.h" // Depends on Entity
#include "gaunlet/scene/entity/LightingComponents.h"
