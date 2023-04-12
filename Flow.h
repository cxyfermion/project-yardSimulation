#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Conveyor.h"
#include "SlewingWheel.h"
#include "Berth.h"
#include "TrainLoader.h"
#include "Yard.h"
#include "Silo.h"
#include "SimuCore.h"
#include "Energy.h"

struct FlowAttrib
{
	std::string flow_name;					//���̴���
	int flow_idx;							//������ţ���0��112
	int flow_state;							//����״̬��0��ɫδ���ã�1��ɫ���̱�ѡ�еȴ��ֳ�������2��ɫ�������У�3����ɫ��������
	unsigned int flow_time;				//�����ܼ�����ʱ��
	bool scene_ready;						//�ֳ������ź�
	std::vector<std::string> equipments;	//��ǰ���̰����������豸�Ĵ���
};

class Myfind1
{
public:
	bool operator ()(FlowAttrib flow)
	{
		return flow.flow_name == "2101";
	}
};

class Myfind2
{
public:
	bool operator ()(FlowAttrib flow)
	{
		return flow.flow_name == "2181";
	}
};

class Myfind3
{
public:
	bool operator ()(FlowAttrib flow)
	{
		return flow.flow_name == "2179";
	}
};

class Myfind4
{
public:
	bool operator ()(FlowAttrib flow)
	{
		return flow.flow_name == "2191";
	}
};

class Myfind5
{
public:
	bool operator ()(FlowAttrib flow)
	{
		return flow.flow_name == "401";
	}
};

class Myfind6
{
public:
	bool operator ()(FlowAttrib flow)
	{
		return flow.flow_name == "451";
	}
};

class Myfind7
{
public:
	bool operator ()(FlowAttrib flow)
	{
		return flow.flow_name == "471";
	}
};

class Flow
{
public:
	Flow();
	void reset();						//����
	bool window_confirm;				//���̸�������ȷ�ϴ���
	void initGuiStyle();					//��ʽ��ʼ��
	void add_type(Message& message, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard);												//��ӻ�������
	void showGui(Message& message, Energy& energy, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo);					//չʾ���̰�ť
	void train_check(Energy& energy, int end_train_1, int end_train_2, Conveyor& conv, SlewingWheel& wheel, TrainLoader& train, Yard& yard);						//��ж����ֹ���
	void stop_yard_flow(Energy& energy, std::string name_wheel, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Silo& silo);					//�ѳ�ȡ���þ�����������ֹͣ
	void stop_silo_flow(Energy& energy, Conveyor& conv, SlewingWheel& wheel, Yard& yard);																				//Ͳ�ֶ�������ͣ��
	void ship_leave(Energy& energy, int berth_idx, Conveyor& conv, SlewingWheel& wheel, Yard& yard);																	//����ͻȻ�벴���������̿�������
	void end_shiploading(Energy& energy, Conveyor& conv, Berth& berth);																								//װ������
	void end_shipunloading(Message& message, Energy& energy, int berth_finished, Conveyor& conv, SlewingWheel& wheel, Berth& berth, Yard& yard);					//ж������
	void trip_end(bool all, std::vector<std::string> equipments, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo);		//��ѹ����բ����һ������Ϊtrue��ʾ������բ

private:
	ImGuiStyle* style;					//ImGui��ʽ�洢
	FlowAttrib flow_attr;				//����
	std::vector<FlowAttrib> flows;		//����״̬�洢����
	int onfocus;							//��ǰ��������
	bool emergency_stop;				//���̼�ͣ����
	void setText(bool temp);			//���ð�ť������ɫ
	void showButton(FlowAttrib& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo);	//���ư�ť
	void pressButton(FlowAttrib& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo);	//�������̰�ť

};
