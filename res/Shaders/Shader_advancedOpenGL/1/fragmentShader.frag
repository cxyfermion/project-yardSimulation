#version 330 core

out vec4 FragColor;
in vec3 ourColor;
uniform vec3 colors;
uniform float alpha;
in vec2 TexCoords;
uniform sampler2D texture_1;
uniform sampler2D texture_2;
//光照部分变量
uniform vec3 viewPos;
in vec3 normal;
in vec3 fragPos;

//材质的参数结构体
struct Material {
	//vec3 ambient;
	//vec3 diffuse;
	sampler2D diffuse;
	//vec3 specular;
	sampler2D specular;
	float shininess;
};
uniform Material material;

//directional light
struct DirLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//vec3 position;
	vec3 direction;

	//float quadratic;
	//float linear;
	//float constant;

	//float cutOff;
	//float outerCutOff;
};

//Point Light
struct PointLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
	//vec3 direction;

	float quadratic;
	float linear;
	float constant;

	//float cutOff;
	//float outerCutOff;
};

//spot light
struct SpotLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
	vec3 direction;

	float quadratic;
	float linear;
	float constant;

	float cutOff;
	float outerCutOff;
};

uniform DirLight dirLight;
//创建多个光源
uniform PointLight pointLights[4];
uniform SpotLight spotLight;

//directional light function
vec3 calDirLight(DirLight light, vec3 norm, vec3 viewDir);
//point light function
vec3 calPointLight(PointLight light, vec3 norm, vec3 fragpos, vec3 viewDir);
//spot light function
vec3 calSpotLight(SpotLight light, vec3 norm, vec3 fragpos, vec3 viewDir);

void main()
{
	//properties
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);

	//Directional Light
	vec3 result = calDirLight(dirLight, norm, viewDir);
	//Point Light
	for (int i = 0; i < 4; i++)
	{
		result += calPointLight(pointLights[i], norm, fragPos, viewDir);
	}
	//Spot Light
	result += calSpotLight(spotLight, norm, fragPos, viewDir);

	FragColor = vec4(result, 1.0f);
}

vec3 calDirLight(DirLight light, vec3 norm, vec3 viewDir)
{
	vec3 lightDirection = normalize(-light.direction);
	//vec3 lightDirection = normalize(light.position - fragPos);

	//diffuse shading
	float diff = max(dot(norm, lightDirection), 0.0f);
	
	//specular shading
	vec3 reflectDir = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	//combine results
	//Ambient
	//vec3 ambient = light.ambient * material.ambient;
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	//Diffuse
	//vec3 diffuse = light.diffuse * (diff * material.diffuse);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);
	//Specular & shininess
	//vec3 specular = light.specular * (spec * material.specular);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	return ambient + diffuse + specular;
}

vec3 calPointLight(PointLight light, vec3 norm, vec3 fragpos, vec3 viewDir)
{
	//vec3 lightDirection = normalize(-light.direction);
	vec3 lightDirection = normalize(light.position - fragPos);

	//diffuse shading
	float diff = max(dot(norm, lightDirection), 0.0f);
	
	//specular shading
	vec3 reflectDir = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	//combine results
	//Ambient
	//vec3 ambient = light.ambient * material.ambient;
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	//Diffuse
	//vec3 diffuse = light.diffuse * (diff * material.diffuse);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);
	//Specular & shininess
	//vec3 specular = light.specular * (spec * material.specular);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	//Attenuation
	float Distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * Distance + light.quadratic * pow(Distance, 2));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 calSpotLight(SpotLight light, vec3 norm, vec3 fragpos, vec3 viewDir)
{
	//vec3 lightDirection = normalize(-light.direction);
	vec3 lightDirection = normalize(light.position - fragPos);

	//diffuse shading
	float diff = max(dot(norm, lightDirection), 0.0f);
	
	//specular shading
	vec3 reflectDir = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	//聚光边缘平滑
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	//combine results
	//Ambient
	//vec3 ambient = light.ambient * material.ambient;
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	//Diffuse
	//vec3 diffuse = light.diffuse * (diff * material.diffuse);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);
	//Specular & shininess
	//vec3 specular = light.specular * (spec * material.specular);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	//Attenuation
	float Distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * Distance + light.quadratic * pow(Distance, 2));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}

/*
#version 330 core
out vec4 FragColor;
float near = 0.1;
float far  = 100.0;
float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC
	return (2.0 * near * far) / (far + near - z * (far - near));
}
void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
	FragColor = vec4(vec3(depth), 1.0);
}
*/