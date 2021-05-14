#version 420
out vec4 FragColor;

uniform sampler2D tex;
in vec2 TexCoords;

void main()
{
	FragColor = texture(tex, TexCoords);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
}