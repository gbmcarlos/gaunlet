#pragma once

#include "Graphics.h"

#include <box2d/box2d.h> // Expose box2d to the client

#include "../scene/renderer/Renderer.h" // Depends on Graphics and GraphicsComponents
#include "../scene/Scene.h" // Depends on ENTT
#include "../scene/entity/Entity.h" // Depends on Scene

#include "../scene/camera/OrthographicCamera.h" // Doesn't depend on anything
#include "../scene/mesh/PolygonMesh.h" // Doesn't depend on anything
#include "../scene/mesh/2d/samples/TriangleMesh.h" // Depends on PolygonMesh
#include "../scene/mesh/2d/samples/SquareMesh.h" // Depends on PolygonMesh

#include "../scene/entity/GraphicsComponents.h" // Depends on PolygonMesh and CircleMesh
#include "../scene/entity/PhysicsComponents.h" // Depends on box2d
#include "../scene/entity/ScriptComponents.h" // Depends on Entity
