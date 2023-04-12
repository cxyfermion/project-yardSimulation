#pragma once
#include <vector>
#include <imgui.h>

//煤炭片段，创造时机为源头从无输出切换为有输出的瞬间
struct Fragment
{
	float begin;		//开头位置，在整条流程结尾时减少整个片段的煤量，开头永久向前推进
	float end;		//末尾位置，在整条流程为脱钩（即当前源头位置不中断供给）情况下末尾位置固定于源头位置重合
	bool disconnected;		//脱钩标志，刚开启（状态false）一个煤炭片段为源头从无输出切换为有输出的瞬间；脱钩瞬间（状态改为true）为源头从有输出切换为无输出的瞬间
	float amount;			//载货量，在每个时间片都计算增加量或减少量：当当前片段的begin与本流程末尾位置重合，根据皮带带速减少本段量；当当前片段end位置与开头位置重合，以由上级决定的输出效率增加量（可以添加随机数扰动）
	int cargo_type;			//货物大类，0表示未定义
	int cargo_index;		//货物小类，0表示未定义

};

//物流网节点
struct Node
{
	int type;			//节点种类，0表示卸船机，1表示皮带，2表示斗轮机，3表示装船机，4表示堆场，5表示筒仓
	bool in_out;		//斗轮机运行模式，0表示堆料，1表示取料



};

class Web
{
public:
	Web();


private:


};
