#pragma once
#include <fstream>
#include <glm/glm.hpp>
#include <imgui.h>
#include "Environment.h"
#include "Flow.h"

//中控
struct ConsoleFile
{
	bool random_initiating;			//随机初始化选择位
	int compitence;					//权限：0游客，1卸船机驾驶员，2船舶调度员，3火车调度员，4火车船舶总调度员，5流程管理员，6仿真人员，7堆场管理员，8筒仓管理员，9变电所管理员，10超级管理员，11气象员，12上帝
};

//仿真核文件
struct CoreFile
{
	double time;					//程序总运行时间（无论是否暂停）
	double simuTime;				//仿真运行时间
	bool simu_pause;				//程序状态，true为暂停，false为运行
	float run_rate;					//程序运行速度
	bool stepping;					//步进状态
	bool reset_zero;				//重置——零状态
	bool reset_rand;				//重置——随机，若两个随机都被激活，优先进行随机重置
	int runtime_minutes;			//仿真运行时间-分
	int runtime_hours;				//仿真运行时间-时
	double simu_deltaTime;
	bool freshRequire;				//刷新时刻标志位，true表示需要刷新
	double freshGapTime;			//刷新间隔时间，刷新间隔时间是固定的，不会受仿真速率影响，而仿真时间会受仿真速率影响
	int freshGapSwitch;				//刷新间隔时间选择，初始值为50ms（20帧），1表示20ms50FPS，2表示50ms20FPS，3表示100ms10FPS，4表示200ms5FPS，5表示500ms2FPS，6表示1000ms1FPS
	double curGapTime;				//当前累计刷新时间
	double simu_lastFrame;			//上一刷新时间记录
	double terminate_time;			//程序终结时间
	double terminate_countdown;	//程序终结倒计时
	int runtime_seconds;			//仿真运行时间-秒
	int runtime_days;				//仿真运行时间-日
};

//能源类存储
struct EnergyFile
{
	int trip;										//跳闸的变压器序号，-1表示没跳闸
	float total_energy;								//总能耗
	float cur_pow[8];								//8台主变的当前能耗
	float cur_energy[8];							//8台主变的当前总能耗
	int state[153];									//开关柜状态
	float time[153];								//开关柜运行总时长
	float pow_rate[153];							//开关柜运行总时长
};

//环境类存储
struct EnvironmentFile
{
	int weather;							//天气，0为晴天，1为阴天，2为下雨，3为大雨，4为大雾霾，5为大风，6为下雪，7为沙尘暴，8雷暴
	float lightPos[3];						//光源位置，注意读取转换为vec3
	float deltaTime;
	float lastFrame;
	float circle_radius;						//模拟太阳的移动半径
	bool skybox;							//天空盒开关
	bool background;						//背景开关
	int texture_act;						//激活的材质
	int time_mode;							//时间映射模式：0为没有映射，1为使用仿真时间映射，2为本地指定映射
	int minute_local;						//本地分钟
	int hour_local;							//本地小时
	bool light;								//光照开关
	bool spotlight;							//聚光模式
};

//物流网存储
struct FragmentFile
{
	int froms_code;						//起源设备名，对于连接卸船机的皮带，其源节点有4个（固定，不论是否有输出）
	int to_code;						//终止设备序号
	float total_length;					//皮带在当前流程起源到终止设备的总长度，在当前片段创建时计算
	float begin;							//开头位置，在整条流程结尾时减少整个片段的煤量，开头永久向前推进
	float end;							//末尾位置，在整条流程为未脱钩（即当前源头位置不中断供给）情况下末尾位置固定于源头位置重合
	bool disconnected;					//脱钩标志，刚开启（状态false）一个煤炭片段为源头从无输出切换为有输出的瞬间；脱钩瞬间（状态改为true）为源头从有输出切换为无输出的瞬间
	float amount;						//载货量，在每个时间片都计算增加量或减少量：当当前片段的begin与本流程末尾位置重合，根据皮带带速减少本段量；当当前片段end位置与开头位置重合，以由上级决定的输出效率增加量（可以添加随机数扰动）
	int cargo_type;						//货物大类，0表示未定义
	int cargo_index;					//货物小类，0表示未定义
};

struct WebFile
{
	float convAmount[51];				//皮带当前载货量
	float convPos[74];					//节点中的皮带累计长度
	bool wheelMode[74];					//本节点（若是斗轮机）的运行模式，0为堆料，1为取料
	bool out[74];						//本节点（若是流起源节点）的输出状态，0为没有在输出，1为有在输出
	float out_flux[74];					//本节点（若是流起源节点或取料模式斗轮机）的输出流量
	int out_type[74];					//本节点（若是流起源节点或取料模式斗轮机）的输出货物大类
	int out_index[74];					//本节点（若是流起源节点或取料模式斗轮机）的输出货物小类
};

//流程类存储
struct FlowFile
{
	bool window_confirm;					//流程负载启动确认窗口
	bool windowGroundChoose;				//地坑装车种类选择窗口
	bool groundSelected;					//地坑装车已选定
	int chooseType;							//地坑选定大类
	int chooseIndex;						//地坑选定小类
	int onfocus;								//当前焦点流程
	bool emergency_stop;					//流程急停窗口
	int flow_state[113];
	unsigned int flow_time[113];
	bool scene_ready[113];
};

//皮带类存储
struct ConvFile
{
	int conv_state[51];
	float conv_cur_storage[51];
	int flow_name[51];
	int flow_idx[51];
	int conv_type[51];
	int conv_index[51];
	float conv_coords[459];					//皮带坐标VBO，数据x1,y1,x2,y2,z,r,g,b,focus
};

