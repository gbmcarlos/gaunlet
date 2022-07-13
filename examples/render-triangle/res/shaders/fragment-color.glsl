#version 410 core

layout(location = 0) out vec4 color;
uniform vec3 u_color;

void main() {
   color = vec4(u_color.x, u_color.y, u_color.z, 1.0);
}