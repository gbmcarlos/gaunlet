#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];

// Inputs
in vec2 v_textureCoordinates[];
in vec3 v_normal[];
flat in uint v_entityIndex[];

// Ouputs
out vec2 g_textureCoordinates;
out vec3 g_normal;
flat out uint g_entityIndex;
out vec3 g_barycentricCoordinates;

void main() {

    gl_Position = gl_in[0].gl_Position;
    g_textureCoordinates = v_textureCoordinates[0];
    g_normal = v_normal[0];
    g_entityIndex = v_entityIndex[0];
    g_barycentricCoordinates = vec3(1, 0, 0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    g_textureCoordinates = v_textureCoordinates[1];
    g_normal = v_normal[1];
    g_entityIndex = v_entityIndex[1];
    g_barycentricCoordinates = vec3(0, 1, 0);
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    g_textureCoordinates = v_textureCoordinates[2];
    g_normal = v_normal[2];
    g_entityIndex = v_entityIndex[2];
    g_barycentricCoordinates = vec3(0, 0, 1);
    EmitVertex();

    EndPrimitive();

}
