#include "Environment.h"
#include "stb_image.h"
#include "data.h"

#define SCR_WIDTH 1440
#define SCR_HEIGHT 810

Environment::Environment()
{
	const char* temp[] = { u8"��", u8"��ľ", u8"����ʯ", u8"שǽ" , u8"ʯǽ" , u8"��", u8"����", u8"����" };
	for (int i = 0; i < 8; i++)
	{
		this->textures[i] = temp[i];
	}
	this->model = glm::mat4(1.0f);
	this->view = glm::mat4(1.0f);
	this->projection = glm::mat4(1.0f);
	this->deltaTime = 0.0f;
	this->lastFrame = 0.0f;
	this->lightPos = glm::vec3(1.2f, 1.3f, -1.5f);
	this->weather = 0;
	this->circle_radius = 6.0f;
}

void Environment::updateEnv()
{
	float timp_temp = (float)glfwGetTime();
	this->deltaTime = timp_temp - this->lastFrame;
	this->lastFrame = timp_temp;
}

void Environment::env_dispatch(int hours, int minutes, bool authority, Camera& camera, Shader& woodShader, Shader& glassShader, Shader& skyboxShader)
{
	static bool skybox = false;			//��պп���
	static bool background = true;		//��������
	static int texture_act = 0;			//����Ĳ���
	static int time_mode = 1;			//ʱ��ӳ��ģʽ��0Ϊû��ӳ�䣬1Ϊʹ�÷���ʱ��ӳ�䣬2Ϊ����ָ��ӳ��
	static int minute_local = 0;		//���ط���
	static int hour_local = 12;			//����Сʱ
	//��Դ����
	static bool light = true;			//���տ���
	static bool spotlight = false;		//�۹�ģʽ
	//����
	static float material_specular[3] = { 0.5f, 0.5f, 0.5f };
	static float material_shininess = { 64.0f };
	//�۹�
	static float spotLight_ambient[3] = { 0.2f, 0.2f, 0.2f };
	static float spotLight_diffuse[3] = { 0.5f, 0.5f, 0.5f };
	static float spotLight_specular[3] = { 1.0f, 1.0f, 1.0f };
	static float spotLight_cutOff = { 10.0f };
	static float spotLight_outerCutOff = { 17.0f };
	static float spotLight_direction[3] = { -0.2f, -1.0f, -0.4f };
	//�����
	static float dirLight_direction[3] = { -0.3f, -1.0f, -4.0f };
	static float dirLight_ambient[3] = { 0.08f, 0.08f, 0.08f };
	static float dirLight_diffuse[3] = { 0.2f, 0.2f, 0.2f };
	static float dirLight_specular[3] = { 0.2f, 0.2f, 0.2f };
	//���Դ
	static float pointLights_ambient[3] = { 0.2f, 0.2f, 0.2f };
	static float pointLights_diffuse[3] = { 0.4f, 0.4f, 0.4f };
	static float pointLights_specular[3] = { 0.9f, 0.9f, 0.9f };
	static float pointLights_position[3] = { 0.0f, 0.0f, 0.0f };
	if (authority)
	{
		if (ImGui::CollapsingHeader(u8"��������"))
		{
			ImGui::Indent();
			ImGui::Checkbox(u8"����ǽ����", &background);
			ImGui::SameLine();
			ImGui::Checkbox(u8"��պп���", &skybox);
			ImGui::Text(u8"ѡ��������");
			ImGui::SameLine();
			ImGui::RadioButton(u8"����", &this->weather, 0);
			ImGui::SameLine();
			ImGui::RadioButton(u8"����", &this->weather, 1);
			ImGui::SameLine();
			ImGui::RadioButton(u8"����", &this->weather, 2);
			ImGui::SameLine();
			ImGui::RadioButton(u8"����", &this->weather, 3);
			ImGui::SameLine();
			ImGui::RadioButton(u8"������", &this->weather, 4);
			ImGui::SameLine();
			ImGui::RadioButton(u8"���", &this->weather, 5);
			ImGui::SameLine();
			ImGui::RadioButton(u8"��ѩ", &this->weather, 6);
			ImGui::SameLine();
			ImGui::RadioButton(u8"ɳ����", &this->weather, 7);
			ImGui::SameLine();
			ImGui::RadioButton(u8"�ױ�", &this->weather, 8);
			if (background)
			{
				ImGui::Text(u8"���ñ���ǽ���ʣ�");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100.0f);
				ImGui::Combo("  ", &texture_act, this->textures, IM_ARRAYSIZE(this->textures));
				//ImGui::ListBox("  ", &texture_act, textures, IM_ARRAYSIZE(textures), 4);
				//ImGui::PushItemWidth(100.0f);
				if (texture_act < 6)
				{
					ImGui::SameLine();
					ImGui::Checkbox(u8"���տ���", &light);
					if (light)
					{
						ImGui::Text(u8"ѡ��ʱ��ӳ��ģʽ��");
						ImGui::SameLine();
						ImGui::RadioButton(u8"��ʱ��ӳ��", &time_mode, 0);
						ImGui::SameLine();
						ImGui::RadioButton(u8"ʹ�÷���ʱ��ӳ��", &time_mode, 1);
						ImGui::SameLine();
						ImGui::RadioButton(u8"ʹ�ñ���ʱ��ӳ��", &time_mode, 2);
						if (time_mode == 2)
						{
							ImGui::Text(u8"ָ������ʱ�䣺");
							ImGui::SameLine();
							ImGui::SetNextItemWidth(200.0f);
							ImGui::SliderInt(u8"ʱ", &hour_local, 0, 23);
							ImGui::SameLine();
							ImGui::SetNextItemWidth(200.0f);
							ImGui::SliderInt(u8"��", &minute_local, 0, 59);
							ImGui::Text((u8"����ʱ�䣺" + std::to_string(hour_local) + u8"ʱ" + std::to_string(minute_local) + u8"��").c_str());
						}
						if (ImGui::CollapsingHeader(u8"��������"))
						{
							ImGui::Indent();
							//��Դ��������
							ImGui::Text(u8"��������");
							ImGui::ColorEdit3("Material Specular", material_specular);
							ImGui::SliderFloat("Material Shininess", &material_shininess, 0.0f, 400.0f);
							ImGui::Separator();
							ImGui::Text(u8"�۹�����");
							if (spotlight)
							{
								ImGui::Checkbox(u8"�۹�ģʽ", &spotlight);
								ImGui::ColorEdit3(u8"�۹� ������", spotLight_ambient);
								ImGui::ColorEdit3(u8"�۹� ɢ��", spotLight_diffuse);
								ImGui::ColorEdit3(u8"�۹� ����", spotLight_specular);
								ImGui::SliderFloat(u8"�۹� ��Ե", &spotLight_cutOff, 5.0f, 40.0f);
								ImGui::SliderFloat(u8"�۹� ��Ե", &spotLight_outerCutOff, 10.0f, 60.0f);
								ImGui::SliderFloat3(u8"�۹� ����", spotLight_direction, -1.0f, 1.0f);
							}
							else
							{
								ImGui::Checkbox(u8"ɢ��ģʽ", &spotlight);
							}
							ImGui::Separator();
							ImGui::Text(u8"���������");
							ImGui::ColorEdit3("DirLight Ambient", dirLight_ambient);
							ImGui::ColorEdit3("DirLight Diffuse", dirLight_diffuse);
							ImGui::ColorEdit3("DirLight Specular", dirLight_specular);
							ImGui::SliderFloat3("DirLight Direction", dirLight_direction, -1.0f, 1.0f);
							//û��ʱ��ӳ��ʱ�����Զ�����Դ
							if (time_mode == 0)
							{
								ImGui::Separator();
								ImGui::Text(u8"���Դ����");
								ImGui::ColorEdit3("PointLights Ambient", pointLights_ambient);
								ImGui::ColorEdit3("PointLights Diffuse", pointLights_diffuse);
								ImGui::ColorEdit3("PointLights Specular", pointLights_specular);
								ImGui::SliderFloat3("PointLights Position", pointLights_position, -10.0f, 10.0f);
							}
							else
							{
								ImGui::Separator();
								ImGui::Text(u8"���õ��Դ���ƶ��뾶��");
								ImGui::SliderFloat("radius", &this->circle_radius, 2.0f, 15.0f);
							}
							ImGui::Unindent();
						}
					}
				}
			}
			ImGui::Unindent();
		}
	}
	if (time_mode == 0)
	{
		this->drawEnv(-1, -1, camera, woodShader, glassShader, skyboxShader, background, skybox, texture_act, light, spotlight,
			material_specular, material_shininess, spotLight_ambient, spotLight_diffuse, spotLight_specular,
			spotLight_cutOff, spotLight_outerCutOff, spotLight_direction, dirLight_direction, dirLight_ambient,
			dirLight_diffuse, dirLight_specular, pointLights_ambient, pointLights_diffuse, pointLights_specular, pointLights_position);
	}
	else if (time_mode == 1)
	{
		this->drawEnv(hours, minutes, camera, woodShader, glassShader, skyboxShader, background, skybox, texture_act, light, spotlight,
			material_specular, material_shininess, spotLight_ambient, spotLight_diffuse, spotLight_specular,
			spotLight_cutOff, spotLight_outerCutOff, spotLight_direction, dirLight_direction, dirLight_ambient,
			dirLight_diffuse, dirLight_specular, pointLights_ambient, pointLights_diffuse, pointLights_specular, pointLights_position);
	}
	else if (time_mode == 2)
	{
		this->drawEnv(hour_local, minute_local, camera, woodShader, glassShader, skyboxShader, background, skybox, texture_act, light, spotlight,
			material_specular, material_shininess, spotLight_ambient, spotLight_diffuse, spotLight_specular,
			spotLight_cutOff, spotLight_outerCutOff, spotLight_direction, dirLight_direction, dirLight_ambient,
			dirLight_diffuse, dirLight_specular, pointLights_ambient, pointLights_diffuse, pointLights_specular, pointLights_position);
	}
}

