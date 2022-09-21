#version 410 core

layout (quads, equal_spacing, ccw) in;

struct EntityPropertySet {
    float leftSizeFactor;
    float bottomSizeFactor;
    float rightSizeFactor;
    float topSizeFactor;
    uint textureIndex;
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

uniform float u_maxHeight;

in vec2 tc_textureCoordinates[];
in vec3 tc_normal[];

out vec2 te_textureCoordinates;
out vec3 te_normal;
out vec3 tc_vertexColor;

// Textures
uniform sampler2D heightmap;

vec2 interpolateTextureCoordinates();
vec3 interpolateNormal();
vec3 getVertexColor();
vec4 interpolateVertexPosition();
vec2 interpolate2(vec2 point, vec2 vector1, vec2 vector2, vec2 vector3, vec2 vector4);
vec3 interpolate3(vec2 point, vec3 vector1, vec3 vector2, vec3 vector3, vec3 vector4);
vec4 interpolate4(vec2 point, vec4 vector1, vec4 vector2, vec4 vector3, vec4 vector4);

void main() {

    te_textureCoordinates = interpolateTextureCoordinates();
    te_normal = interpolateNormal();
    tc_vertexColor = getVertexColor();

    vec4 vertexPosition = interpolateVertexPosition();
    float height = texture(heightmap, te_textureCoordinates).y * u_maxHeight;
    vertexPosition.y = height;

    gl_Position = projection * view * vertexPosition;

}

vec2 interpolateTextureCoordinates() {
    return interpolate2(gl_TessCoord.xy, tc_textureCoordinates[0], tc_textureCoordinates[1], tc_textureCoordinates[2], tc_textureCoordinates[3]);
}

vec3 interpolateNormal() {
    return interpolate3(gl_TessCoord.xy, tc_normal[0], tc_normal[1], tc_normal[2], tc_normal[3]);
}

vec3 getVertexColor() {

    float edgeThreshold = 0.001f;

    if (gl_TessCoord.x < edgeThreshold || gl_TessCoord.x > 1-edgeThreshold || gl_TessCoord.y < edgeThreshold || gl_TessCoord.y > 1-edgeThreshold) {
        return vec3(1, 0, 0);
    } else {
        return vec3(0, 1, 0);
    }

}

vec4 interpolateVertexPosition() {

    return vec4(
        interpolate3(
            gl_TessCoord.xy,
            vec3(gl_in[0].gl_Position.x, 0, gl_in[0].gl_Position.z),
            vec3(gl_in[1].gl_Position.x, 0, gl_in[1].gl_Position.z),
            vec3(gl_in[2].gl_Position.x, 0, gl_in[2].gl_Position.z),
            vec3(gl_in[3].gl_Position.x, 0, gl_in[3].gl_Position.z)
        ),
        1
    );

}

vec2 interpolate2(vec2 point, vec2 vector1, vec2 vector2, vec2 vector3, vec2 vector4) {

    vec2 a = mix(vector1, vector2, point.x);
    vec2 b = mix(vector4, vector3, point.x);
    vec2 c = mix(a, b, point.y);

    return c.xy;

}

vec3 interpolate3(vec2 point, vec3 vector1, vec3 vector2, vec3 vector3, vec3 vector4) {

    vec3 a = mix(vector1, vector2, point.x);
    vec3 b = mix(vector4, vector3, point.x);
    vec3 c = mix(a, b, point.y);

    return c.xyz;

}

vec4 interpolate4(vec2 point, vec4 vector1, vec4 vector2, vec4 vector3, vec4 vector4) {

    vec4 a = mix(vector1, vector2, point.x);
    vec4 b = mix(vector4, vector3, point.x);
    vec4 c = mix(a, b, point.y);

    return c.xyzw;

}

