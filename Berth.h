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
* 实际抓斗闲置位置在大料斗上方，即crab_bucket_hor = 1而不是-1
* 实际抓斗在crab_bucket_hor = 1位置就是在大料斗上方，和激活的皮带流程无关，显示界面中抓斗在A和B的区分是为了可视化
* 实际抓斗的运行路线不是直角的，在抓斗移动过程中可以一边水平移动，一边垂直移动，但将此模型简化为抓斗在两侧位置到达后直接上下料，因此抓斗bucket_speed_hor属性表示了抓斗水平移动即垂直到位之前的全过程，bucket_speed_ver表征抓斗抓煤和卸煤的速度，而不表示抓斗在垂直移动的速度。
船舶到达后，每个舱室将提供一定量的煤炭总量
抓斗在一次抓取过程中在抓取循环连续减少被抓料的舱室煤量
抓斗在卸煤过程中将煤转移至卸船机大料斗的容量中，连续间接变化
卸船机大料斗根据抓斗的运煤速度（也可以由当前大料斗内煤容量线性决定放煤速度）连续地向皮带输出煤
后续系统可添加：
	卸船机行走+船舶停靠位置扰动
	船卸煤在前期高容量时容易卸，卸速快，而后减慢
	抓斗在6个阶段移动速度的区分

船只泊位调度逻辑：
4泊位船舶小于等于2万吨
21#22#泊位：
	若任一泊位停了6.5万吨级别的船，另一泊位不能靠泊
	若一泊位停了5万吨的船，另一泊位只能停2万吨及以下的船
	若一泊位停了3.5万吨的船，另一泊位只能停3.5万吨及以下的船

1碎石		2水渣		3工业盐		4纸浆		5黄砂		6铜矿		7石油焦			8焦煤		9化工煤		10混煤		12优混		13无烟煤	14沥青
1大混		2蒙混		3大同		4大精		5陕煤		6俄罗斯煤	7俄罗斯无烟	8兴煤
1新中港	2神华精		3伊泰		4天马		5特底灰		6神混		7神优			8优一		9神华		10富动

*/

struct Type_ship
{
	int type_index;			//货物小类编号，具体到货物的名称，从1开始计数
	int type_type;			//货物大类编号，1为大类命名，2为来源命名，3为企业命名
	std::string type_name;	//堆货名称
};

struct ShipUnloader
{
	std::string unloader_name;		//卸船机编号
	int unloader_index;				//卸船机序号
	int unloader_state;				//卸船机状态，0红色未运行，1蓝色设备就绪，2绿色负载运行
	unsigned int unloader_time;	//卸船机总计运行时间
	float crab_bucket_hor;			//抓斗水平位置，-1.0f闲置，0.0f在船上，1.0f在皮带上
	float crab_bucket_ver;			//抓斗垂直位置，0.0f表示降到位，1.0f表示升到位
	bool crab_bucket_switch;		//门机抓斗位于哪条皮带，0A皮带，1B皮带
	bool crab_bucket_loaded;		//抓斗已取料信号
};

struct Pos_bucket
{
	int ship;		//船舶位置
	int conv_A;		//A皮带位置
	int conv_B;		//B皮带位置
	int idle;		//闲置位置
};

struct ShipLoader
{
	std::string loader_name;		//装船机编号
	int loader_state;				//装船机状态，0红色未运行，1蓝色设备就绪，2绿色负载运行
	unsigned int loader_time;		//装船机总计运行时间
	int loader_pow;					//装船机运行功率
};

struct Ship
{
	std::string ship_name;					//船编号
	int ship_state;							//船只状态，0未到港（不显示），1到锚地等待（在锚地显示为红色），2正在靠泊（显示为绿色），3靠泊（显示为当前煤量百分比），4正在离港（显示为蓝色）
	float wait_pos[2];						//锚地位置，在中控决定调度船舶入港后同时决定ship_state，wait_pos，berth_idx，到正靠泊状态切换时自动计算船舶当前位置、角度。注意：此值为原图中的大值
	double angle;							//移动方向
	int berth_idx;							//分配的泊位，0为21号泊位，1为22号泊位，2为4号泊位右，3为4号泊位左，4为23泊位
	float ship_total_storage;				//总容量，吨位级别（level 0.7为5千吨，0.9为2万吨，1.1为3.5万吨，1.3为5万吨，1.5为6.5万吨）
	float ship_current_storage;			//当前容量
	int ship_index;							//货小类（0表示未定义）
	int ship_type;							//货大类（0表示未定义）
	float ship_coords[8];					//船舶坐标：x,y,z,state当前状态,percent存储量百分比，focus获得焦点（未到港时x,y都为0），level吨位级别，type货物种类
	long double ship_dist;					//船舶与泊位的距离，如果距离开始变大，则视为船舶开始离开泊位，就设定为靠泊状态
};

