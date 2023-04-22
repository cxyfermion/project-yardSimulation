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
#include "Record.h"

class Console
{
	friend class Record;
public:
	Console();
	int init();
	void run();

private:
	GLFWwindow* window;
	bool random_initiating;		//随机初始化选择位
	int compitence;				//权限：0游客，1卸船机驾驶员，2船舶调度员，3火车调度员，4火车船舶总调度员，5流程管理员，6仿真人员，7堆场管理员，8筒仓管理员，9变电所管理员，10超级管理员，11气象员，12上帝
	std::string names[13];
	std::string descriptions[13];
	void identity_choose();		//权限选择

};
