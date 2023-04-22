#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
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
#include "Environment.h"

struct Type_flow
{
	int type_index;			//����С���ţ����嵽��������ƣ���1��ʼ����
	int type_type;			//��������ţ�1Ϊ����������2Ϊ��Դ������3Ϊ��ҵ����
	std::string type_name;	//�ѻ�����
};

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
	friend class Record;
public:
	Flow();
	void reset();						//����
	bool window_confirm;				//���̸�������ȷ�ϴ���
	void initGuiStyle();					//��ʽ��ʼ��
	void show_ground();					//��ʾ�ؿ�����ѡ�����
	void add_type(Message& message, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Web& web);																//��ӻ�������
	void showGui(Message& message, Energy& energy, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web);										//չʾ���̰�ť
	void weatherCheck(Message& message, Environment& environment, Energy& energy, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web);	//�������
	void train_check(Energy& energy, int end_train_1, int end_train_2, TrainLoader& train, Yard& yard, Web& web);																				//��ж����ֹ���
	void stop_yard_flow(Energy& energy, std::string name_wheel, Berth& berth, TrainLoader& train, Silo& silo, Web& web);																			//�ѳ�ȡ���þ�����������ֹͣ
	void stop_silo_flow(Energy& energy, Yard& yard, Web& web);																																		//Ͳ�ֶ�������ͣ��
	void ship_leave(Energy& energy, int berth_idx, Yard& yard, Web& web);																															//����ͻȻ�벴���������̿�������
	void end_shiploading(Energy& energy, Berth& berth, Web& web);																																	//װ������
	void end_shipunloading(Message& message, Energy& energy, int berth_finished, Conveyor& conv, SlewingWheel& wheel, Berth& berth, Yard& yard);												//ж������
	void trip_end(bool all, std::vector<std::string> equipments, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web);							//��ѹ����բ����һ������Ϊtrue��ʾ������բ
	void end_web(Conveyor& conv, SlewingWheel& wheel, std::string finishEndName);																													//����������ʹƤ������ɫ��˸�����ֻ����ɫ

private:
	ImGuiStyle* style;					//ImGui��ʽ�洢
	Type_flow type_unit;				//���ﵥԪ����������������ż�������
	std::vector<Type_flow> names;		//���������������ƴ洢����
	FlowAttrib flow_attr;				//����
	std::vector<FlowAttrib> flows;		//����״̬�洢����
	std::vector<std::string> reasons;	//��ͣԭ����ʷ�洢����
	bool windowGroundChoose;			//�ؿ�װ������ѡ�񴰿�
	bool groundSelected;				//�ؿ�װ����ѡ��
	int chooseType;						//�ؿ�ѡ������
	int chooseIndex;					//�ؿ�ѡ��С��
	int onfocus;							//��ǰ��������
	bool emergency_stop;				//���̼�ͣ����
	void setText(bool temp);			//���ð�ť������ɫ
	void showButton(FlowAttrib& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web);	//���ư�ť
	void pressButton(FlowAttrib& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web);	//�������̰�ť
	void save_reason(std::string reason);
	void load_reason(Message& message);

};
