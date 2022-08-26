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
    vec3 position;
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

// Inputs
flat in uint v_entityIndex;
in vec3 v_barycentricCoordinates;

// Outputs
layout (location = 0) out vec4 o_color;
layout (location = 1) out int o_entityId;

void main() {

    if (v_barycentricCoordinates.x > 0.01 && v_barycentricCoordinates.y > 0.01 && v_barycentricCoordinates.z > 0.01) {
        discard;
    }

    o_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    o_entityId = properties[v_entityIndex].entityId;

}
