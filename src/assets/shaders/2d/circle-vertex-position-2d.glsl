#version 410 core

// Shader uniform
uniform mat4 u_viewProjection;

// Vertex attributes
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_localCoordinates;
layout(location = 2) in float a_thickness;
layout(location = 3) in float a_fade;
layout(location = 4) in vec2 a_textureCoordinates;
layout(location = 5) in int a_textureIndex;
layout(location = 6) in vec4 a_color;

// Outputs
out vec2 v_localCoordinates;
flat out float v_thickness;
flat out float v_fade;
out vec2 v_textureCoordinates;
flat out int v_textureIndex;
out vec4 v_color;

void main() {

   v_localCoordinates = a_localCoordinates;
   v_thickness = a_thickness;
   v_fade = a_fade;
   v_textureCoordinates = a_textureCoordinates;
   v_textureIndex  = a_textureIndex;
   v_color = a_color;

   // position value
   gl_Position = u_viewProjection * a_position;

}