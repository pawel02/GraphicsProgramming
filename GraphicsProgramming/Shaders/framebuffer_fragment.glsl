#version 420
out vec4 FragColor;

uniform sampler2D tex;
in vec2 TexCoords;

void main()
{
	FragColor = vec4(vec3(1 - texture(tex, TexCoords)),1.0);
}