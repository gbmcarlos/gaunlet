#version 410 core

#include "GL_PREFABS_PATH/shaders/includes/scene-properties.glsl"
#include "./../entity-properties.glsl"

uniform float u_width;

// Vertex attributes
layout (location = 0) in vec4 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_textureCoordinates;
layout (location = 3) in uint a_entityIndex;

// Outputs
flat out uint v_entityIndex;

void main() {

    v_entityIndex = a_entityIndex;

    vec4 clipPosition = projection * view * entityPropertySets[a_entityIndex].transform * a_position;
    vec4 clipNormal = projection * view * entityPropertySets[a_entityIndex].transform * vec4(a_normal, 0.0f);

    vec2 offset = normalize(clipNormal.xy) * u_width * clipPosition.w;
    clipPosition.xy += offset;

    gl_Position = clipPosition;

}