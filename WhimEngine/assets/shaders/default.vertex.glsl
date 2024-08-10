#version 330 core

// Input vertex data, different for all executions of this shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

// Output to next stage in render pipeline
out vec3 fragColor;
out vec2 texCoord;

// Constant throughout a single render pass
uniform mat4 mvp;

void main() {
  // Output position of the vertex, in clip space: mvp * position
  gl_Position = mvp * vec4(aPos, 1.0f);
  texCoord = aTexCoord;
  fragColor = aColor;
}