//斗轮机存储
struct WheelFile
{
	int wheel_state[8];
	bool wheel_mode[8];
	float wheel_coords[56];					//斗轮机坐标：x,y,z,r,g,b,focus
};

//泊位存储
struct BerthFile
{
	int loader_state;						//装船机状态
	int unloader_state[8];
	float crab_bucket_hor[8];
	float crab_bucket_ver[8];
	bool crab_bucket_switch[8];
	bool crab_bucket_loaded[8];
	int berth_finished;						//卸完结束泊位序号，-1为不结束，0到3为泊位的A皮带，4到7为泊位B皮带
	bool webUnloaders[4];					//卸船机是否可用（针对物流网）
	bool info_ships_occupied[34];			//船舶对应序号是否被注册，注册过的船只不能重复注册
	bool berth_occupied[5];					//泊位占用状态
	bool unload_mode_gen;					//通用泊位卸船模式，false为FIFO先进先出，true为LIFO后进先出
	float unloader_coords[40];				//卸船机坐标：x,y,z,state设备状态,focus获得焦点
	float bucket_coords[40];				//抓斗坐标：x,y,z,ver料斗颜色标志0黑色1白色,focus获得焦点0失去焦点1
	float loader_coords[5];					//装船机坐标：x,y,z,state,focus
};

struct ShipFile
{
	char ship_name[50];
	int ship_state;
	float wait_pos[2];
	double angle;
	int berth_idx;
	float ship_total_storage;
	float ship_current_storage;
	int ship_index;
	int ship_type;
	float ship_coords[8];
	long double ship_dist;
};

//装车楼存储
struct TrainFile
{
	int building_state[2];					//装车楼运行状态，-1灰色系统离线（停机状态），0红色未上电或故障，1绿色运行
	int building_mode[2];					//装车模式切换，0先进先出FIFO，1后进先出LIFO，2左（16和18道），3右（15和17道）
	float building_coords_A[6];				//装车楼A坐标：x,y,z,state,focus,mode（取值0-3）
	float building_coords_B[6];				//装车楼B坐标：x,y,z,state,focus,mode（取值0-3）
};

//堆场存储
struct YardFile
{
	int yard_choosed;						//选中的堆场
	int child_choosed;						//选中的子堆场
	int type_choosed;						//选中子堆场的货物大类
	int index_choosed;						//选中子堆场的货物小类
	float amount_choosed;					//选中子堆场的货物存量
	bool data_send;							//数据是否发送
	bool yard_choosing;						//是否开启堆场选择窗口
	int flow_yard_choosed_1;				//由流程选中的堆场1号
	int flow_yard_choosed_2;				//由流程选中的堆场2号
	int flow_num_choose;					//由流程选中的堆场的数量
	int num_children[10];
	bool yard_focus[10];
};

//筒仓存储
struct SiloFile
{
	bool data_send;							//数据是否发送
	bool select_window;						//是否显示筒仓选择界面
	float amount_apply;					//申请堆放量
	int selected_index;						//选中的筒仓
	bool silo_occupied[14];
	float storage_targets;					//目标存量
	float storage_process;					//当前存量变化
	int straight_state;						//直出状态，0为不直出，1为直出大混，2为直出化工煤
	float straight_flux;					//直出流量
	unsigned int out_total_0;				//总出大混
	unsigned int out_total_1;				//总出化工煤
	float cur_storage[14];
	bool silo_type[14];
	float silo_flux[14];
	bool silo_focus[14];
	bool silo_state_up[14];
	bool silo_state_down[14];
	float silo_coords[8][14];
};

//存档结构体
struct Save
{
	ConsoleFile consoleFile;
	CoreFile coreFile;
	EnergyFile energyFile;
	EnvironmentFile environmentFile;
	int sizeofFragments;
	std::vector<FragmentFile> fragmentFiles;
	WebFile webFile;
	FlowFile flowFile;
	ConvFile convFile;
	int sizeofAct_flows;
	std::vector<FlowState> act_flows;
	WheelFile wheelFile;
	BerthFile berthFile;
	int sizeofShips;
	std::vector<ShipFile> ships;
	TrainFile trainFile;
	int sizeofTrains;
	std::vector<TrainAttrib> trains;
	YardFile yardFile;
	std::vector<Yard_child> yards;
	SiloFile siloFile;
};

class Record
{
public:
	Record();
	void initGuiStyle();
	void save_level(int mark, Message& message, bool& random_initiating, int& compitence, SimuCore& core, Energy& energy, Environment& environment, Web& web, Flow& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo);		//储存存档
	void load_level(int mark, Message& message, bool& random_initiating, int& compitence, SimuCore& core, Energy& energy, Environment& environment, Web& web, Flow& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo);		//存档读取
	int showGui();			//显示界面，若返回0表示无事发生；返回正数表示存档的栏位，返回负数表示读档的栏位
	//bool new_saved;			//在当前仿真循环已经存储过一次的情况下，记录为true，表示再次存档会覆盖原来储存的一个存档；记录为false，表示当前仿真循环还未存过档，会记录新的存档

private:
	Save save_file;
	FragmentFile fragmentFile;
	FlowState flowState;
	ShipFile shipFile;
	TrainAttrib train;
	One_wagon wagon;
	Yard_child child;
	ImGuiStyle* style;
	void pre_button();
	void post_button();

};
