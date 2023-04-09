#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;
in vec3 fragPos;

uniform vec3 viewPos;
uniform float texture_state;
uniform bool light_switch;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;

//材质的参数结构体
struct Material 
{
	//采样结果由用户控制
	//sampler2D diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

//directional light
struct DirLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};
uniform DirLight dirLight;

//Point Light
struct PointLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	float quadratic;
	float linear;
	float constant;
};
//创建多个光源
uniform PointLight pointLights[1];

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
uniform SpotLight spotLight;

//directional light function
vec3 calDirLight(DirLight light, vec3 norm, vec3 viewDir, vec4 texture_use);
//point light function
vec3 calPointLight(PointLight light, vec3 norm, vec3 fragpos, vec3 viewDir, vec4 texture_use);
//spot light function
vec3 calSpotLight(SpotLight light, vec3 norm, vec3 fragpos, vec3 viewDir, vec4 texture_use);

void main()
{   
	//材质选择
	vec4 texture_use;
    texture_use = vec4(1.0f) * step(texture_state, 0.1);
    texture_use += texture(texture1, TexCoords) * step(0.9, texture_state) * step(texture_state, 1.1);
    texture_use += texture(texture2, TexCoords) * step(1.9, texture_state) * step(texture_state, 2.1);
    texture_use += texture(texture3, TexCoords) * step(2.9, texture_state) * step(texture_state, 3.1);
    texture_use += texture(texture4, TexCoords) * step(3.9, texture_state) * step(texture_state, 4.1);
    texture_use += texture(texture5, TexCoords) * step(4.9, texture_state);
	
	//光照计算
	if (light_switch)
	{
		FragColor = texture_use;
	}
	else
	{
		//properties
		vec3 norm = normalize(Normal);
		vec3 viewDir = normalize(viewPos - fragPos);
		//Directional Light
		vec3 result = calDirLight(dirLight, norm, viewDir, texture_use);
		//Point Light
		for (int i = 0; i < 1; i++)
		{
			result += calPointLight(pointLights[i], norm, fragPos, viewDir, texture_use);
		}
		//Spot Light
		result += calSpotLight(spotLight, norm, fragPos, viewDir, texture_use);
		FragColor = vec4(result, 1.0f);
	}
}

vec3 calDirLight(DirLight light, vec3 norm, vec3 viewDir, vec4 texture_use)
{
	//点光源与平行光切换
	vec3 lightDirection = normalize(-light.direction);
	//vec3 lightDirection = normalize(light.position - fragPos);

	//diffuse shading
	float diff = max(dot(norm, lightDirection), 0.0f);
	
	//specular shading
	vec3 reflectDir = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	//combine results
	/*Ambient*/
	vec3 ambient = light.ambient * texture_use.rgb;
	/*Diffuse*/
	vec3 diffuse = light.diffuse * (diff * texture_use.rgb);
	/*Specular & shininess*/
	vec3 specular = light.specular * (spec * material.specular);
	//vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	return ambient + diffuse + specular;
}

vec3 calPointLight(PointLight light, vec3 norm, vec3 fragpos, vec3 viewDir, vec4 texture_use)
{
	//点光源与平行光切换
	//vec3 lightDirection = normalize(-light.direction);
	vec3 lightDirection = normalize(light.position - fragPos);

	//diffuse shading
	float diff = max(dot(norm, lightDirection), 0.0f);
	
	//specular shading
	vec3 reflectDir = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	//combine results
	/*Ambient*/
	vec3 ambient = light.ambient * texture_use.rgb;
	/*Diffuse*/
	vec3 diffuse = light.diffuse * (diff * texture_use.rgb);
	/*Specular & shininess*/
	vec3 specular = light.specular * (spec * material.specular);
	//vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	//Attenuation
	float Distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * Distance + light.quadratic * pow(Distance, 2));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 calSpotLight(SpotLight light, vec3 norm, vec3 fragpos, vec3 viewDir, vec4 texture_use)
{
	//点光源与平行光切换
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
	/*Ambient*/
	vec3 ambient = light.ambient * texture_use.rgb;
	/*Diffuse*/
	vec3 diffuse = light.diffuse * (diff * texture_use.rgb);
	/*Specular & shininess*/
	vec3 specular = light.specular * (spec * material.specular);
	//vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	//Attenuation
	float Distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * Distance + light.quadratic * pow(Distance, 2));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}
