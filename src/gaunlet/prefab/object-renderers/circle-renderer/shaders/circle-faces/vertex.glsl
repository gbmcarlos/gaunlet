#version 410 core

#include "GL_PREFABS_PATH/shaders/includes/scene-properties.glsl"
#include "entity-properties.glsl"

// Vertex attributes
layout (location = 0) in vec4 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_textureCoordinates;
layout (location = 3) in uint a_entityIndex;

// Outputs
out vec2 v_textureCoordinates;
out vec3 v_normal;
flat out uint v_entityIndex;
out vec2 v_localCoordinates;

void main() {

    v_textureCoordinates = a_textureCoordinates;
    v_normal = a_normal;
    v_entityIndex = a_entityIndex;
    v_localCoordinates = vec2(sign(a_position.x), sign(a_position.y));

    gl_Position = projection * view * entityPropertySet[a_entityIndex].transform * a_position;

}