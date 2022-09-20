#version 410 core

struct EntityPropertySet {
    uint textureIndex;
    float leftSizeFactor;
    float rightSizeFactor;
    float bottomSizeFactor;
    float topSizeFactor;
};

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

// Uniforms
layout (std140) uniform EntityPropertySets {
    EntityPropertySet entityPropertySets[100];
};

layout (std140) uniform SceneProperties {
    mat4 view;
    mat4 projection;
    DirectionalLight directionalLight;
};

uniform mat4 u_modelTransform;
uniform float u_maxHeight;

// Vertex attributes
layout (location = 0) in vec4 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_textureCoordinates;
layout (location = 3) in uint a_entityIndex;

// Outputs
out vec2 v_textureCoordinates;
out vec3 v_normal;
flat out uint v_entityIndex;

// Textures
uniform sampler2D heightmap;

void main() {

    v_textureCoordinates = a_textureCoordinates;
    v_normal = a_normal;
    v_entityIndex = a_entityIndex;

    vec4 vertexPosition = u_modelTransform * a_position;
    float height = texture(heightmap, v_textureCoordinates).y * u_maxHeight;
    vertexPosition += vec4(vec3(0, 1, 0) * height, 0);

    gl_Position = vertexPosition;

}