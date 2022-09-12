#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];

// Inputs
in vec2 v_textureCoordinatesVertex[];
in vec3 v_normalVertex[];
flat in uint v_entityIndexVertex[];

// Ouputs
out vec2 v_textureCoordinates;
out vec3 v_normal;
flat out uint v_entityIndex;
out vec3 v_barycentricCoordinates;

void main() {

    gl_Position = gl_in[0].gl_Position;
    v_textureCoordinates = v_textureCoordinatesVertex[0];
    v_normal = v_normalVertex[0];
    v_entityIndex = v_entityIndexVertex[0];
    v_barycentricCoordinates = vec3(1, 0, 0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    v_textureCoordinates = v_textureCoordinatesVertex[1];
    v_normal = v_normalVertex[1];
    v_entityIndex = v_entityIndexVertex[1];
    v_barycentricCoordinates = vec3(0, 1, 0);
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    v_textureCoordinates = v_textureCoordinatesVertex[2];
    v_normal = v_normalVertex[2];
    v_entityIndex = v_entityIndexVertex[2];
    v_barycentricCoordinates = vec3(0, 0, 1);
    EmitVertex();

    EndPrimitive();

}
