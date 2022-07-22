#version 410 core

// Inputs
in vec2 v_textureCoordinates;
flat in int v_textureIndex;
in vec4 v_color;

// Outputs
out vec4 o_color;

// Textures
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;
uniform sampler2D texture8;
uniform sampler2D texture9;
uniform sampler2D texture10;

vec4 sampleTexture(int textureIndex, vec2 textureCoordinates);

void main() {

    vec4 textureColor = sampleTexture(v_textureIndex, v_textureCoordinates);
    o_color = textureColor * v_color;
}

vec4 sampleTexture(int textureIndex, vec2 textureCoordinates) {

    if (textureIndex == 0) {
        return texture(texture0, textureCoordinates);
    } else if (textureIndex == 1) {
        return texture(texture1, textureCoordinates);
    } else if (textureIndex == 2) {
        return texture(texture2, textureCoordinates);
    } else if (textureIndex == 3) {
        return texture(texture3, textureCoordinates);
    } else if (textureIndex == 4) {
        return texture(texture4, textureCoordinates);
    } else if (textureIndex == 5) {
        return texture(texture5, textureCoordinates);
    } else if (textureIndex == 6) {
        return texture(texture6, textureCoordinates);
    } else if (textureIndex == 7) {
        return texture(texture7, textureCoordinates);
    } else if (textureIndex == 8) {
        return texture(texture8, textureCoordinates);
    } else if (textureIndex == 9) {
        return texture(texture9, textureCoordinates);
    } else if (textureIndex == 10) {
        return texture(texture10, textureCoordinates);
    }

    return vec4(1.0f, 1.0f, 1.0f, 1.0f);

}
