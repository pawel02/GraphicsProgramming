#version 420
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

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
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
};

uniform Material material;
uniform Light light;

uniform float blend_factor;
uniform vec3 viewPos;


void main()
{
	vec3 ambient = light.Ambient * material.Ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.Pos - FragPos);  
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.Diffuse * (diff * material.Diffuse);

	float specularStrength = 0.5;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);

	vec3 specular = light.Specular * (material.Specular * spec);  

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}