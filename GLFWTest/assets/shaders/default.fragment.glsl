#version 330 core
in vec3 fragColor;
uniform float Time;

out vec4 FragColor;

void main()
{
    FragColor = vec4(fragColor.x, fragColor.y, fragColor.z, 1.0f);
} 