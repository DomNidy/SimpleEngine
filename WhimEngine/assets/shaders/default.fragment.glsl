#version 330 core
in vec3 fragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

out vec4 FragColor;


void main()
{
    FragColor = texture(ourTexture, TexCoord) + vec4(fragColor.rgb, 1);
//	FragColor = vec4(vec3(gl_FragCoord.z), 1.0); (Use to visualize depth buffer, colors get darker as z is lower)
} 