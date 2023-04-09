#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>
#include"res/GL_heads/Shader.h"
#include "stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define SCR_WIDTH 1000
#define SCR_HEIGHT 800

/*
һ���������غ���
glClear():���Ӵ�����������һ�������Ƶ�ͼ�Ρ�
glClearColor(GLclampf red��GLclampf green��GLclampf blue�� GLclampf alpha):������ɫ��Alphaֵ�����������ɫ����������Ҫʹ�õ����ֵ��Χ�ڡ�0.0f��1.0f����
glClearDepth(GLclampf depth):�������ֵ��������Ȼ������������
glClearStencil(GLint value):����ģ��ֵ������ģ�建�����������
glClearAccum(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha):������ۻ�������ֵ��
glClearIndex( GLfloat c)��ָ�������ɫ����������ʱʹ�õ�������Ĭ��ֵΪ�㡣

glClearʹ�� | �������ϲ�ͬ�Ļ����־λ��������Ҫ����Ļ��壬����ʹ�����±�־λ��
GL_COLOR_BUFFER_BIT:      ��ǰ��д����ɫ����
GL_DEPTH_BUFFER_BIT:      ��Ȼ���
GL_ACCUM_BUFFER_BIT:      �ۻ�����
GL_STENCIL_BUFFER_BIT:     ģ�建��

float vertices[]����������飬����x,y,z������ֵ

��ν�Ĳ�ֵ����Ƭ�β�ֵ��Fragment interpolation������Ⱦ�Ƿ�����Fragment shader��
���֪������graphic pipeline�Ļ�����fragment shader֮ǰ�ǹ�դ����Rasterisation����
�ڹ�դ���Ľ׶Σ�������һ���򵥵������λᱻ�ָ�ɺܶ�ܶ�СƬ�Σ�����һ������fragment��Ҳ����Ƭ�Ρ�
��������ͼ�α��ָ���˺ܶ�ܶ��Ƭ�Σ���ÿ������Ƭ����ɫ���������ڹ�դ���׶θ���ÿ��Ƭ������ͼ�ε�λ�ñ����Բ�ֵ�ˡ�
��Fragment shader���棬��ÿ��Ƭ�ν�����Ⱦ����Ϊ��Ⱦ����ɫ�������ε��������㣬������Ⱦ��ʱ���������������ڵ�Ƭ����������趨�õ���ɫ��
һ��Ƭ�ε���ɫ�ɸ�����Ƭ����ɫ���������������ص㣬һ��Ƭ�ε���ɫ�����ɺܶ�����ص����ɫ��ϡ�
�����Ļ������ֻ�����������������ɫ����ô���������������ڵ�Ƭ��֮��ʣ�µ�Ƭ�ξ�û���κ���ɫ��
���������һ�£�������������Բ�ֵ����ô���������Σ�����ɫ�ĵط��������������ε��������㣬�����������������������ڵ�Ƭ�Ρ�

������б������
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

void glDrawArrays(  GLenum mode,    GLint first,    GLsizei count);
mode
	��Ҫ��Ⱦ��ͼԪ���ͣ����� GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN ��GL_TRIANGLES��
	GL_POINTS����ÿһ��������Ϊһ������д�������n�������˵�n��������N����
	GL_LINES������ÿ����������Ϊһ���������߶Σ�����2n��1��2n֮�乲������n���߶Σ��ܹ�����N/2���߶�
	GL_LINE_STRIP�����ƴӵ�һ�����㵽���һ����������������һ���߶Σ���n��n+1�����㶨�����߶�n���ܹ�����n��1���߶�
	GL_LINE_LOOP�����ƴӵ�һ�����㵽���һ����������������һ���߶Σ�Ȼ�����һ������͵�һ��������������n��n+1�����㶨�����߶�n���ܹ�����n���߶�
	GL_TRIANGLES����ÿ����������Ϊһ�������������Σ�����3n��2��3n��1��3n�����˵�n�������Σ��ܹ�����N/3��������
	GL_TRIANGLE_STRIP������һ�������������Σ���������n������n��n+1��n+2�����˵�n�������Σ�����ż��n������n+1��n��n+2�����˵�n�������Σ��ܹ�����N-2��������
	GL_TRIANGLE_FAN������һ�������������Σ����������ɵ�һ�����㼰�������Ķ���ȷ��������1��n+1��n+2�����˵�n�������Σ��ܹ�����N-2��������
first
	�����黺���е���һλ��ʼ���ƣ�һ��Ϊ0.
count
	�����ж��������.




*/

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::vec2 rec_pos = glm::vec2(0.0f);
float alpha = 0.2;

