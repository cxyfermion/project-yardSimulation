#pragma once
#include <string>
#include <vector>
#include <imgui.h>

/*
卸船机->皮带->斗轮机
斗轮机->皮带->装车楼
斗轮机->皮带->筒仓（直出）
堆场和船舶在此系统中被省去

*/

struct Type_web
{
	int type_index;			//货物小类编号，具体到货物的名称，从1开始计数
	int type_type;			//货物大类编号，1为大类命名，2为来源命名，3为企业命名
	std::string type_name;	//堆货名称
};

//煤炭片段，创造时机为源头从无输出切换为有输出的瞬间
struct Fragment
{
	float begin;				//开头位置，在整条流程结尾时减少整个片段的煤量，开头永久向前推进
	float end;				//末尾位置，在整条流程为脱钩（即当前源头位置不中断供给）情况下末尾位置固定于源头位置重合
	bool disconnected;		//脱钩标志，刚开启（状态false）一个煤炭片段为源头从无输出切换为有输出的瞬间；脱钩瞬间（状态改为true）为源头从有输出切换为无输出的瞬间
	float amount;			//载货量，在每个时间片都计算增加量或减少量：当当前片段的begin与本流程末尾位置重合，根据皮带带速减少本段量；当当前片段end位置与开头位置重合，以由上级决定的输出效率增加量（可以添加随机数扰动）
	int cargo_type;			//货物大类，0表示未定义
	int cargo_index;		//货物小类，0表示未定义

};

//记录节点前面或后面连接信息的结构体
struct PrePos
{
	std::string other_name;				//节点的代码
	bool act;							//此节点是否与当前节点有物理连接
};

//物流网节点
struct Node
{
	std::string name;					//本节点名称
	std::vector<PrePos> pre_codes;		//本节点前面连接节点的代码
	std::vector<PrePos> pos_codes;		//本节点后面连接节点的代码
	int this_code;						//当前节点代码（从0开始）
	int type;							//节点种类，0表示船舶，1表示卸船机，2表示皮带，3表示斗轮机，4表示装船机，5表示堆场，6表示筒仓，7表示装车楼，8表示炼化直出，9表示来自3泊位的输入，10表示地坑
	int convLength;						//本节点（若是皮带）的长度
	float convSpeed;					//本节点（若是皮带）的带速
	bool wheelMode;						//本节点（若是斗轮机）的运行模式，0为堆料，1为取料

};

class Web
{
public:
	Web();
	void reset();
	void initGuiStyle();
	void update();
	void add_type(std::string str_name, int type_type);
	void web_dispatch(bool unreal);

private:
	ImGuiStyle* style;
	Type_web type_unit;
	std::vector<Type_web> names;
	Fragment fragment;						//煤炭片段
	std::vector<Fragment> fragments;		//煤炭片段群
	PrePos prepos;							//前后节点结构体
	Node node;								//节点
	std::vector<Node> nodes;				//物流网
	void pre_button(bool choosed);
	void post_button();

};
