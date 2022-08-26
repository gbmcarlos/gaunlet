#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];

flat in uint v_entityIndexVertex[];

flat out uint v_entityIndex;
out vec3 v_barycentricCoordinates;

void main() {

    gl_Position = gl_in[0].gl_Position;
    v_entityIndex = v_entityIndexVertex[0];
    v_barycentricCoordinates = vec3(1, 0, 0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    v_entityIndex = v_entityIndexVertex[1];
    v_barycentricCoordinates = vec3(0, 1, 0);
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    v_entityIndex = v_entityIndexVertex[2];
    v_barycentricCoordinates = vec3(0, 0, 1);
    EmitVertex();

    EndPrimitive();

}
