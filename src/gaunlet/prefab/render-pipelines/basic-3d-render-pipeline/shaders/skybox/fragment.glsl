#version 410 core

// Inputs
in vec3 v_textureCoordinates;

// Outputs
layout (location = 0) out vec4 o_color;

// Textures
uniform samplerCube skybox;

void main() {
    o_color = texture(skybox, v_textureCoordinates);
}