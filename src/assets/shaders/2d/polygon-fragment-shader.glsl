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

vec4 sampleTexture(int textureIndex, vec2 textureCoordinates);

void main() {

    vec4 textureColor = sampleTexture(v_textureIndex, v_textureCoordinates);
    color = textureColor * v_color;
}

vec4 sampleTexture(int textureIndex, vec2 textureCoordinates) {

    if (textureIndex == 0) {
        return texture(texture0, textureCoordinates);
    } else if (textureIndex == 1) {
        return texture(texture1, textureCoordinates);
    } else if (textureIndex == 2) {
        return texture(texture2, textureCoordinates);
    }

    return vec4(1.0f, 1.0f, 1.0f, 1.0f);

}
