#version 410 core

struct EntityPropertySet {
    mat4 transform;
    vec4 color;
    uint textureIndex;
    int entityId;
};

// Uniforms
layout (std140) uniform EntityPropertySets {
    EntityPropertySet entityPropertySets[100];
};

uniform vec3 u_color;

// Inputs
flat in uint v_entityIndex;

// Outputs
layout (location = 0) out vec4 o_color;
layout (location = 1) out int o_entityId;

void main() {

    o_color = vec4(u_color, 1.0f);

    o_entityId = entityPropertySets[v_entityIndex].entityId;

}
