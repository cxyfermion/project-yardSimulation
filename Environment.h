#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <vector>
#include <imgui.h>
#include <glm/glm.hpp>
#include "res/GL_heads/Shader.h"
#include "res/GL_heads/Camera.hpp"

/*
大雨、雾天、大风天气暂停作业
6h和18h整时光源色最接近黄色，前后1h变化区间。雾天、大雨、阴天天气下无光源颜色变化、
6时时太阳的位置是最右端，18时太阳的位置是最左端，12时太阳在背景板正前方
sin(pi/2) = 1 sin(pi) = 0 sin(3pi/2) = -1 sin(2pi) = 0
cos(pi/2) = 0 cos(pi) = -1 cos(3pi/2) = 0 cos(2pi) = 1
6时x为1	12时x为0		18时x为-1		23时x为0
x = sin(((hour+minute/60.0)/12.0) * M_PI) * radius
6时z为0		12时z为1		18时z为0		23时z为-1
z = -cos(((hour+minute/60.0)/12.0) * M_PI) * radius

*/

class Environment
{
public:
	Environment();
	void updateEnv();
	void env_dispatch(int hours, int minutes, bool authority, Camera& camera, Shader& woodShader, Shader& glassShader, Shader& skyboxShader);
	void createEnv(Shader& woodShader, Shader& glassShader, Shader& skyboxShader);
	void processInput(Camera& camera, GLFWwindow* window);
	int weather;				//天气，0为晴天，1为阴天，2为下雨，3为大雨，4为大雾霾，5为大风，6为下雪，7为沙尘暴，8雷暴

private:
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 lightPos;
	float deltaTime;
	float lastFrame;
	float circle_radius;					//模拟太阳的移动半径
	const char* textures[8];
	unsigned int cubeVAO, cubeVBO;
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int woodTexture;
	unsigned int marbleTexture;
	unsigned int brickTexture;
	unsigned int stoneTexture;
	unsigned int goldTexture;
	unsigned int cubemapTexture;
	void drawEnv(int hour, int minute, Camera& camera, Shader& woodShader, Shader& glassShader, Shader& skyboxShader, 
		bool background, bool skybox, int texture_type, bool light, bool spotlight,
		float material_specular[3], float material_shininess, float spotLight_ambient[3], float spotLight_diffuse[3], float spotLight_specular[3],
		float spotLight_cutOff, float spotLight_outerCutOff, float spotLight_direction[3], float dirLight_direction[3], float dirLight_ambient[3],
		float dirLight_diffuse[3], float dirLight_specular[3], float pointLights_ambient[3], float pointLights_diffuse[3], float pointLights_specular[3],
		float pointLights_position[3]);
	unsigned int load_texture(const char* texture_path);
	unsigned int loadCubemap(std::vector<std::string>& faces);

};
