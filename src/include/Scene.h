#pragma once

#include "Graphics.h"

#include <box2d/box2d.h> // Expose box2d to the client

#include "../scene/renderer/Renderer.h" // Depends on Graphics and GraphicsComponents
#include "../scene/Scene.h" // Depends on ENTT
#include "../scene/entity/Entity.h" // Depends on Scene

#include "../scene/camera/Camera.h" // Doesn't depend on anything
#include "../scene/camera/PerspectiveCamera.h" // Doesn't depend on anything
#include "../scene/camera/OrthographicCamera.h" // Doesn't depend on anything

#include "../scene/model/Model.h"
#include "../scene/model/polygon-mesh/PolygonMesh.h" // Doesn't depend on anything
#include "../scene/model/circle-mesh/CircleMesh.h" // Doesn't depend on anything
#include "../scene/model/Sprites.h"

#include "../scene/entity/GraphicsComponents.h" // Depends on PolygonMesh and CircleMesh
#include "../scene/entity/PhysicsComponents.h" // Depends on box2d
#include "../scene/entity/ScriptComponents.h" // Depends on Entity
