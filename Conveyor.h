#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include <glm/glm.hpp>
#include <imgui.h>
#include "res/GL_heads/Camera.hpp"
#include "res/GL_heads/Shader.h"
#include "Web.h"

/*
* 注意：sort算法中的比较仅支持类似<，不支持<=
所有设备状态都转蓝色后，该皮带可以空载运行，也就是浅绿色运行状态中。流程参与设备都渐次空载运行后，最末位设备允许卸煤，就完成了负载运行，选中的所有设备颜色变为深绿色，进入负载运行状态

中控室的流程指令第一是某某号流程选择指令，在某某号流程的所有设备都正常后，参与流程的所有设备在显示屏上都显示蓝色的闪烁信号，提示中控室按下待机运行按钮，然后个设备依照次序待机运行，也就是空载运行，颜色依次变为浅绿色
某某号流程参与设备全部待机运行完毕。中控室按下负载运行，各设备颜色变为深绿色，并给流程最后设备一个可以运送煤炭的信号。

BC7A和BC7B全部合并为BC7
XBC1和XBC2表示为XC1和XC2
*/

struct Type_conv
{
	int type_index;			//货物小类编号，具体到货物的名称，从1开始计数
	int type_type;			//货物大类编号，1为大类命名，2为来源命名，3为企业命名
	std::string type_name;	//堆货名称
};

struct ConvAttrib
{
	std::string conv_name;		//皮带代号
	int conv_state;				//皮带运行状态，-1灰色系统离线（停机状态），0红色未上电或故障，1蓝色皮带在现场已经通电，各项保护设施已经正常，告诉中控室该皮带可以进入下步流程环节，技术术语叫做皮带待机完成，2浅绿色运行（空载），3深绿运行（负载）
	int conv_idx;				//皮带序号
	int conv_pow;				//皮带运行功率
	float conv_speed;			//带速
	int conv_length;			//皮带长度（米）
	float conv_max_storage;	//皮带最高缓存容量，由皮带长度决定
	float conv_cur_storage;		//皮带当前容量
	int flow_name;				//占用流程序号（由原流程名字stoi转换为int），-1为未占用
	int flow_idx;				//在占用流程中的级次（第几位）（从0开始）
	int conv_type;				//货大类（0表示未定义）
	int conv_index;				//货小类（0表示未定义）
};

struct FlowState
{
	int flow_num;		//流程代号
	bool begin;			//流程起始是否在供给货物，true是
};

class ConvSort
{
public:
	bool operator()(ConvAttrib conv_1, ConvAttrib conv_2)
	{
		return conv_1.flow_idx < conv_2.flow_idx;
	}
};

class ConvResort
{
public:
	bool operator()(ConvAttrib conv_1, ConvAttrib conv_2)
	{
		return conv_1.conv_idx < conv_2.conv_idx;
	}
};

class Conveyor
{
public:
	Conveyor();
	void reset();																					//重置
	void draw(Camera& camera, Shader& convShader, float value);									//绘制
	void initGuiStyle();																				//样式初始化
	void conv_dispatch(bool unreal);																//流程状态显示
	void updateConvs(float simurate);																//更新皮带
	void add_type(std::string str_name, int type_type);											//添加货物新类
	bool startCheck(std::vector<std::string>& equipments);											//检查传入的设备代号容器是否都空闲且物料输送请求一致，返回false为禁止启动，返回true为允许启动
	bool sceneCheck(std::vector<std::string>& equipments);											//现场检查
	void run_unloaded(std::vector<std::string>& equipments);										//空载运行
	bool runCheck(int type, int index, std::vector<std::string>& equipments);						//检查对应的皮带设备中是否存在冲突的货物类，返回false表示不允许启动
	void run_loaded(int index_flow, int type, int index, std::vector<std::string>& equipments);	//负载运行
	void shutDown(int index_flow);																	//流程停止
	void cancel(std::vector<std::string>& equipments);												//流程取消
	bool cancelCheck(std::vector<std::string>& equipments);											//流程取消检查，返回true表示允许取消本流程
	void emergency_shutDown(int index_flow, std::vector<std::string>& equipments);				//流程急停
	void set_focus(std::vector<std::string>& equipments);											//选中流程的显示
	void lose_focus();																				//失焦后的流程显示

private:
	ImGuiStyle* style;					//ImGui样式
	Type_conv type_unit;				//货物单元，包含货物种类序号及其名称
	std::vector<Type_conv> names;		//货物种类编号与名称存储容器
	ConvAttrib conv;					//皮带状态结构体
	float conv_coords[459];				//皮带坐标VBO，数据x1,y1,x2,y2,z,r,g,b,focus
	std::vector<ConvAttrib> belts;		//皮带数据存储容器
	unsigned int convVBO, convVAO;		//VAO & VBO
	FlowState flow_state;				//流程状态存储结构体
	std::vector<FlowState> act_flows;	//激活的流程状态
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	void pre_button();
	void post_button();

};
