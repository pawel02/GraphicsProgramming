#version 420
out vec4 FragColor;

uniform sampler2D tex;
in vec2 TexCoords;

void main()
{

	FragColor = texture(tex, TexCoords);
}