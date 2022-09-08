#version 410 core

struct EntityProperties {
    mat4 transform;
    vec4 color;
    uint textureIndex;
    int entityId;
};

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

// Uniforms
layout (std140) uniform EntityPropertiesBlock {
    EntityProperties properties[100];
};

layout (std140) uniform ScenePropertiesBlock {
    mat4 view;
    mat4 projection;
    DirectionalLight directionalLight;
};

// Vertex attributes
layout (location = 0) in vec4 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_textureCoordinates;
layout (location = 3) in uint a_entityIndex;

// Outputs
out vec2 v_textureCoordinates;
out vec3 v_normal;
flat out uint v_entityIndex;

void main() {

    v_textureCoordinates = a_textureCoordinates;
    v_normal = a_normal;
    v_entityIndex = a_entityIndex;

    gl_Position = projection * view * properties[a_entityIndex].transform * a_position;

}