//VBO
//float vertices[] = 
//{
//	-0.5f, -0.5f, 0.0f, 
//	0.5f, -0.5f, 0.0f, 
//	0.5f, 0.5f, 0.0f, 
//
//	0.5f, 0.5f, 0.0f, 
//	-0.5f, 0.5f, 0.0f, 
//	-0.5f, -0.5f, 0.0f, 
//};
//float vertices[] = 
//{
//	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
//	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
//	0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 
//};

//����������1��������Ϊ�������ֵ��������ͼ�����Ч��
//��Ч��������Ƭ����ɫ���ļ���texture(ourTexture, n * TexCoords) ����n���
float vertices[] = 
{
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
	0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 

	0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 
	-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
};

//EBO
//float vertices[] =
//{
//	-0.5f, -0.5f, 0.0f,
//	0.5f, -0.5f, 0.0f,
//	0.5f, 0.5f, 0.0f,
//	-0.5f, 0.5f, 0.0f,
//};
//int indecies[] =
//{
//	0,1,2,
//	2,3,0
//};

//��ɫ��
//in�������ɫ��������˸�ʽ����һ����ɫ��������ɫ����in֮ǰ��Ҫ����layout�������VAO�л�ȡ����
//const char* vertexShaderData =
//	"#version 330 core\n"
//	"layout (location = 0) in vec3 aPos;\n"
//	"layout (location = 1) in vec3 aColor;\n"
//	"out vec3 ourColor;\n"
//	"void main()\n"
//	"{\n"
//	"	ourColor = aColor;\n"
//	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
//	"}\0";
//const char* fragmentShaderData =
//	"#version 330 core\n"
//	"out vec4 FragColor;\n"
//	"in vec3 ourColor;\n"
//	"uniform float xColor;\n"
//	"void main()\n"
//	"{\n"
//	//"	FragColor = vec4(xColor, 0.5f, 0.31f, 1.0f);\n"
//	//"	FragColor = vec4(1.0f, 0.5f, 0.31f, 1.0f);\n"
//	"	FragColor = vec4(xColor, ourColor.y, ourColor.z, 1.0f);\n"
//	"}\n\0";

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
			alpha += 0.01;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (alpha > 0)
		{
			alpha -= 0.01;
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
}

//view port
//ÿ����ǰ���ڵĴ�С���Ķ�����������ͱ����ã�����javascript���¼���������ʱ�����ظ����ã�
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_cursor_posotion(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "mouse has been moved xpos: " << xpos << " ypos: " << ypos << std::endl;
}

void mouse_scroll_posotion(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << "mouse has been scrolled xoffset: " << xoffset << " yoffset: " << yoffset << std::endl;
}

