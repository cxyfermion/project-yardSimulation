#version 330 core

out vec4 FragColor;
in vec3 ourColor;
uniform vec3 colors;
uniform float alpha;
in vec2 TexCoords;
uniform sampler2D texture_1;
uniform sampler2D texture_2;
//���ղ��ֱ���
uniform vec3 viewPos;
in vec3 normal;
in vec3 fragPos;

//���ʵĲ����ṹ��
struct Material {
	//vec3 ambient;
	//vec3 diffuse;
	//����������ambient��diffuse����ȵģ�����ֻ����һ��
	//����ṹ���е�diffuse��ԭ��ģ���е�texture_0��ͬ���ݵģ�Ϊ��ͼ����
	sampler2D diffuse;
	vec3 specular;
	//��specular��������vec3תΪsampler2D����openGL���м��㲻ͬ�Ĳ��ʵĲ�ͬ����ֵ������ʵľ�����֮��ģ�
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

	//���Դ��Ƭ�ε�Ч������ΪF = 1.0 / (Kc + Kl * d + Kq * d^2)
	//ʽ�ں��ж�����������볣�����������������ʾ
	//����Ч��Ϊ���ԴͶ���������ȸߣ�Զ�����ȵ�
	float quadratic;
	float linear;
	float constant;

	//�۹⡪����
	float cutOff;
	float outerCutOff;
};
uniform Light light;

uniform bool light_mode;
uniform bool diffuse_reflection;

void main()
{
	//FragColor = mix(texture(texture_1, 1 * TexCoords), texture(texture_2, 3 * TexCoords) * vec4(ourColor * colors, 1.0f), alpha);

	if (light_mode)	//�۹�ģʽ
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

		//�۹��Եƽ��
		//I = (�� - ��) / ��
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