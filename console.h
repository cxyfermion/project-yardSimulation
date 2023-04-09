#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Flow.h"
#include "Environment.h"

class Console
{
public:
	Console();
	int init();
	void run();

private:
	GLFWwindow* window;
	bool random_initiating;		//�����ʼ��ѡ��λ
	int compitence;				//Ȩ�ޣ�0�οͣ�1ж������ʻԱ��2��������Ա��3�𳵵���Ա��4�𳵴����ܵ���Ա��5���̹���Ա��6������Ա��7�ѳ�����Ա��8Ͳ�ֹ���Ա��9��������Ա��10����Ա��11�ϵ�
	std::string names[12];
	std::string descriptions[12];
	void identity_choose();		//Ȩ��ѡ��

};