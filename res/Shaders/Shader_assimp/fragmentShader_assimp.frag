#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
};
uniform Material material;

struct Light
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;
uniform vec3 viewPos;

float quadratic = 0.032f;
float linear = 0.092f;
float constant = 1.0f;

float cutOff = cos(radians(12.5f));
float outerCutOff = cos(radians(17.5f));

//环境光方向
vec3 direction = vec3(-0.2f, -1.0f, -0.5f);

void main()
{
	/*以下为主动光源（有源光或手电筒）*/
	//ambient
	vec3 ambient = light.ambient * texture(material.texture_diffuse, TexCoords).rgb;
	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = light.diffuse * (diff * texture(material.texture_diffuse, TexCoords).rgb);
	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64.0f);
	vec3 specular = light.specular * spec * texture(material.texture_specular, TexCoords).rgb;
	//intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = cutOff - outerCutOff;
	float intensity = clamp((theta - outerCutOff) / epsilon, 0.0f, 1.0f);
	//Attenuation
	float Distance = length(light.position - FragPos);
	float attenuation = 1.0f / (constant + linear * Distance + quadratic * pow(Distance, 2));
	 ambient *= attenuation * intensity;
	 diffuse *= attenuation * intensity;
	 specular *= attenuation * intensity;
	
	/*以下为无源光（环境光照）*/
	vec3 ambient2 = light.ambient * texture(material.texture_diffuse, TexCoords).rgb;
	vec3 lightDir2 = normalize(-direction);
	float diff2 = max(dot(lightDir2, norm), 0.0f);
	//vec3 diffuse2 = light.diffuse * (diff2 * texture(material.texture_diffuse, TexCoords).rgb);
	vec3 diffuse2 = light.diffuse * (diff * texture(material.texture_diffuse, TexCoords).rgb);
	vec3 reflectDir2 = reflect(-lightDir2, norm);
	//float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0f), 64.0f);
	float spec2 = pow(max(dot(viewDir, reflectDir), 0.0f), 64.0f);
	vec3 specular2 = light.specular * spec2 * texture(material.texture_specular, TexCoords).rgb;
	//complie
	vec3 result = ambient + diffuse + specular + ambient2 + diffuse2 + specular2;
	FragColor = vec4(result, 1.0f);
	//FragColor = vec4(result * attenuation * intensity, 1.0f);
	//FragColor = texture(material.texture_diffuse, TexCoords);
}
