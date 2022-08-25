#pragma once

#include "Graphics.h"

#include <box2d/box2d.h> // Expose box2d to the client

#include "graphics-engine/scene/deferred-renderer/DeferredRenderer.h" // Depends on Graphics and GraphicsComponents
#include "graphics-engine/scene/Scene.h" // Depends on ENTT
#include "graphics-engine/scene/entity/Entity.h" // Depends on Scene

#include "graphics-engine/scene/camera/Camera.h" // Doesn't depend on anything
#include "graphics-engine/scene/camera/PerspectiveCamera.h" // Doesn't depend on anything
#include "graphics-engine/scene/camera/OrthographicCamera.h" // Doesn't depend on anything

#include "graphics-engine/scene/model/Model.h"
#include "graphics-engine/scene/model/Vertex.h"
#include "graphics-engine/scene/model/Sprites.h"

#include "graphics-engine/scene/entity/GraphicsComponents.h" // Depends on PolygonMesh and CircleMesh
#include "graphics-engine/scene/entity/PhysicsComponents.h" // Depends on box2d
#include "graphics-engine/scene/entity/ScriptComponents.h" // Depends on Entity
#include "graphics-engine/scene/entity/LightingComponents.h"