struct Pos_Berth
{
	float x_4_1;		//4号左泊位靠泊x坐标，对应门机11#，12#，berth_idx = 3
	float x_4_2;		//4号右泊位靠泊x坐标，对应门机13#，14#，berth_idx = 2
	float x_21;			//21号泊位靠泊x坐标，对应门机1#，2#，berth_idx = 0
	float x_22;			//22号泊位靠泊x坐标，对应门机3#，4#，berth_idx = 1
	float x_23;			//23号泊位靠泊x坐标，对应门机SL4，berth_idx = 4
};

//泊位
class Berth
{
public:
	Berth();
	void reset(SimuCore& core, bool rand_init);		//重置
	void ship_random_initiator(SimuCore& core);	//船舶随机初始化
	int berth_finished;								//卸完结束泊位序号，-1为不结束，0到3为泊位的A皮带，4到7为泊位B皮带
	ShipLoader loader;								//装船机属性结构体，不得将权限设置为private，否则在构造函数会引发访问冲突
	void drawUnloader(Camera& camera, Shader& unloaderShader, Shader& bucketShader);
	void drawLoader(Camera& camera, Shader& loaderShader);
	void drawShip(Camera& camera, Shader& shipShader);
	void initGuiStyle();							//样式初始化
	int ship_dispatch();							//船舶调度控制台，返回需要流程停止的泊位序号
	void unloader_dispatch();					//卸船机调度台
	void updateBuckets(float simurate);			//更新抓斗状态
	int updateShips(float simurate);			//更新船只状态，返回1为装船装满结束
	void add_type(std::string str_name, int type_type);				//添加货物新类
	bool set_unloading_ship(std::vector<std::string>& equipments);		//卸船机卸船准备检查，返回false为开始失败
	bool set_loading_ship(int load_type, int load_index);				//装船机装船准备检查，返回false为装船启动失败
	void run_unloader_unloaded(std::vector<std::string>& equipments);	//空载运行
	void run_unloader_loaded(std::vector<std::string>& equipments);	//负载运行
	void unloader_shutDown(std::vector<std::string>& equipments);		//流程取消
	int get_up_type();							//获取通用泊位货物大类
	int get_up_index();							//获取通用泊位货物小类
	float get_up_amount();						//获取通用泊位作业量
	int get_down_type();						//获取4泊位货物小类
	int get_down_index();						//获取4泊位货物小类
	float get_down_amount();					//获取4泊位作业量
	void set_focus(std::vector<std::string>& equipments);
	void lose_focus();

private:
	ImGuiStyle* style;								//ImGui样式存储
	std::string info_ships_name[34];				//船舶名称
	float info_ships_storage[34];					//船舶载货量
	bool info_ships_occupied[34];					//船舶对应序号是否被注册，注册过的船只不能重复注册
	bool berth_occupied[5];							//泊位占用状态
	bool unload_mode_gen;							//通用泊位卸船模式，false为FIFO先进先出，true为LIFO后进先出
	Type_ship type_unit;							//货物单元，包含货物种类序号及其名称
	std::vector<Type_ship> names;					//货物种类编号与名称存储容器
	ShipUnloader unloader;							//卸船机属性结构体
	std::vector<ShipUnloader> unloaders;			//卸船机状态存储容器
	float unloader_coords[40];						//卸船机坐标：x,y,z,state设备状态,focus获得焦点
	Pos_bucket bucket_up;							//1-4#门机料斗数据(int)
	Pos_bucket bucket_down;						//11-14#门机料斗数据(int)
	float bucket_coords[40];						//抓斗坐标：x,y,z,ver料斗颜色标志0黑色1白色,focus获得焦点0失去焦点1
	unsigned int unloaderVBO, unloaderVAO;			//VAO & VBO
	unsigned int bucketVBO, bucketVAO;				//VAO & VBO
	float loader_coords[5];							//装船机坐标：x,y,z,state,focus
	unsigned int loaderVBO, loaderVAO;				//VAO & VBO
	Ship ship;										//船舶属性结构体
	std::vector<Ship> ships;							//船舶容器
	Pos_Berth berth_pos;							//泊位x坐标存储容器(float)
	unsigned int shipVBO, shipVAO;					//VAO & VBO
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	int get_ship_type(float total_storage);		//判定船舶吨位：0为5千吨，1为2万吨，2为3.5万吨，3为5万吨，4为6.5万吨
	float get_ship_level(float total_storage);		//判定船舶吨位：0.7为5千吨，0.9为2万吨，1.1为3.5万吨，1.3为5万吨，1.5为6.5万吨
	bool type_check(int type, int index);			//检查是否存在相应货物类的定义，返回true表示存在定义
	void bucket_unload(float simurate, ShipUnloader& unloader);	//抓斗下行过程中减少对应门机泊位船舶的当前载货量
	void pre_button();
	void post_button();

};
