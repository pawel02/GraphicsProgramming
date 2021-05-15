#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform samplerCube skybox;

void main()
{    
//    vec3 I = normalize(FragPos - viewPos);
//    vec3 R = reflect(I, normalize(Normal));
//    FragColor = vec4(mix(texture(texture_diffuse1, TexCoords).rgb, texture(skybox, R).rgb, 0.5),1.0);

    float ratio = 1 / 1.33;
	vec3 I = normalize(FragPos - viewPos);
	vec3 R = refract(I, normalize(Normal), ratio);
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}