#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "res/GL_heads/Shader.h"
#include "res/GL_heads/Camera.hpp"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define SCR_WIDTH 960
#define SCR_HEIGHT 540
#define MAPSIZE_X 10
#define MAPSIZE_Y 10
#define MAPSIZE_Z 10
#define GLSL_VERSION "#version 330\n"

/*
��ImGui���潻���ı�����Ҫ������ʱ��Ϊstatic���͵ı���������APIʱͨ��&�����ַ

ImGui�ĳ�ʼ��������ѭ��֮ǰ��
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
ImGui��ɫģʽѡ��
ImGui::StyleColorsDark();

��ѭ���У�
	//��֡������䣨����ѭ����ǰ��λ�ã�
	ImGui_ImplOpenGL3_NewFrame();
	//ע�⣺glfw����֡��������openGL����֡���֮��
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//����imgui����Ԫ�صĶ���֮����Ⱦ���
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui�������ݶ��壺
		ImGui::Begin("GUI 2");
		//�м�Ŵ���Ԫ��
		ImGui::End();
	�������ö�����ڣ�ÿ��Begin��End֮��Ϊһ������

imGUI��Դ������䣨����ѭ��֮��
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

//���ؼ�����ͬһ�У�������һ�������Ч������Ϊ�ؼ��ļ�࣬������ѡ
	ImGui::SameLine([float dist]);


*/

/* matrix */
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
/* vector */
glm::vec2 rec_pos = glm::vec2(0.0f);

