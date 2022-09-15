#version 410 core

struct EntityPropertySet {
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
layout (std140) uniform EntityPropertySets {
    EntityPropertySet entityPropertySets[100];
};

layout (std140) uniform SceneProperties {
    mat4 view;
    mat4 projection;
    DirectionalLight directionalLight;
};

// Inputs
in vec2 v_textureCoordinates;
in vec3 v_normal;
flat in uint v_entityIndex;
in vec3 v_barycentricCoordinates;

// Outputs
layout (location = 0) out vec4 o_color;
layout (location = 1) out int o_entityId;

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

vec4 sampleTexture(uint textureIndex, vec2 textureCoordinates);
vec4 getDirectionalLightColor(
    vec3 color, vec3 direction,
    float ambientIntensity, float diffuseIntensity,
    vec3 normal
);

void main() {

    vec4 textureColor = sampleTexture(entityPropertySets[v_entityIndex].textureIndex, v_textureCoordinates);

    // If the fragment is close to the edge, it's a wire
    if (v_barycentricCoordinates.x < 0.02 || v_barycentricCoordinates.y < 0.02 || v_barycentricCoordinates.z < 0.02) {

        o_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    } else {

        vec4 directionalLightColor = getDirectionalLightColor(directionalLight.color, directionalLight.direction, directionalLight.ambientIntensity, directionalLight.diffuseIntensity, v_normal);
        o_color = textureColor * entityPropertySets[v_entityIndex].color * directionalLightColor;
        //o_color = vec4(v_textureCoordinates.x, v_textureCoordinates.y, 0, 1);

    }

    o_entityId = entityPropertySets[v_entityIndex].entityId;

}


vec4 getDirectionalLightColor(vec3 color, vec3 direction, float ambientIntensity, float diffuseIntensity, vec3 normal) {

    vec4 ambientColor = vec4(color * ambientIntensity, 1.0f);
    float diffuseFactor = dot(normalize(normal), -direction);

    if (diffuseFactor > 0) {
        return vec4(color * diffuseIntensity * diffuseFactor, 1.0f) + ambientColor;
    } else {
        return ambientColor;
    }

}

vec4 sampleTexture(uint textureIndex, vec2 textureCoordinates) {

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
