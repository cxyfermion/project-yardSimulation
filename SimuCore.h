#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <random>

/*
* glfwGetTime()返回程序运行的秒数，类型为double
若要timer机制正常工作，必须使用glfwPollEvents()调用，如果使用glfwWaitEvents()则timer无法正常工作。
原因在于glfwPollEvents不管队列中有没有window event都是立即返回并恢复主线程执行。
而glfwWaitEvents则必须等待至少有一个window event进入到对列中，然后才会返回并恢复主线程。

*/

//仿真核心
class SimuCore
{
public:
	SimuCore();
	void initGuiStyle();								//样式初始化
	void simulator_gui();							//仿真控制界面
	void base_info();								//基本信息显示界面
	void updateTime();								//更新系统时间
	float genRandomFloat(float min, float max);		//随机生成一个float
	int genRandomInt(int min, int max);				//随机生成一个int
	bool dice(float probability);					//概率判定
	double time;									//程序总运行时间（无论是否暂停）
	double simuTime;								//仿真运行时间
	bool simu_pause;								//程序状态，true为暂停，false为运行
	float run_rate;									//程序运行速度
	bool stepping;									//步进状态
	bool reset_zero;								//重置——零状态
	bool reset_rand;								//重置——随机，若两个随机都被激活，优先进行随机重置
	int runtime_minutes;							//仿真运行时间-分
	int runtime_hours;								//仿真运行时间-时

private:
	double simu_deltaTime;
	double simu_lastFrame;
	double terminate_time;							//程序终结时间
	double terminate_countdown;					//程序终结倒计时
	int runtime_seconds;							//仿真运行时间-秒
	int runtime_days;								//仿真运行时间-日
	std::default_random_engine generator;			//随机数生成器
	ImGuiStyle* style;								//ImGui样式
	void fresh_time();								//计算当前仿真时间
	void pre_button(bool choosed);					//堆场选择按钮样式设置
	void post_button();

};
