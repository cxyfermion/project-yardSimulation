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
一、清除等相关函数
glClear():将从窗口中清除最后一次所绘制的图形。
glClearColor(GLclampf red，GLclampf green，GLclampf blue， GLclampf alpha):设置颜色和Alpha值，用于清除颜色缓冲区，需要使用的填充值范围在【0.0f，1.0f】。
glClearDepth(GLclampf depth):设置深度值，用于深度缓冲区的清除。
glClearStencil(GLint value):设置模板值，用于模板缓冲区的清除。
glClearAccum(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha):清除积累缓冲区的值。
glClearIndex( GLfloat c)：指定清除颜色索引缓冲区时使用的索引。默认值为零。

glClear使用 | 运算符组合不同的缓冲标志位，表明需要清除的缓冲，可以使用以下标志位。
GL_COLOR_BUFFER_BIT:      当前可写的颜色缓冲
GL_DEPTH_BUFFER_BIT:      深度缓冲
GL_ACCUM_BUFFER_BIT:      累积缓冲
GL_STENCIL_BUFFER_BIT:     模板缓冲

float vertices[]的坐标分三组，代表x,y,z的坐标值

所谓的插值叫做片段插值（Fragment interpolation）。渲染是发生在Fragment shader。
如果知道整个graphic pipeline的话，在fragment shader之前是光栅化（Rasterisation）。
在光栅化的阶段，哪怕是一个简单的三角形会被分割成很多很多小片段，就是一个个的fragment，也就是片段。
所以整个图形被分割成了很多很多的片段，而每个传入片段着色器的数据在光栅化阶段根据每个片段所在图形的位置被线性插值了。
在Fragment shader里面，对每个片段进行渲染。因为渲染的颜色是三角形的三个顶点，于是渲染的时候会对三个顶点所在的片段填充我们设定好的颜色。
一个片段的颜色由附近的片段颜色决定，并不是像素点，一个片段的颜色可能由很多个像素点的颜色混合。
这样的话，如果只定义了三个顶点的颜色，那么除开三个顶点所在的片段之后，剩下的片段就没有任何颜色。
你可以试想一下，如果不进行线性插值，那么整个三角形，有颜色的地方就是整个三角形的三个顶点，而且是三角形三个顶点所在的片段。

纹理倾斜矫正：
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

void glDrawArrays(  GLenum mode,    GLint first,    GLsizei count);
mode
	需要渲染的图元类型，包括 GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN ，GL_TRIANGLES。
	GL_POINTS：把每一个顶点作为一个点进行处理，顶点n即定义了点n，共绘制N个点
	GL_LINES：连接每两个顶点作为一个独立的线段，顶点2n－1和2n之间共定义了n条线段，总共绘制N/2条线段
	GL_LINE_STRIP：绘制从第一个顶点到最后一个顶点依次相连的一组线段，第n和n+1个顶点定义了线段n，总共绘制n－1条线段
	GL_LINE_LOOP：绘制从第一个顶点到最后一个顶点依次相连的一组线段，然后最后一个顶点和第一个顶点相连，第n和n+1个顶点定义了线段n，总共绘制n条线段
	GL_TRIANGLES：把每三个顶点作为一个独立的三角形，顶点3n－2、3n－1和3n定义了第n个三角形，总共绘制N/3个三角形
	GL_TRIANGLE_STRIP：绘制一组相连的三角形，对于奇数n，顶点n、n+1和n+2定义了第n个三角形；对于偶数n，顶点n+1、n和n+2定义了第n个三角形，总共绘制N-2个三角形
	GL_TRIANGLE_FAN：绘制一组相连的三角形，三角形是由第一个顶点及其后给定的顶点确定，顶点1、n+1和n+2定义了第n个三角形，总共绘制N-2个三角形
first
	从数组缓存中的哪一位开始绘制，一般为0.
count
	数组中顶点的数量.




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

//纹理坐标中1可以设置为更大的数值，会有贴图矩阵的效果
//此效果可以在片段着色器文件中texture(ourTexture, n * TexCoords) 设置n完成
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

//着色器
//in语句是着色器的输入端格式，第一级着色器顶点着色器的in之前需要加上layout语句来从VAO中获取数据
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
//每当当前窗口的大小被改动，这个函数就被调用（类似javascript的事件，被触发时不断重复调用）
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
	//防止纹理倾斜
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//读取png图像前需要将图像倒置
	stbi_set_flip_vertically_on_load(true);
	/*filter options*/
	//在S方向用镜像重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//在T方向用延伸
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//下采样（让图像的分辨率变低，缩小图像），就近采样，大纹理贴小图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//上采样（让图像的分辨率变高，放大图像），线性插值采样，小纹理贴大图
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
	/*初始化与载入*/
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
	//生成VBO，1为生成VBO的数量
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	
	//VBO绑定
	//bind，参数为类型与VBO代号
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将顶点数据传给opengl的方法
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	
	////EBO绑定
	////对于EBO，下面两行语句与VBO的上两行语句对应，需要改动第一个类型参数，改动传入的参数，一一对应
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), &indecies, GL_STATIC_DRAW);
	
	//position attribute，3为每组顶点的参数个数，即表示xyz坐标（opengl一次（一个顶点）读取几个数据），数据类型，是否标准化，读取步长（每顶点），每次读取的偏移量步长（前面要加(void*)）
	//这里是绑定VAO，glVertexAttribPointer也用于向顶点着色器的layout语句传输变量，VAO的第0组数据通过这个语句传给顶点着色器GLSL
	//顶点着色器是渲染管线的第一级输入，数据来自VAO，glVertexAttribPointer与glEnableVertexAttribArray成对出现，每一对对应一个VAO，第一个参数为VAO的序号，通过GLSL中layout的location坐标序号，一一对应，来传入数据
	//注意：每个VAO对应的layout(location = x) glVertexAttribPointer(x, ...) glEnableVertexAttribArray(x)这三处地方的x都要一致
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute，同position attribute，定义第二个序号为1的VAO，规定读取格式并绑定
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture attribute，第二个参数为每个顶点的参数个数，参数为纹理坐标，所以参数个数为2
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
		//给GLSL-uniform传数据
		//颜色变换部分
		float time = glfwGetTime();
		float xValue = 0.5 * std::sin(time) + 0.5;
		float yValue = 0.5 * std::cos(time) + 0.5;
		glm::vec3 myVec;
		myVec.x = xValue;
		myVec.y = yValue;
		myVec.z = 0.31f;
		myShader.setVec3("colors", myVec);
		//myShader.setFloat("xColor", xValue);
		//变换
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
		//RGBA通道，alpha通道是透明度通道，0.0f代表完全透明
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		/*渲染管线*/
		myShader.use();

		//仅画线模式
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		/*无EBO绘制*/
		//绑定VAO
		glBindVertexArray(VAO);
		//纹理绑定
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, container_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, smile_texture);
		myShader.setFloat("alpha", alpha);
		//绘画，第二个参数为总共绘制的顶点个数
		glDrawArrays(GL_TRIANGLES, 0, 6);

		////有EBO绘制
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		////第二个物体的绘制
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