#version 410 core

layout (vertices = 4) out;

in vec2 v_textureCoordinates[];
in vec3 v_normal[];
flat in uint v_entityIndex[];

out vec2 tc_textureCoordinates[];
out vec3 tc_normal[];
flat out uint tc_entityIndex[];

void main() {

    if (gl_InvocationID == 0) {
        gl_TessLevelOuter[0] = 4;
        gl_TessLevelOuter[1] = 4;
        gl_TessLevelOuter[2] = 4;
        gl_TessLevelOuter[3] = 4;

        gl_TessLevelInner[0] = 4;
        gl_TessLevelInner[1] = 4;
    }

    tc_textureCoordinates[gl_InvocationID] = v_textureCoordinates[gl_InvocationID];
    tc_entityIndex[gl_InvocationID] = v_entityIndex[gl_InvocationID];
    tc_normal[gl_InvocationID] = v_normal[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

}
