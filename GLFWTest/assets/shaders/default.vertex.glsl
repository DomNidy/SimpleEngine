#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) uniform mat3x3 u_Rot;

out vec3 fragColor;

void main() {
  fragColor = color;
  gl_PointSize = 15;
  gl_Position = vec4(u_Rot * aPos, 1.0f);
}
