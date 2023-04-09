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
	//大多数情况下ambient和diffuse是相等的，所以只定义一个
	//这个结构体中的diffuse与原先模型中的texture_0是同内容的，为贴图材质
	sampler2D diffuse;
	vec3 specular;
	//将specular的类型由vec3转为sampler2D，让openGL自行计算不同的材质的不同反光值（例如实木与金属之间的）
	sampler2D specular_diff;
	float shininess;
};
uniform Material material;

//light struct
struct Light{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 direction;

	//点光源与片段的效果大致为F = 1.0 / (Kc + Kl * d + Kq * d^2)
	//式内含有二次项，线性项与常项，以下面三个变量表示
	//计算效果为与光源投射点近的亮度高，远处亮度低
	float quadratic;
	float linear;
	float constant;

	//聚光——Φ
	float cutOff;
	float outerCutOff;
};
uniform Light light;

uniform bool light_mode;
uniform bool diffuse_reflection;

void main()
{
	//FragColor = mix(texture(texture_1, 1 * TexCoords), texture(texture_2, 3 * TexCoords) * vec4(ourColor * colors, 1.0f), alpha);

	if (light_mode)	//聚光模式
	{
		//vec3 lightDirection = normalize(light.position - fragPos);
		//float theta = dot(lightDirection, normalize(-light.direction));
		//if (theta > light.cutOff)
		//{
		/*Ambient*/
		//vec3 ambient = light.ambient * material.ambient;
		vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

		/*Diffuse*/
		vec3 norm = normalize(normal);
		vec3 lightDirection = normalize(light.position - fragPos);
		//vec3 lightDirection = normalize(-light.direction);
		float diff = max(dot(norm, lightDirection), 0.0f);
		//vec3 diffuse = light.diffuse * (diff * material.diffuse);
		vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

		/*Specular & shininess*/
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDirection, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular;
		if (diffuse_reflection)
		{
			specular = light.specular * (spec * material.specular);
		}
		else
		{
			specular = light.specular * spec * texture(material.specular_diff, TexCoords).rgb;
		}

		//聚光边缘平滑
		//I = (θ - γ) / ε
		float theta = dot(lightDirection, normalize(-light.direction));
		float epsilon = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

		diffuse *= intensity;
		specular *= intensity;

		/*Attenuation*/
		float Distance = length(light.position - fragPos);
		float attenuation = 1.0f / (light.constant + light.linear * Distance + light.quadratic * pow(Distance, 2));

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0f);
		//}
		//else		//do spot light
		//{
		//	FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0f);
		//}
	}
	else
	{
		/*Ambient*/
		//vec3 ambient = light.ambient * material.ambient;
		vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

		/*Diffuse*/
		vec3 norm = normalize(normal);
		vec3 lightDirection = normalize(light.position - fragPos);
		//vec3 lightDirection = normalize(-light.direction);
		float diff = max(dot(norm, lightDirection), 0.0f);
		//vec3 diffuse = light.diffuse * (diff * material.diffuse);
		vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

		/*Specular & shininess*/
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDirection, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular;
		if (diffuse_reflection)
		{
			specular = light.specular * (spec * material.specular);
		}
		else
		{
			specular = light.specular * spec * texture(material.specular_diff, TexCoords).rgb;
		}
		/*Attenuation*/
		float Distance = length(light.position - fragPos);
		float attenuation = 1.0f / (light.constant + light.linear * Distance + light.quadratic * pow(Distance, 2));

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0f);
	}
}