void Environment::createEnv(Shader& woodShader, Shader& glassShader, Shader& skyboxShader)
{
	// cube VAO
	glGenVertexArrays(1, &this->cubeVAO);
	glGenBuffers(1, &this->cubeVBO);
	glBindVertexArray(this->cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// skybox VAO
	glGenVertexArrays(1, &this->skyboxVAO);
	glGenBuffers(1, &this->skyboxVBO);
	glBindVertexArray(this->skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// screen quad VAO
	glEnableVertexAttribArray(0);
	//���ʶ�ȡ
	this->woodTexture = this->load_texture("res/textures/wood.png");
	this->marbleTexture = this->load_texture("res/textures/marble.png");
	this->brickTexture = this->load_texture("res/textures/brickwall.jpg");
	this->stoneTexture = this->load_texture("res/textures/stonewall.jpg");
	this->goldTexture = this->load_texture("res/textures/gold.jpg");
	std::vector<std::string> faces
	{
		"res/skybox/right.jpg",
		"res/skybox/left.jpg",
		"res/skybox/top.jpg",
		"res/skybox/bottom.jpg",
		"res/skybox/front.jpg",
		"res/skybox/back.jpg"
	};
	this->cubemapTexture = this->loadCubemap(faces);
	woodShader.use();
	woodShader.setInt("texture1", 1);
	woodShader.setInt("texture2", 2);
	woodShader.setInt("texture3", 3);
	woodShader.setInt("texture4", 4);
	woodShader.setInt("texture5", 5);
	glassShader.use();
	glassShader.setInt("skybox", 0);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
}

void Environment::processInput(Camera& camera, GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKayboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKayboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKayboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKayboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		camera.ProcessKayboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		camera.ProcessKayboard(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera.ProcessView(true);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera.ProcessView(false);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.shiftMode = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.shiftMode = 3.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.shiftMode = 0.1f;
	}
	else
	{
		camera.shiftMode = 1.0f;
	}
}

void Environment::drawEnv(int hour, int minute, Camera& camera, Shader& woodShader, Shader& glassShader, Shader& skyboxShader, 
	bool background, bool skybox, int texture_type, bool light, bool spotlight,
	float material_specular[3], float material_shininess, float spotLight_ambient[3], float spotLight_diffuse[3], float spotLight_specular[3],
	float spotLight_cutOff, float spotLight_outerCutOff, float spotLight_direction[3], float dirLight_direction[3], float dirLight_ambient[3],
	float dirLight_diffuse[3], float dirLight_specular[3], float pointLights_ambient[3], float pointLights_diffuse[3], float pointLights_specular[3],
	float pointLights_position[3])
{
	float gray_ratio = 0.6f;	//�������ӣ������磬ֵΪ1����������ֵΪ0.6
	if (this->weather == 0 || this->weather == 5)
	{
		gray_ratio = 1.0f;
	}
	float timeBias = 0.0f;		//Ϊ0��ʾ̫����ȫ�µ�ƽ�ߣ�Ϊ2��ʾ̫����ȫ����
	//ҹ�����Ļ��ɫ�̶�Ϊ50,50,50
	if (hour >= 5 && hour < 7)
	{
		//�糿
		timeBias = ((float)hour - 5.0f) + minute / 60.0f;
	}
	else if (hour >= 17 && hour < 19)
	{
		//����
		timeBias = 2.0f - (((float)hour - 17.0f) + minute / 60.0f);
	}
	else if (hour >= 7 && hour < 17)
	{
		//ҹ��
		timeBias = 2.0f;
	}
	if (background)
	{
		this->model = glm::mat4(1.0f);
		this->view = camera.GetViewMatrix();
		this->projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		if (texture_type < 6)
		{
			//�޲��ʻ�ľ��
			woodShader.use();
			woodShader.setBool("light_switch", !light);
			woodShader.setMat4("model", this->model);
			woodShader.setMat4("view", this->view);
			woodShader.setMat4("projection", this->projection);
			woodShader.setFloat("texture_state", (float)texture_type);
			woodShader.setVec3("viewPos", camera.Position);
			//���մ���
			/*��ԴӰ������Ĵ���*/
			woodShader.setVec3("spotLight.ambient", spotLight_ambient[0], spotLight_ambient[1], spotLight_ambient[2]);
			woodShader.setVec3("spotLight.diffuse", spotLight_diffuse[0], spotLight_diffuse[1], spotLight_diffuse[2]);
			woodShader.setVec3("spotLight.specular", spotLight_specular[0], spotLight_specular[1], spotLight_specular[2]);
			woodShader.setFloat("spotLight.constant", 1.0f);
			woodShader.setFloat("spotLight.linear", 0.09f);
			woodShader.setFloat("spotLight.quadratic", 0.032f);
			woodShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(spotLight_cutOff)));
			woodShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotLight_outerCutOff)));
			if (spotlight)
			{
				//�۹�ģʽ
				woodShader.setVec3("spotLight.position", camera.Position);
				woodShader.setVec3("spotLight.direction", camera.Front);
			}
			else
			{
				//ɢ��ģʽ
				woodShader.setVec3("spotLight.position", lightPos);
				woodShader.setVec3("spotLight.direction", spotLight_direction[0], spotLight_direction[1], spotLight_direction[2]);
			}
			//directional light
			woodShader.setVec3("dirLight.direction", dirLight_direction[0], dirLight_direction[1], dirLight_direction[2]);
			woodShader.setVec3("dirLight.ambient", dirLight_ambient[0], dirLight_ambient[1], dirLight_ambient[2]);
			woodShader.setVec3("dirLight.diffuse", dirLight_diffuse[0], dirLight_diffuse[1], dirLight_diffuse[2]);
			woodShader.setVec3("dirLight.specular", dirLight_specular[0], dirLight_specular[1], dirLight_specular[2]);
			//point lights
			for (int i = 0; i < 1; i++)
			{
				if (hour == -1)
				{
					woodShader.setVec3(("pointLights[" + std::to_string(i) + "].position").c_str(), pointLightPositions[i] + glm::vec3(pointLights_position[0], pointLights_position[1], pointLights_position[2]));
					woodShader.setVec3(("pointLights[" + std::to_string(i) + "].ambient").c_str(), pointLights_ambient[0], pointLights_ambient[1], pointLights_ambient[2]);
					woodShader.setVec3(("pointLights[" + std::to_string(i) + "].diffuse").c_str(), pointLights_diffuse[0], pointLights_diffuse[1], pointLights_diffuse[2]);
					woodShader.setVec3(("pointLights[" + std::to_string(i) + "].specular").c_str(), pointLights_specular[0], pointLights_specular[1], pointLights_specular[2]);
				}
				else
				{
					//��ʱ�������Դ��y���겻�䣬x��z���껭Բ��6ʱ��18ʱǰ��һСʱʹ��Դƫůɫ������ָ���18-17ʱ���͹�Դ���ȣ�5-6ʱ��߹�Դ����
					float x = (float)sin(((hour + minute / 60.0) / 12.0) * M_PI) * circle_radius;
					float z = -(float)cos(((hour + minute / 60.0) / 12.0) * M_PI) * circle_radius;
					woodShader.setVec3(("pointLights[" + std::to_string(i) + "].position").c_str(), pointLightPositions[i] + glm::vec3(x, 0.0f, z));
					//��RGB���������µ�ֵ�����㵽ʵ��diffuseʱ��Ҫ����ϵ��
					float r = 1.0f;
					float g = 1.0f;
					float b = 1.0f;
					//�������Ȳ���ʱ��仯�����ܵ���������죨���Ʋ㣩��Ӱ��
					if (this->weather == 7)
					{
						//ɳ����������ɫ�¹̶�
						if ((hour >= 5 && hour < 7) || (hour >= 17 && hour < 19))
						{
							//�ճ����䣺�����仯
							r = (50.0f + timeBias * 102.5f) / 255.0f;
							g = (50.0f + timeBias * 75.5f) / 255.0f;
							b = (50.0f + timeBias * 27.5f) / 255.0f;
						}
						else if (hour >= 7 && hour < 17)
						{
							//����
							r = 1.0f;
							g = 201.0f / 255.0f;
							b = 105.0f / 255.0f;
						}
						else
						{
							//���ϣ����ԣ���Դ����Ȧ�ƶ���������ǿ�Ȼ��ǹ��ߵ������
							r = 50.0f / 255.0f;
							g = 50.0f / 255.0f;
							b = 50.0f / 255.0f;
						}
					}
					else
					{
						//������ɫ�µı仯
						if ((hour >= 5 && hour < 7) || (hour >= 17 && hour < 19))
						{
							//�糿�����
							if (timeBias <= 0.5f)
							{
								//�ճ���һ��Сʱ��������Ļ��RGB��50�ᵽ90 * gray_ratio
								r = (50.0f + timeBias * 2 * (90.0f * gray_ratio - 50.0f)) / 255.0f;
								g = (50.0f + timeBias * 2 * (90.0f * gray_ratio - 50.0f)) / 255.0f;
								b = (50.0f + timeBias * 2 * (90.0f * gray_ratio - 50.0f)) / 255.0f;
							}
							else if (timeBias <= 1.0f)
							{
								//�ճ��ڶ���Сʱ��R 90��255��G 90��155��B 90��50
								r = (90.0f + (timeBias - 0.5f) * 330.0f) * gray_ratio / 255.0f;
								g = (90.0f + (timeBias - 0.5f) * 130.0f) * gray_ratio / 255.0f;
								b = (90.0f - (timeBias - 0.5f) * 80.0f) * gray_ratio / 255.0f;
							}
							else if (timeBias <= 1.5f)
							{
								//�ճ�������Сʱ��R 255��G 155��255��B 50
								r = 1.0f * gray_ratio;
								g = (155.0f + (timeBias - 1.0f) * 200.0f) * gray_ratio / 255.0f;
								b = 50.0f * gray_ratio / 255.0f;
							}
							else if (timeBias <= 2.0f)
							{
								//�ճ����İ�Сʱ��R 255��G 255��B 50��255
								r = 1.0f * gray_ratio;
								g = 1.0f * gray_ratio;
								b = (50.0f + (timeBias - 1.5f) * 410.0f) * gray_ratio / 255.0f;
							}
						}
						else if (hour >= 7 && hour < 17)
						{
							//����
							r = 1.0f * gray_ratio;
							g = 1.0f * gray_ratio;
							b = 1.0f * gray_ratio;
						}
						else
						{
							//���ϣ����ԣ���Դ����Ȧ�ƶ���������ǿ�Ȼ��ǹ��ߵ������
							r = 50.0f / 255.0f;
							g = 50.0f / 255.0f;
							b = 50.0f / 255.0f;
						}
					}
					woodShader.setVec3(("pointLights[" + std::to_string(i) + "].ambient").c_str(), pointLights_ambient[0], pointLights_ambient[1], pointLights_ambient[2]);
					woodShader.setVec3(("pointLights[" + std::to_string(i) + "].diffuse").c_str(), r, g, b);
					woodShader.setVec3(("pointLights[" + std::to_string(i) + "].specular").c_str(), pointLights_specular[0], pointLights_specular[1], pointLights_specular[2]);
				}
				woodShader.setFloat(("pointLights[" + std::to_string(i) + "].constant").c_str(), 1.0f);
				woodShader.setFloat(("pointLights[" + std::to_string(i) + "].linear").c_str(), 0.09f);
				woodShader.setFloat(("pointLights[" + std::to_string(i) + "].quadratic").c_str(), 0.032f);
			}
			woodShader.setVec3("material.specular", material_specular[0], material_specular[1], material_specular[2]);
			woodShader.setFloat("material.shininess", material_shininess);
			glBindVertexArray(this->cubeVAO);
			switch (texture_type)
			{
			case 0:
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, this->woodTexture);
				break;
			case 1:
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, this->woodTexture);
				break;
			case 2:
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, this->marbleTexture);
				break;
			case 3:
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, this->brickTexture);
				break;
			case 4:
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, this->stoneTexture);
				break;
			case 5:
				glActiveTexture(GL_TEXTURE5);
				glBindTexture(GL_TEXTURE_2D, this->goldTexture);
				break;
			default:
				break;
			}
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
		else
		{
			//�������
			glassShader.use();
			glassShader.setMat4("model", this->model);
			glassShader.setMat4("view", this->view);
			glassShader.setMat4("projection", this->projection);
			glassShader.setVec3("cameraPos", camera.Position);
			glassShader.setFloat("texture_state", (float)(texture_type - 6));
			glBindVertexArray(this->cubeVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
	}
	if (skybox)
	{
		// change depth function so depth test passes when values are equal to depth buffer's content
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		this->view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		this->projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		skyboxShader.setMat4("view", this->view);
		skyboxShader.setMat4("projection", this->projection);
		float ratio = 1.0f;
		const static float night_ratio = 0.2f;
		if (hour != -1)
		{
			if (hour >= 5 && hour < 19)
			{
				//����������趨
				ratio = night_ratio + (gray_ratio - night_ratio) * timeBias / 2.0f;
			}
			else
			{
				ratio = night_ratio;
			}
		}
		skyboxShader.setFloat("gray_ratio", ratio);
		// skybox cube
		glBindVertexArray(this->skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		// set depth function back to default
		glDepthFunc(GL_LESS);
	}
}

unsigned int Environment::load_texture(const char* texture_path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//stbi_set_flip_vertically_on_load(true);
	/*filter options*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texture_path, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
		{
			format = GL_RED;
		}
		if (nrChannels == 3)
		{
			format = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

unsigned int Environment::loadCubemap(std::vector<std::string>& faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}
