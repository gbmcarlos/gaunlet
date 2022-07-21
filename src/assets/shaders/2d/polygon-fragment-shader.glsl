#version 410 core

// Inputs
in vec2 v_textureCoordinates;
flat in int v_textureIndex;
in vec4 v_color;

// Outputs
out vec4 color;

// Textures
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {

    if (v_textureIndex == 0) {
        color = texture(texture0, v_textureCoordinates);
    } else if (v_textureIndex == 1) {
        color = texture(texture1, v_textureCoordinates);
    } else if (v_textureIndex == 2) {
        color = texture(texture2, v_textureCoordinates);
    }

    color = color * v_color;
}
