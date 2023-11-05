#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;

out vec3 fragColor;

void main() {
	fragColor = color;
    gl_PointSize = 15;
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
}