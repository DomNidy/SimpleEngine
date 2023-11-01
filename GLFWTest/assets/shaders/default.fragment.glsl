#version 330 core
out vec4 FragColor;
uniform float Time;

void main()
{
    FragColor = vec4(Time, 0.2f, 0.2f, 1.0f);
} 