/* camera attributes */
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float alpha = 0.4f;
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//VBO
float vertices[] =
{
	//����
	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	//�ײ�
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	//����
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	//����
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	//����
	-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	//ǰ��
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

//EBO
//int indecies[] =
//{
//	0,1,2,
//	2,3,0
//};


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (alpha < 1.0)
		{
			alpha += 0.01f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (alpha > 0)
		{
			alpha -= 0.01f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		rec_pos.x -= 0.04f;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		rec_pos.x += 0.04f;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		rec_pos.y += 0.04f;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		rec_pos.y -= 0.04f;
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
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_cursor_posotion(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	//ֻ�ڰ�������Ҽ�ʱ����任�ӽ�
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_TRUE)
	{
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

void mouse_scroll_posotion(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

unsigned int load_texture(const char* texture_path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	stbi_set_flip_vertically_on_load(true);
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


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_posotion);
	glfwSetScrollCallback(window, mouse_scroll_posotion);
	//����������ڲ���ʾ���
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
	}

	/*Shader*/
	Shader myShader("res/Shaders/Shader_before_light/vertexShader_before_light.vert", "res/Shaders/Shader_before_light/fragmentShader_before_light.frag");

	//���Ч����ʾ
	glEnable(GL_DEPTH_TEST);

	/*imGUI*/
	//set up dear imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//�����������ָ��ImGui���ض������ļ���ÿ�����й��󣬴��ڵ���Ϣ�ᱻ��¼������ļ���
	//io.IniFilename = "test.ini";
	//setup the platform
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	//setup style
	ImGui::StyleColorsDark();

	/*buffers*/
	unsigned int VBO, VAO/*, EBO*/;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	//VBO��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	////EBO��
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), &indecies, GL_STATIC_DRAW);

	//position attribute��3Ϊÿ�鶥��Ĳ�������������ʾxyz���꣨openglһ�Σ�һ�����㣩��ȡ�������ݣ����������ͣ��Ƿ��׼������ȡ������ÿ���㣩��ÿ�ζ�ȡ��ƫ����������ǰ��Ҫ��(void*)��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/*Texture*/
	unsigned int container_texture = load_texture("res/textures/container.png");
	unsigned int smile_texture = load_texture("res/textures/smile.png");
	myShader.use();
	myShader.setInt("texture_1", 0);
	myShader.setInt("texture_2", 1);

	//�Ƿ���ʾInGui��DEMO
	bool showDemo = false;
	bool showWindow2 = false;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		float time = (float)glfwGetTime();
		float xValue = (float)(0.5 * std::sin(time) + 0.5);
		float yValue = (float)(0.5 * std::cos(time) + 0.5);
		glm::vec3 myVec;
		myVec.x = xValue;
		myVec.y = yValue;
		myVec.z = 0.31f;

		deltaTime = time - lastFrame;
		lastFrame = time;

		//Gui variables
		static float scale_value[3] = { 1.0f, 1.0f, 1.0f };
		static float color_value[3] = { 1.0f, 1.0f, 1.0f };
		static float rotate_value[3] = { 1.0f, 1.0f, 1.0f };
		//�Զ�����ɫ�л�
		static bool isTexture = false;
		//��ɫ�任Ч���л�����������ʾģʽ�У�
		static bool isColor = false;
		//�෽���л�
		static bool isMultiple = false;
		//�Ƕ�
		static float angle = 0.0f;
		if (isTexture)
		{
			myShader.setVec3("colors", myVec);
		}
		else
		{
			myShader.setVec3("colors", color_value[0], color_value[1], color_value[2]);
		}
		myShader.setBool("isTexture", isTexture);
		myShader.setBool("isColor", isColor);

		projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
		myShader.setMat4("projection", projection);
		view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		myShader.setMat4("view", view);

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*start new frame for imGUI*/
		ImGui_ImplOpenGL3_NewFrame();
		//ע�⣺glfw����֡��������openGL����֡���֮��
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//ImGui���ܲ��Դ���
		if (showDemo)
		{
			ImGui::ShowDemoWindow(&showDemo);
		}
		//�ڶ�������
		if (showWindow2)
		{
			ImGui::Begin("GUI 2");
			ImGui::Text("the second ImGui window here");
			if (ImGui::Button("shut down window2"))
			{
				showWindow2 = false;
			}
			ImGui::End();
		}

		myShader.use();

		//������ģʽ
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glLineWidth(4.0f);

		/*��EBO����*/
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, container_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, smile_texture);
		myShader.setFloat("alpha", alpha);

		//��Ӧimgui->DragFloat
		//static float scale_value = 1.0f;
		//��Ӧimgui->DragFloat2
		//static float scale_value[2] = { 1.0f, 1.0f };
		//��Ӧimgui->Dragfloat3
		//static float scale_value[3] = { 1.0f, 1.0f, 1.0f };

		if (!isMultiple)
		{
			glm::mat4 model = glm::mat4(1.0f);
			//��Ӧimgui->DragFloat
			//model = glm::scale(model, glm::vec3(scale_value));
			//��Ӧimgui->DragFloat2
			//model = glm::scale(model, glm::vec3(scale_value[0], scale_value[1], 1.0f));
			//��Ӧimgui->Dragfloat3
			model = glm::scale(model, glm::vec3(scale_value[0], scale_value[1], scale_value[2]));
			model = glm::translate(model, cubePositions[0]);
			model = glm::translate(model, glm::vec3(rec_pos.x, rec_pos.y, 0.0f));
			model = glm::rotate(model, glm::radians(angle * 60), glm::vec3(rotate_value[0], rotate_value[1], rotate_value[2]));
			myShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else
		{
			for (unsigned int i = 0; i < 10; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				//��Ӧimgui->DragFloat
				//model = glm::scale(model, glm::vec3(scale_value));
				//��Ӧimgui->DragFloat2
				//model = glm::scale(model, glm::vec3(scale_value[0], scale_value[1], 1.0f));
				//��Ӧimgui->Dragfloat3
				model = glm::scale(model, glm::vec3(scale_value[0], scale_value[1], scale_value[2]));
				model = glm::translate(model, cubePositions[i]);
				model = glm::translate(model, glm::vec3(rec_pos.x, rec_pos.y, 0.0f));
				float angle = 20.0f * i;
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				myShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		////��EBO����
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//render GUI
		float FPS = ImGui::GetIO().Framerate;
		ImGui::Begin("Gui::Hello_world");
		ImGui::Text("model Attribute control pad");
		//��ȡ֡��
		ImGui::Text("FPS = %f", FPS);
		/*drag float�϶���*/
		//ʵʱ���������С������ע�ⶨ��ı���float��Ҫ��static����
		//����������Ϊ�϶�ֵ�仯�������ȣ����ĸ��͵��������Ϊֵ�仯������������
		//ImGui::DragFloat("scale", &scale_value, 0.1f, 0.01f, 5.0f);
		//ImGui::DragFloat2("scale", scale_value, 0.1f, 0.01f, 5.0f);
		ImGui::DragFloat3("scale", scale_value, 0.1f, 0.01f, 5.0f);
		/*slider float*/
		//Ŀ�����Ϊstatic�ͣ��������ĸ�����Ϊֵ�����ޡ�����
		ImGui::SliderFloat3("color", color_value, 0.0f, 1.0f);
		ImGui::SliderFloat("alpha", &alpha, 0.0f, 1.0f);
		ImGui::SliderFloat3("rotate", rotate_value, 0.0f, 1.0f);
		/*color edit*/
		ImGui::ColorEdit3("color", color_value);
		/*check box*/
		//�л���ʾ��ϲ��ʻ����Զ�����ʣ���Ҫ����һ��bool��ͨ��checkbox�����ƣ���ͬʱ����������Ƭ����ɫ�����л���Ⱦģʽ�����ݴ���
		ImGui::Checkbox("Texture", &isTexture);
		//���ؼ�����ͬһ�У�����Ϊ�ؼ��ļ��
		ImGui::SameLine();
		ImGui::Checkbox("isColor", &isColor);
		ImGui::SameLine();
		ImGui::Checkbox("isMultiple", &isMultiple);
		/*slider angle*/
		ImGui::SliderAngle("angle", &angle);
		/*button*/
		if (ImGui::Button("DEMO on/off"))
		{
			showDemo = !showDemo;
		}
		//���ؼ�����ͬһ��
		ImGui::SameLine(270.0f);
		if (ImGui::Button("open window2"))
		{
			showWindow2 = true;
		}
		ImGui::End();

		//render to screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//clear up for imGUI
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}