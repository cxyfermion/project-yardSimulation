#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "res/GL_heads/Camera.hpp"
#include "res/GL_heads/Shader.h"
#include "SimuCore.h"
#include "Message.h"

/*
child_type子堆场堆货种类：
1碎石		2水渣		3工业盐		4纸浆		5黄砂		6铜矿		7石油焦			8焦煤		9化工煤		10混煤		12优混		13无烟煤	14沥青
1大混		2蒙混		3大同		4大精		5陕煤		6俄罗斯煤	7俄罗斯无烟	8兴煤
1新中港	2神华精		3伊泰		4天马		5特底灰		6神混		7神优			8优一		9神华		10富动

type
0 vec3(1.0f)
1 vec3(0.3f)
2 vec3(0.4f)
3 vec3(0.85f)
4 vec3(0.6f)
5 vec3(0.7f, 0.7f, 0.0f)
6 vec3(0.9f, 0.9f, 0.4f)
7 vec3(0.0f)

子堆场右上角的点反应右边的斗轮机与其交互状态：0无交互（与右下角一致），1正装货（绿色），2正取货（蓝色）
子堆场左上角的点反应左边的斗轮机与其交互状态：0无交互（与右下角一致），1正装货（绿色），2正取货（蓝色）
（可以用纹理来区分）子堆场其他点反映当前堆场的货物种类：无货物为白色，种类在第二行则显示为0.0f的黑，种类在第三行则显示为0.0f的黑，
碎石显示为0.5f的灰，水渣显示为0.7f的灰，黄砂显示为暗黄，工业盐显示为0.85f的灰色，铜矿显示为亮黄，其余煤显示为0.2f黑色

当取料堆当前容量低于低位报警阈值，显示屏上材料（即子堆场下半部分）会闪烁

堆料申请失败可能原因：
1.该场地已有另一种货物
2.该场地的货物加上申请量后超过堆放能力上限

港区去炼化的煤二种，一是大混1号，二是化工煤。

*/

struct Type_yard
{
	int type_index;			//货物小类编号，具体到货物的名称，从1开始计数
	int type_type;			//货物大类编号，1为大类命名，2为来源命名，3为企业命名
	std::string type_name;	//堆货名称
};

struct Yard_child
{
	//子堆场结构体
	int child_name;				//子堆场序号，从1开始
	float child_max_amount;	//子堆场最大堆货量
	float child_cur_amount;	//子堆场当前堆货量
	float child_flux_left;		//子堆场变动流量
	float child_flux_right;		//子堆场变动流量
	bool child_focus;			//子堆场选择注意变量
	int child_type;				//子堆场堆货大类（0表示未定义）
	int child_index;			//子堆场堆货小类（0表示未定义）
	int child_state_left;		//子堆场西端运行模式：0无交互（显示堆场货物材质），1正堆料（绿色），2正取料（蓝色）
	int child_state_right;		//子堆场东端运行模式：0无交互（显示堆场货物材质），1正堆料（绿色），2正取料（蓝色）
};

struct Yard_father
{
	//父堆场结构体
	std::string yard_name;						//堆场代号
	int yard_index;								//堆场序号（从1开始）
	std::vector<Yard_child> children;			//子堆场
	std::vector<std::string> wheel_available;	//堆场的连接斗轮机
	float center_x;								//堆场中心x坐标
	float center_y;								//堆场中心y坐标
	bool yard_focus;							//堆场注意状态设置
	float yard_coords[7];						//堆场坐标：x,y,z,state_zuo,state_you,type,focus
};

class FindYardG
{
public:
	bool operator ()(Yard_father father)
	{
		return father.yard_name == "G";
	}
};

class FindYardH
{
public:
	bool operator ()(Yard_father father)
	{
		return father.yard_name == "H";
	}
};

class FindYardK
{
public:
	bool operator ()(Yard_father father)
	{
		return father.yard_name == "K";
	}
};

class FindYardWEST2
{
public:
	bool operator ()(Yard_father father)
	{
		return father.yard_name == "WEST2";
	}
};

//堆场
class Yard
{
public:
	Yard();
	void reset(SimuCore& core, bool rand_init);		//重置，rand_init为true表示启动随机初始化
	void yard_random_initiator(SimuCore& core);	//堆场随机初始化
	std::string terminate_wheel;					//因堆料取尽而被关闭的斗轮机代号
	int yard_choosed;								//选中的堆场
	int child_choosed;								//选中的子堆场
	int type_choosed;								//选中子堆场的货物大类
	int index_choosed;								//选中子堆场的货物小类
	float amount_choosed;							//选中子堆场的货物存量
	bool data_send;									//数据是否发送
	bool yard_choosing;								//是否开启堆场选择窗口
	void drawYard(Camera& camera, Shader& yardShader);
	void initGuiStyle();								//样式初始化
	void yard_dispatch(Message& message, bool unreal);				//ImGui堆场状态控制台：可以添加新的货物品种，获取堆场当前状态信息，参数为true表示有强制指定权
	void add_type(std::string str_name, int type_type);		//添加货物新类
	bool start_stack(Message& message, std::vector<std::string>& equipments, int yard, int child, int type, int index, float amount, float flux);	//开始堆料，参数为堆场号，子场号，货物类，申请堆量，流量。返回false为堆料申请失败
	bool start_reclaim(Message& message, std::vector<std::string>& equipments, int yard, int child, int type, int index, float flux);				//开始取料，返回false为取料申请失败
	void yard_end(std::vector<std::string>& equipments);		//终止堆场行动（扫描输入设备中的斗轮机，并设置相应堆场为0状态）
	bool type_check(int type, int index);			//检查是否存在相应货物类的定义，返回true表示存在定义
	bool updateYards(float simurate);				//返回0，无事；返回1，调取关闭斗轮机代号并终结对应流程
	void yard_choose(Message& message);								//选择堆场界面
	void set_focus(std::vector<std::string>& equipments);
	void lose_focus();
	void send_reset();								//发送信号复位

private:
	int flow_yard_choosed_1;				//由流程选中的堆场1号
	int flow_yard_choosed_2;				//由流程选中的堆场2号
	int flow_num_choose;					//由流程选中的堆场的数量
	ImGuiStyle* style;						//ImGui样式
	Type_yard type_unit;					//货物单元，包含货物种类序号及其名称
	std::vector<Type_yard> names;			//货物种类编号与名称存储容器
	Yard_child child;						//子堆场
	Yard_father father;						//堆场
	std::vector<Yard_father> yards;			//堆场容器
	unsigned int yardVBO, yardVAO;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	void showYardInfo(int yard_area, bool unreal);		//堆场信息显示
	void chooseYardInfo(std::vector<Yard_father>::iterator& it_begin, std::vector<Yard_father>::iterator& it_end, int schoose);	//堆场选择信息显示
	void pre_button(bool choosed);						//堆场选择按钮样式设置
	void post_button();

};
