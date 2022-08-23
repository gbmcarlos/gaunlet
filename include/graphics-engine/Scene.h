#pragma once

#include "Graphics.h"

#include <box2d/box2d.h> // Expose box2d to the client

#include "../../src/scene/deferred-renderer/DeferredRenderer.h" // Depends on Graphics and GraphicsComponents
#include "../../src/scene/Scene.h" // Depends on ENTT
#include "../../src/scene/entity/Entity.h" // Depends on Scene

#include "../../src/scene/camera/Camera.h" // Doesn't depend on anything
#include "../../src/scene/camera/PerspectiveCamera.h" // Doesn't depend on anything
#include "../../src/scene/camera/OrthographicCamera.h" // Doesn't depend on anything

#include "../../src/scene/model/Model.h"
#include "../../src/scene/model/Vertex.h"
#include "../../src/scene/model/Sprites.h"

#include "../../src/scene/entity/GraphicsComponents.h" // Depends on PolygonMesh and CircleMesh
#include "../../src/scene/entity/PhysicsComponents.h" // Depends on box2d
#include "../../src/scene/entity/ScriptComponents.h" // Depends on Entity
#include "../../src/scene/entity/LightingComponents.h"