unsigned int load_texture(const char* texture_path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//��ֹ������б
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//��ȡpngͼ��ǰ��Ҫ��ͼ����
	stbi_set_flip_vertically_on_load(true);
	/*filter options*/
	//��S�����þ����ظ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//��T����������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//�²�������ͼ��ķֱ��ʱ�ͣ���Сͼ�񣩣��ͽ���������������Сͼ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//�ϲ�������ͼ��ķֱ��ʱ�ߣ��Ŵ�ͼ�񣩣����Բ�ֵ������С��������ͼ
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
			//jpg
			format = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			//png
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
	/*��ʼ��������*/
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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
	}

	/*Shader*/
	Shader myShader("res/Shaders/Shader_prim/vertexShader.vert", "res/Shaders/Shader_prim/fragmentShader.frag");

	/*buffers*/
	//Vertex_Buffer_Object -> VBO
	//Vertex_Array_Object -> VAO
	unsigned int VBO, VAO/*, EBO*/;
	glGenVertexArrays(1, &VAO);
	//����VBO��1Ϊ����VBO������
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	
	//VBO��
	//bind������Ϊ������VBO����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//���������ݴ���opengl�ķ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	
	////EBO��
	////����EBO���������������VBO������������Ӧ����Ҫ�Ķ���һ�����Ͳ������Ķ�����Ĳ�����һһ��Ӧ
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), &indecies, GL_STATIC_DRAW);
	
	//position attribute��3Ϊÿ�鶥��Ĳ�������������ʾxyz���꣨openglһ�Σ�һ�����㣩��ȡ�������ݣ����������ͣ��Ƿ��׼������ȡ������ÿ���㣩��ÿ�ζ�ȡ��ƫ����������ǰ��Ҫ��(void*)��
	//�����ǰ�VAO��glVertexAttribPointerҲ�����򶥵���ɫ����layout��䴫�������VAO�ĵ�0������ͨ�������䴫��������ɫ��GLSL
	//������ɫ������Ⱦ���ߵĵ�һ�����룬��������VAO��glVertexAttribPointer��glEnableVertexAttribArray�ɶԳ��֣�ÿһ�Զ�Ӧһ��VAO����һ������ΪVAO����ţ�ͨ��GLSL��layout��location������ţ�һһ��Ӧ������������
	//ע�⣺ÿ��VAO��Ӧ��layout(location = x) glVertexAttribPointer(x, ...) glEnableVertexAttribArray(x)�������ط���x��Ҫһ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute��ͬposition attribute������ڶ������Ϊ1��VAO���涨��ȡ��ʽ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture attribute���ڶ�������Ϊÿ������Ĳ�������������Ϊ�������꣬���Բ�������Ϊ2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	/*Texture*/
	unsigned int container_texture = load_texture("res/textures/container.png");
	unsigned int smile_texture = load_texture("res/textures/smile.png");
	myShader.use();
	myShader.setInt("texture_1", 0);
	myShader.setInt("texture_2", 1);


	while (!glfwWindowShouldClose(window))
	{
		/*update*/
		processInput(window);
		//��GLSL-uniform������
		//��ɫ�任����
		float time = glfwGetTime();
		float xValue = 0.5 * std::sin(time) + 0.5;
		float yValue = 0.5 * std::cos(time) + 0.5;
		glm::vec3 myVec;
		myVec.x = xValue;
		myVec.y = yValue;
		myVec.z = 0.31f;
		myShader.setVec3("colors", myVec);
		//myShader.setFloat("xColor", xValue);
		//�任
		model = glm::mat4(1.0f);
		//float ratio_scale = 0.2 * std::sin(time) + 0.7;
		//model = glm::scale(model, glm::vec3(ratio_scale, ratio_scale, ratio_scale));
		model = glm::translate(model, glm::vec3(rec_pos.x, rec_pos.y, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.5f, 1.0f));
		myShader.setMat4("model", model);

		projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
		myShader.setMat4("projection", projection);
		view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		myShader.setMat4("view", view);


		/*render*/
		//RGBAͨ����alphaͨ����͸����ͨ����0.0f������ȫ͸��
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		/*��Ⱦ����*/
		myShader.use();

		//������ģʽ
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		/*��EBO����*/
		//��VAO
		glBindVertexArray(VAO);
		//�����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, container_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, smile_texture);
		myShader.setFloat("alpha", alpha);
		//�滭���ڶ�������Ϊ�ܹ����ƵĶ������
		glDrawArrays(GL_TRIANGLES, 0, 6);

		////��EBO����
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		////�ڶ�������Ļ���
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-0.5f, -0.2f, 0.0f));
		//myShader.setMat4("model", model);
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);


		/*display*/
		glfwSwapBuffers(window);
		/*pollEvents*/
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}