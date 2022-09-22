#version 410 core

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    vec3 position;
    float diffuseIntensity;
};

layout (std140) uniform SceneProperties {
    mat4 view;
    mat4 projection;
    vec4 viewport;
    DirectionalLight directionalLight;
};

// Vertex attributes
layout (location = 0) in vec4 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_textureCoordinates;
layout (location = 3) in uint a_entityIndex;

// Outputs
out vec3 v_textureCoordinates;

void main() {

    v_textureCoordinates = vec3(a_position);
    vec4 position = projection * mat4(mat3(view)) * vec4(vec3(a_position), 1.0f);
    gl_Position = position.xyww;

}