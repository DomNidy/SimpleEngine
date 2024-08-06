#version 330 core
in vec3 fragColor;
in vec2 TexCoord;

uniform float Time;
uniform sampler2D ourTexture;

out vec4 FragColor;


void main()
{
    FragColor = texture(ourTexture, TexCoord) + vec4(fragColor.rgb, 1);
} 