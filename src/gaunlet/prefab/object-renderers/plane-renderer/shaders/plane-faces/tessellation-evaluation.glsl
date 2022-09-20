#version 410 core

layout (quads, equal_spacing, ccw) in;

struct EntityPropertySet {
    uint textureIndex;
    float leftSizeFactor;
    float rightSizeFactor;
    float bottomSizeFactor;
    float topSizeFactor;
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

// Textures
uniform sampler2D heightmap;


vec2 interpolate2(vec2 point, vec2 vector1, vec2 vector2, vec2 vector3, vec2 vector4);
vec3 interpolate3(vec2 point, vec3 vector1, vec3 vector2, vec3 vector3, vec3 vector4);
vec4 interpolate4(vec2 point, vec4 vector1, vec4 vector2, vec4 vector3, vec4 vector4);

void main() {

    te_textureCoordinates = interpolate2(gl_TessCoord.xy, tc_textureCoordinates[0], tc_textureCoordinates[1], tc_textureCoordinates[2], tc_textureCoordinates[3]);
    te_normal = interpolate3(gl_TessCoord.xy, tc_normal[0], tc_normal[1], tc_normal[2], tc_normal[3]);

    vec4 vertexPosition = interpolate4(gl_TessCoord.xy, gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position);
    float height = texture(heightmap, te_textureCoordinates).y * u_maxHeight;
    vertexPosition += vec4(vec3(0, 1, 0) * height, 0);

    gl_Position = projection * view * vertexPosition;

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

