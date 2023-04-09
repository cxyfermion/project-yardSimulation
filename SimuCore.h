#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <random>

/*
* glfwGetTime()���س������е�����������Ϊdouble
��Ҫtimer������������������ʹ��glfwPollEvents()���ã����ʹ��glfwWaitEvents()��timer�޷�����������
ԭ������glfwPollEvents���ܶ�������û��window event�����������ز��ָ����߳�ִ�С�
��glfwWaitEvents�����ȴ�������һ��window event���뵽�����У�Ȼ��Ż᷵�ز��ָ����̡߳�

*/

//�������
class SimuCore
{
public:
	SimuCore();
	void initGuiStyle();								//��ʽ��ʼ��
	void simulator_gui();							//������ƽ���
	void base_info();								//������Ϣ��ʾ����
	void updateTime();								//����ϵͳʱ��
	float genRandomFloat(float min, float max);		//�������һ��float
	int genRandomInt(int min, int max);				//�������һ��int
	bool dice(float probability);					//�����ж�
	double time;									//����������ʱ�䣨�����Ƿ���ͣ��
	double simuTime;								//��������ʱ��
	bool simu_pause;								//����״̬��trueΪ��ͣ��falseΪ����
	float run_rate;									//���������ٶ�
	bool stepping;									//����״̬
	bool reset_zero;								//���á�����״̬
	bool reset_rand;								//���á���������������������������Ƚ����������
	int runtime_minutes;							//��������ʱ��-��
	int runtime_hours;								//��������ʱ��-ʱ

private:
	double simu_deltaTime;
	double simu_lastFrame;
	double terminate_time;							//�����ս�ʱ��
	double terminate_countdown;					//�����սᵹ��ʱ
	int runtime_seconds;							//��������ʱ��-��
	int runtime_days;								//��������ʱ��-��
	std::default_random_engine generator;			//�����������
	ImGuiStyle* style;								//ImGui��ʽ
	void fresh_time();								//���㵱ǰ����ʱ��
	void pre_button(bool choosed);					//�ѳ�ѡ��ť��ʽ����
	void post_button();

};