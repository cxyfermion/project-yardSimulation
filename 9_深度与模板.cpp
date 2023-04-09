#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>
#include"res/GL_heads/Shader.h"
#include"res/GL_heads/Camera.hpp"
#include "stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>

#define SCR_WIDTH 1000
#define SCR_HEIGHT 800
#define GLSL_VERSION "#version 330\n"

/*
��Ļ�ռ�������ͨ��OpenGL��glViewport��������ӿ�������أ����ҿ���ֱ��ʹ��GLSL�ڽ�����gl_FragCoord��Ƭ����ɫ����ֱ�ӷ��ʡ�
gl_FragCoord��x��y����������Ƭ�ε���Ļ�ռ����꣨����(0, 0)λ�����½ǣ���z��������������Ƭ�����������ֵ��

��Ȳ���Ĭ���ǽ��õģ��������Ҫ������Ȳ��ԵĻ���������Ҫ��GL_DEPTH_TESTѡ������������
glEnable(GL_DEPTH_TEST);
�������õ�ʱ�����һ��Ƭ��ͨ������Ȳ��ԵĻ���OpenGL������Ȼ����д����Ƭ�ε�zֵ�����û��ͨ����Ȼ��壬��ᶪ����Ƭ�Ρ��������������Ȼ��壬�㻹Ӧ����ÿ����Ⱦ����֮ǰʹ��GL_DEPTH_BUFFER_BIT�������Ȼ��壬�����������ʹ����һ����Ⱦ�����е�д������ֵ��
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
��ĳЩ����������Ҫ������Ƭ�ζ�ִ����Ȳ��Բ�������Ӧ��Ƭ�Σ�����ϣ��������Ȼ��塣��������˵������ʹ��һ��ֻ����(Read-only)��Ȼ��塣OpenGL�������ǽ�����Ȼ����д�룬ֻ��Ҫ���������������(Depth Mask)����ΪGL_FALSE�Ϳ����ˣ�
glDepthMask(GL_FALSE);

��Ȳ��Ժ���
OpenGL���������޸���Ȳ�����ʹ�õıȽ������������������������OpenGLʲôʱ���ͨ������һ��Ƭ�Σ�ʲôʱ��ȥ������Ȼ��塣���ǿ��Ե���glDepthFunc���������ñȽ������������˵��Ⱥ���(Depth Function)����
glDepthFunc(GL_LESS);
GL_ALWAYS	��Զͨ����Ȳ���
GL_NEVER	��Զ��ͨ����Ȳ���
GL_LESS	��Ƭ�����ֵС�ڻ�������ֵʱͨ������
GL_EQUAL	��Ƭ�����ֵ���ڻ����������ֵʱͨ������
GL_LEQUAL	��Ƭ�����ֵС�ڵ��ڻ����������ֵʱͨ������
GL_GREATER	��Ƭ�����ֵ���ڻ����������ֵʱͨ������
GL_NOTEQUAL	��Ƭ�����ֵ�����ڻ����������ֵʱͨ������
GL_GEQUAL	��Ƭ�����ֵ���ڵ��ڻ����������ֵʱͨ������

����Ļ�ռ��з����Ե����ֵ�任���������ֵ������Ƭ����ɫ�����£�
#version 330 core
out vec4 FragColor;
float near = 0.1;
float far  = 100.0;
float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC
	return (2.0 * near * far) / (far + near - z * (far - near));
}
void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far; // Ϊ����ʾ���� far
	FragColor = vec4(vec3(depth), 1.0);
}

��ȳ�ͻ
һ���ܳ������Ӿ������������ƽ����������ηǳ����ܵ�ƽ��������һ��ʱ�ᷢ������Ȼ���û���㹻�ľ���������������״�ĸ���ǰ�档���������������״���ϵ����л�ǰ��˳����ᵼ�º���ֵĻ��ơ�������������ȳ�ͻ(Z-fighting)����Ϊ��������������������״������(Fight)˭�ô��ڶ��ˡ�
������һֱʹ�õĳ����У��м����ط�����ȳ�ͻ���Ƿǳ����Եġ����ӱ������ڵذ��ͬһ�߶��ϣ���Ҳ����ζ�����ӵĵ���͵ذ��ǹ����(Coplanar)��������������ֵ����һ���ģ�������Ȳ���û�а취����Ӧ����ʾ��һ����
��ֹ��ȳ�ͻ
��һ��Ҳ������Ҫ�ļ�������Զ��Ҫ�Ѷ������ڵ�̫���������������ǵ�һЩ�����λ��ص���ͨ������������֮������һ���û��޷�ע�⵽��ƫ��ֵ���������ȫ��������������֮�����ȳ�ͻ�������Ӻ͵ذ�������У����ǿ��Խ�����������y����΢�ƶ�һ�㡣����λ�õ����΢С�ı佫��̫���ܱ�ע�⵽�������ܹ���ȫ������ȳ�ͻ�ķ�����
�ڶ��������Ǿ����ܽ���ƽ������ԶһЩ����ǰ�������ᵽ�˾����ڿ�����ƽ��ʱ�Ƿǳ��ߵģ�����������ǽ���ƽ��Զ��۲��ߣ����ǽ��������ƽ��ͷ�����Ÿ���ľ��ȡ�Ȼ��������ƽ������̫Զ���ᵼ�½��������屻�ü�����������ͨ����Ҫʵ���΢�����������ʺ���ĳ����Ľ�ƽ����롣

��Ƭ����ɫ��������һ��Ƭ��֮��ģ�����(Stencil Test)�Ὺʼִ�У�����Ȳ���һ������Ҳ���ܻᶪ��Ƭ�Ρ�����������������Ƭ�λ������Ȳ��ԣ������ܻᶪ�������Ƭ�Ρ�ģ������Ǹ�����һ�����������еģ�������ģ�建��(Stencil Buffer)
һ��ģ�建���У���ͨ����ÿ��ģ��ֵ(Stencil Value)��8λ�ġ�����ÿ������/Ƭ��һ������256�ֲ�ͬ��ģ��ֵ�����ǿ��Խ���Щģ��ֵ����Ϊ������Ҫ��ֵ��Ȼ��ĳһ��Ƭ����ĳһ��ģ��ֵ��ʱ�����ǾͿ���ѡ�������Ǳ������Ƭ���ˡ�
ģ�建��������Ϊ�Զ�������
ģ�建�����������������ȾƬ��ʱ��ģ�建���趨Ϊһ���ض���ֵ��ͨ������Ⱦʱ�޸�ģ�建������ݣ�����д����ģ�建�塣��ͬһ�������߽������ģ���Ⱦ�����У����ǿ��Զ�ȡ��Щֵ���������������Ǳ���ĳ��Ƭ�Ρ�
����Ĳ������£�
����ģ�建���д�롣
��Ⱦ���壬����ģ�建������ݡ�
����ģ�建���д�롣
��Ⱦ�����������壬��θ���ģ�建������ݶ����ض���Ƭ�Ρ�

���������GL_STENCIL_TEST������ģ����ԡ�����һ�д���֮�����е���Ⱦ���ö�����ĳ�ַ�ʽӰ����ģ�建�塣
glEnable(GL_STENCIL_TEST);
ע�⣬����ɫ����Ȼ���һ������Ҳ��Ҫ��ÿ�ε���֮ǰ���ģ�建�塣
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

����Ȳ��Ե�glDepthMask����һ����ģ�建��Ҳ��һ�����Ƶĺ�����glStencilMask������������һ��λ����(Bitmask)�������뽫Ҫд�뻺���ģ��ֵ������(AND)���㡣Ĭ����������õ�λ��������λ��Ϊ1����Ӱ���������������ǽ�������Ϊ0x00��д�뻺�������ģ��ֵ��󶼻���0.������Ȳ����е�glDepthMask(GL_FALSE)�ǵȼ۵ġ�
glStencilMask(0xFF); // ÿһλд��ģ�建��ʱ������ԭ��
glStencilMask(0x00); // ÿһλ��д��ģ�建��ʱ������0������д�룩

ģ�庯��
����Ȳ���һ�������Ƕ�ģ�建��Ӧ��ͨ������ʧ�ܣ��Լ���Ӧ�����Ӱ��ģ�建�壬Ҳ����һ�����Ƶġ�һ�������������ܹ���������ģ����ԣ�glStencilFunc��glStencilOp��
glStencilFunc(GLenum func, GLint ref, GLuint mask)һ����������������
func������ģ����Ժ���(Stencil Test Function)��������Ժ�������Ӧ�õ��Ѵ����ģ��ֵ�Ϻ�glStencilFunc������refֵ�ϡ����õ�ѡ���У�GL_NEVER��GL_LESS��GL_LEQUAL��GL_GREATER��GL_GEQUAL��GL_EQUAL��GL_NOTEQUAL��GL_ALWAYS�����ǵ��������Ȼ���ĺ������ơ�
ref��������ģ����ԵĲο�ֵ(Reference Value)��ģ�建������ݽ��������ֵ���бȽϡ�
mask������һ�����룬��������ο�ֵ�ʹ����ģ��ֵ�ڲ��ԱȽ�����֮ǰ������(AND)���㡣��ʼ���������λ��Ϊ1��
��һ��ʼ���Ǹ��򵥵�ģ�������У�����������Ϊ��
glStencilFunc(GL_EQUAL, 1, 0xFF)
������OpenGL��ֻҪһ��Ƭ�ε�ģ��ֵ����(GL_EQUAL)�ο�ֵ1��Ƭ�ν���ͨ�����Բ������ƣ�����ᱻ������

����glStencilFunc����������OpenGLӦ�ö�ģ�建��������ʲô������������Ӧ����θ��»��塣�����ҪglStencilOp��������ˡ�
glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)һ����������ѡ������ܹ��趨ÿ��ѡ��Ӧ�ò�ȡ����Ϊ��
sfail��ģ�����ʧ��ʱ��ȡ����Ϊ��
dpfail��ģ�����ͨ��������Ȳ���ʧ��ʱ��ȡ����Ϊ��
dppass��ģ����Ժ���Ȳ��Զ�ͨ��ʱ��ȡ����Ϊ��
ÿ��ѡ�����ѡ�����µ�����һ����Ϊ��
��Ϊ			����
GL_KEEP		���ֵ�ǰ�����ģ��ֵ
GL_ZERO		��ģ��ֵ����Ϊ0
GL_REPLACE		��ģ��ֵ����ΪglStencilFunc�������õ�refֵ
GL_INCR			���ģ��ֵС�����ֵ��ģ��ֵ��1
GL_INCR_WRAP	��GL_INCRһ���������ģ��ֵ���������ֵ�����
GL_DECR		���ģ��ֵ������Сֵ��ģ��ֵ��1
GL_DECR_WRAP	��GL_DECRһ���������ģ��ֵС��0��������Ϊ���ֵ
GL_INVERT		��λ��ת��ǰ��ģ�建��ֵ
Ĭ�������glStencilOp������Ϊ(GL_KEEP, GL_KEEP, GL_KEEP)�ģ����Բ����κβ��ԵĽ������Σ�ģ�建�嶼�ᱣ������ֵ��Ĭ�ϵ���Ϊ�������ģ�建�壬�����������д��ģ�建��Ļ�������Ҫ���ٶ�����һ��ѡ�����ò�ͬ��ֵ��
���ԣ�ͨ��ʹ��glStencilFunc��glStencilOp�����ǿ��Ծ�ȷ��ָ������ģ�建���ʱ������Ϊ�ˣ�����Ҳ����ָ��ʲôʱ�����ģ�建��ͨ������ʲôʱ��Ƭ����Ҫ��������

��������������
�����������������������������������������������ǽ���Ϊÿ��������һ����������������Χ����һ����С����ɫ�߿򡣵�����Ҫ�ڲ�����Ϸ��ѡ��һ����λ���в����ģ���Ҫ�������ѡ�е����ĸ���λ��ʱ�����Ч���ͷǳ������ˡ�Ϊ���崴�������Ĳ������£�
1.�ڻ��ƣ���Ҫ��������ģ�����֮ǰ����ģ�庯������ΪGL_ALWAYS��ÿ�������Ƭ�α���Ⱦʱ����ģ�建�����Ϊ1��
2.��Ⱦ���塣
3.����ģ��д���Լ���Ȳ��ԡ�
4.��ÿ����������һ��㡣
5.ʹ��һ����ͬ��Ƭ����ɫ�������һ�������ģ��߿���ɫ��
6.�ٴλ������壬��ֻ������Ƭ�ε�ģ��ֵ������1ʱ�Ż��ơ�
7.�ٴ�����ģ��д�����Ȳ��ԡ�


*/

