#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) uniform mat3x3 u_Rot = mat3x3(1.0); // Default identity matrix (if we dont set a transform this will just be matrix of 0s, breaking rendering)

out vec3 fragColor;
out vec2 TexCoord;


void main() {
  TexCoord = aTexCoord;
  fragColor = aColor;
  gl_PointSize = 15;
  gl_Position = vec4(u_Rot * aPos, 1.0f) ;
}
