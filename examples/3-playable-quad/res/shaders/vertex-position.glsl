#version 410 core

layout(location = 0) in vec4 position;
uniform mat4 u_viewProjection;

void main() {
   gl_Position = u_viewProjection * position;
}