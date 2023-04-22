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
	int type_index;			//货物小类编号，具体到货物的名称，从1开始计数
	int type_type;			//货物大类编号，1为大类命名，2为来源命名，3为企业命名
	std::string type_name;	//堆货名称
};

struct FlowAttrib
{
	std::string flow_name;					//流程代号
	int flow_idx;							//流程序号，由0到112
	int flow_state;							//流程状态，0白色未启用，1红色流程被选中等待现场就绪，2绿色空载运行，3深绿色负载运行
	unsigned int flow_time;				//流程总计运行时间
	bool scene_ready;						//现场就绪信号
	std::vector<std::string> equipments;	//当前流程包含的所有设备的代号
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
	void reset();						//重置
	bool window_confirm;				//流程负载启动确认窗口
	void initGuiStyle();					//样式初始化
	void show_ground();					//显示地坑种类选择界面
	void add_type(Message& message, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Web& web);																//添加货物种类
	void showGui(Message& message, Energy& energy, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web);										//展示流程按钮
	void weatherCheck(Message& message, Environment& environment, Energy& energy, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web);	//天气检查
	void train_check(Energy& energy, int end_train_1, int end_train_2, TrainLoader& train, Yard& yard, Web& web);																				//火车卸货中止检查
	void stop_yard_flow(Energy& energy, std::string name_wheel, Berth& berth, TrainLoader& train, Silo& silo, Web& web);																			//堆场取料用尽触发的流程停止
	void stop_silo_flow(Energy& energy, Yard& yard, Web& web);																																		//筒仓堆满或达标停机
	void ship_leave(Energy& energy, int berth_idx, Yard& yard, Web& web);																															//船舶突然离泊触发的流程空载运行
	void end_shiploading(Energy& energy, Berth& berth, Web& web);																																	//装船结束
	void end_shipunloading(Message& message, Energy& energy, int berth_finished, Conveyor& conv, SlewingWheel& wheel, Berth& berth, Yard& yard);												//卸船结束
	void trip_end(bool all, std::vector<std::string> equipments, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web);							//变压器跳闸，第一个参数为true表示主变跳闸
	void end_web(Conveyor& conv, SlewingWheel& wheel, std::string finishEndName);																													//物流网结束使皮带变蓝色闪烁，斗轮机变红色

private:
	ImGuiStyle* style;					//ImGui样式存储
	Type_flow type_unit;				//货物单元，包含货物种类序号及其名称
	std::vector<Type_flow> names;		//货物种类编号与名称存储容器
	FlowAttrib flow_attr;				//流程
	std::vector<FlowAttrib> flows;		//流程状态存储容器
	std::vector<std::string> reasons;	//急停原因历史存储容器
	bool windowGroundChoose;			//地坑装车种类选择窗口
	bool groundSelected;				//地坑装车已选定
	int chooseType;						//地坑选定大类
	int chooseIndex;					//地坑选定小类
	int onfocus;							//当前焦点流程
	bool emergency_stop;				//流程急停窗口
	void setText(bool temp);			//设置按钮字体颜色
	void showButton(FlowAttrib& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web);	//绘制按钮
	void pressButton(FlowAttrib& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web);	//单机流程按钮
	void save_reason(std::string reason);
	void load_reason(Message& message);

};
