#version 410 core

struct EntityProperties {
    mat4 transform;
    vec4 color;
    uint textureIndex;
    int entityId;
};

// Uniforms
layout (std140) uniform EntityPropertiesBlock {
    EntityProperties properties[100];
};

// Inputs
flat in uint v_entityIndex;

// Outputs
layout (location = 0) out vec4 o_color;
layout (location = 1) out int o_entityId;

void main() {

    o_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);

    o_entityId = properties[v_entityIndex].entityId;

}
