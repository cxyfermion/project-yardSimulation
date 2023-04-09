#version 330 core

out vec4 FragColor;
in vec3 ourColor;
uniform vec3 colors;
uniform float alpha;
in vec2 TexCoords;
uniform sampler2D texture_1;
uniform sampler2D texture_2;
//光照部分变量
//uniform vec3 lightColor;
//uniform vec3 objectColor;
//uniform vec3 lightPos;
uniform vec3 viewPos;
//uniform float shininess;
in vec3 normal;
in vec3 fragPos;

//材质的参数结构体
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

//light struct
struct Light{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};
uniform Light light;



void main()
{
	//FragColor = mix(texture(texture_1, 1 * TexCoords), texture(texture_2, 3 * TexCoords) * vec4(ourColor * colors, 1.0f), alpha);
	//FragColor = vec4(0.4f, 0.9f, 1.0f, 1.0f);

	/*Ambient*/
	//float ambientStrength = 0.4f;
	//vec3 ambient = lightColor * ambientStrength;
	///vec3 ambient = lightColor * material.ambient;
	vec3 ambient = light.ambient * material.ambient;

	/*Diffuse*/
	vec3 norm = normalize(normal);
	//vec3 lightDirection = normalize(lightPos - fragPos);
	vec3 lightDirection = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDirection), 0.0f);
	//vec3 diffuse = lightColor * diff;
	//vec3 diffuse = lightColor * (diff * material.diffuse);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	/*Specular & shininess*/
	//float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDirection, norm);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	//vec3 specular = lightColor * spec * specularStrength;
	vec3 specular = light.specular * (spec * material.specular);

	//vec3 result = (ambient + diffuse + specular) * objectColor;
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}