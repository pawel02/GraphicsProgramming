#version 420
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D _tex1;
uniform sampler2D _tex2;

uniform float blend_factor;
void main()
{
	FragColor = mix(texture(_tex1, TexCoord), texture(_tex2, TexCoord), blend_factor);
}