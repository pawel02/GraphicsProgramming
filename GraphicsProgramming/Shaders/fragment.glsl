#version 420
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

//control the intensities for the lightColor
struct Light
{
	vec3 Pos;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float constant;
	float linear;
	float quadratic;
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
	vec3 lightDir = normalize(light.Pos - FragPos);  
	
	vec3 norm = normalize(Normal);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.Diffuse * diff * vec3(texture(material.tex, TexCoords));
	float specularStrength = 0.5;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	vec3 specular = light.Specular * spec * vec3(texture(material.Specular, TexCoords));  

	// spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

	//attenuation
	float dist = length(light.Pos - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = diffuse + specular;
	FragColor = vec4(result, 1.0);
}