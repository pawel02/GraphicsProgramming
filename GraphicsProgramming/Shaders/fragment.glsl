#version 420
out vec4 FragColor;

uniform sampler2D _tex1;
uniform sampler2D _tex2;

in vec2 TexCoord;

uniform float blend_factor;
void main()
{
	FragColor = mix(texture(_tex1, TexCoord), texture(_tex2, TexCoord), blend_factor);
}