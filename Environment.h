#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <vector>
#include <imgui.h>
#include <glm/glm.hpp>
#include "res/GL_heads/Shader.h"
#include "res/GL_heads/Camera.hpp"

/*
���ꡢ���졢���������ͣ��ҵ
6h��18h��ʱ��Դɫ��ӽ���ɫ��ǰ��1h�仯���䡣���졢���ꡢ�����������޹�Դ��ɫ�仯��
6ʱʱ̫����λ�������Ҷˣ�18ʱ̫����λ��������ˣ�12ʱ̫���ڱ�������ǰ��
sin(pi/2) = 1 sin(pi) = 0 sin(3pi/2) = -1 sin(2pi) = 0
cos(pi/2) = 0 cos(pi) = -1 cos(3pi/2) = 0 cos(2pi) = 1
6ʱxΪ1	12ʱxΪ0		18ʱxΪ-1		23ʱxΪ0
x = sin(((hour+minute/60.0)/12.0) * M_PI) * radius
6ʱzΪ0		12ʱzΪ1		18ʱzΪ0		23ʱzΪ-1
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
	int weather;				//������0Ϊ���죬1Ϊ���죬2Ϊ���꣬3Ϊ���꣬4Ϊ��������5Ϊ��磬6Ϊ��ѩ��7Ϊɳ������8�ױ�

private:
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 lightPos;
	float deltaTime;
	float lastFrame;
	float circle_radius;					//ģ��̫�����ƶ��뾶
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
