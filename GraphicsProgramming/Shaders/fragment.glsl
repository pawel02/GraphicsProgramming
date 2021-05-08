#version 420
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

//control the intensities for the lightColor
struct Light
{
	vec3 Pos;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

struct Material 
{
	sampler2D tex;
	sampler2D Specular;

	sampler2D Emission;

	float Shininess;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

uniform float time;


void main()
{
	vec3 ambient = light.Ambient * vec3(texture(material.tex, TexCoords));

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.Pos - FragPos);  
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.Diffuse * diff * vec3(texture(material.tex, TexCoords));

	float specularStrength = 0.5;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	vec3 specular = light.Specular * spec * vec3(texture(material.Specular, TexCoords));  

	vec3 emission = texture(material.Emission, TexCoords + vec2(0, time)).rgb;

	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);
}