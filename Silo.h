#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "res/GL_heads/Camera.hpp"
#include "res/GL_heads/Shader.h"
#include "SimuCore.h"

/*
炼化筒仓一个筒仓大概可以装煤一千吨。有14个筒仓
去往炼化的煤有2种，大混和化工煤
false为大混煤，true为化工煤
//14个筒仓正在堆料的数量最多为1个，正在卸料的最多为1个

*/

struct Silo_unit
{
	int silo_idx;			//筒仓序号，1-14
	float max_storage;		//筒仓容量上限
	float cur_storage;		//筒仓当前容量
	bool silo_type;			//材料类别，false为大混煤vec3(0.3f)，true为化工煤vec3(0.1f)
	float silo_flux;			//筒仓变动流量
	bool silo_focus;			//筒仓选择注意变量
	bool silo_state_up;		//筒仓顶部运行模式：0无交互（显示货物材质），1正堆料（绿色）
	bool silo_state_down;	//筒仓底部运行模式：0无交互（显示货物材质），1正卸料（绿色）
	float silo_coords[8];	//筒仓坐标：x,y,z,state_up,state_down,percent,type,focus
};

class Silo
{
public:
	Silo();
	void reset(SimuCore& core, bool rand_init);									//重置
	void silo_random_initiator(SimuCore& core);								//筒仓随机初始化
	bool data_send;																//数据是否发送
	bool select_window;															//是否显示筒仓选择界面
	float amount_apply;														//申请堆放量
	int selected_index;															//选中的筒仓
	void drawSilo(Camera& camera, Shader& siloShader);
	void initGuiStyle();
	void silo_dispatch(bool unreal);												//ImGui筒仓状态控制台：获取筒仓当前状态信息，参数为true表示有强制指定权
	void start_straight(float flux, bool type);									//直出部分的流程开启判断,参数0大混，1化工
	bool start_stack(int silo_index, bool type, float amount, float flux);		//开始堆料，参数为筒仓号，货物类（false为大混煤，true为化工煤），申请堆量，流量。返回false为堆料申请失败
	bool updateSilos(float gapTime, float simurate);							//返回0，无事；返回1，终结对应流程
	void updateStraight(float gapTime, float simurate);							//更新直出系统状态
	void silo_end_out();														//终止筒仓向炼化输出
	bool out_check();															//返回true，筒仓正在出煤；返回false，筒仓没有出煤
	void set_focus(std::vector<std::string>& equipments);
	void lose_focus();
	void show_select();															//显示筒仓选择界面，返回0为没有选择或选择失败，返回1-14表示选中的筒仓序号
	void start_silo_out(float flux, bool type);									//开始筒仓出
	bool check_silo_out(bool type);												//筒仓出检查
	void end_silo(std::vector<std::string>& equipments);						//结束筒仓出
	void send_reset();															//发送信号复位

private:
	ImGuiStyle* style;
	Silo_unit silo_unit;
	std::vector<Silo_unit> silos;
	bool silo_occupied[14];
	unsigned int siloVBO, siloVAO;
	float storage_targets;				//目标存量
	float storage_process;				//当前存量变化
	int straight_state;					//直出状态，0为不直出，1为直出大混，2为直出化工煤
	float straight_flux;				//直出流量
	unsigned int out_total_0;			//总出大混
	unsigned int out_total_1;			//总出化工煤
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	void pre_button(bool choosed);		//堆场选择按钮样式设置
	void post_button();

};