/* matrix */
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

/* vector */
glm::vec2 rec_pos = glm::vec2(0.0f);
glm::vec3 lightPos = glm::vec3(1.2f, 1.3f, -1.5f);

/* camera attributes */
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float alpha = 0.4f;
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float cubeVertices[] = {
	// positions          // texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
float planeVertices[] = {
	// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
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

glm::vec3 pointLightPositions[] =
{
	glm::vec3(0.7f, 0.2f, 2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(4.0f, 2.0f, -12.0f),
	glm::vec3(0.0f, 0.0f, -3.0f)
};


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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
	}
	
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// build and compile shaders
	Shader shader("res/Shaders/Shader_advancedOpenGL/1/stencil_testing.vert", "res/Shaders/Shader_advancedOpenGL/1/stencil_testing.frag");
	Shader shaderSingleColor("res/Shaders/Shader_advancedOpenGL/1/stencil_testing.vert", "res/Shaders/Shader_advancedOpenGL/1/stencil_single_color.frag");

	/*imGUI*/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	ImGui::StyleColorsDark();

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	unsigned int cubeTexture = load_texture("res/textures/container.png");
	unsigned int floorTexture = load_texture("res/textures/marble.png");

	shader.use();
	shader.setInt("texture1", 0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		float time = (float)glfwGetTime();
		float xValue = (float)(0.5 * std::sin(time) + 0.5);
		float yValue = (float)(0.5 * std::cos(time) + 0.5);
		float camX = std::sin(time) * 5.0f;
		float camZ = std::cos(time) * 5.0f;
		deltaTime = time - lastFrame;
		lastFrame = time;

		/*start new frame for imGUI*/
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // don't forget to clear the stencil buffer!

		// set uniforms
		shaderSingleColor.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shaderSingleColor.setMat4("view", view);
		shaderSingleColor.setMat4("projection", projection);

		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		// draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
		glStencilMask(0x00);
		// floor
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// 1st. render pass, draw objects as normal, writing to the stencil buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
		// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
		// the objects' size differences, making it look like borders.
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		shaderSingleColor.use();
		float scale = 1.1f;
		// cubes
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		shaderSingleColor.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		shaderSingleColor.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);

		/*ImGui*/
		float FPS = ImGui::GetIO().Framerate;
		ImGui::Begin("Gui");
		ImGui::Text("FPS = %f", FPS);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}