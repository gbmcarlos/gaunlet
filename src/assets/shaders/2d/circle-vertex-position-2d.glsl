#version 410 core

struct EntityProperties {
    mat4 transform;
    vec4 color;
    uint textureIndex;
    uint entityId;
    float thickness;
    float fade;
};

// Uniforms
layout (std140) uniform EntityPropertiesBlock {
    EntityProperties properties[100];
};

layout (std140) uniform SceneMatricesBlock {
    mat4 projection;
    mat4 view;
};

// Vertex attributes
layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_localCoordinates;
layout (location = 2) in vec2 a_textureCoordinates;
layout (location = 3) in uint a_entityIndex;

// Outputs
out vec2 v_localCoordinates;
out vec2 v_textureCoordinates;
flat out uint v_entityIndex;

void main() {

    v_localCoordinates = a_localCoordinates;
    v_textureCoordinates = a_textureCoordinates;
    v_entityIndex = a_entityIndex;

    gl_Position = projection * view * properties[a_entityIndex].transform * a_position;

}