#version 330 core
in vec3 fragColor;
in vec2 TexCoord;

uniform float Time;
uniform sampler2D ourTexture;

out vec4 FragColor;


void main()
{
    FragColor = vec4(fragColor, 1.0f);
} 