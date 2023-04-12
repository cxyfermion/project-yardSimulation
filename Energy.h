#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <imgui.h>
#include "Message.h"

/*
注意：开关柜应当还包含类似PT避雷，备用柜，分变所自身，无功补偿柜，所用变，分段联络，进线，隔离，环网柜，出线柜等，在此系统中统统忽略
变电箱的开关柜连接设备前加上@前缀表示当前开关柜作用于配电网或其他，而不是码头设备群组

字符串u8编码格式下ascii码范围的字符值相同，但中文的u8字符和非u8字符值不同

*/

//开关柜，现在系统中的所有开关柜都是高压开关柜，制造商都是华通
struct switchGear
{
	int gear_idx;						//开关柜序号
	int pow;								//开关柜对应设备的额定运行功率
	int state;							//运行状态：0表示冷备状态，1表示热备状态，2表示运行状态
	float time;							//运行总时长
	float pow_rate;						//当前功率比例
	std::string gear_transformer;		//从属的变压器
	std::string gear_type;				//开关柜型号
	int transformation_ratio;			//变比：为正数表示正常变比，为0表示PT避雷，为负表示所用变KV
	std::string gear_provider;			//开关柜制造商
	std::string equipment;				//开关柜控制的设备，首字母为u8'@'表示该供电柜不服务于码头设备群组
};

//变压器，制造商都是三变科技
struct Transformer
{
	std::string transformer_name;			//变压器名称
	std::vector<switchGear> gears;			//当前变压器包含的所有开关柜
	int transformer_idx;					//变压器序号，从0开始，0为中心变
	std::string transformer_type;			//变压器型号
	int max_pow;							//最高功率容量（单位KVa）
	float cur_pow;							//当前功率值
	float cur_energy;						//变压器的当前总能耗
	std::string transformer_provider;		//变压器制造商
};

class Energy
{
public:
	Energy();
	void reset();															//重置
	void update(Message& message, float simurate, double deltaTime);		//更新状态
	void coldStart(std::vector<std::string>& equipments);					//从冷备状态变为热备状态
	void hotStart(std::vector<std::string>& equipments);					//从热备状态变为运行状态
	void hotEnd(std::vector<std::string>& equipments);						//从运行状态变为热备状态
	void coldEnd(std::vector<std::string>& equipments);						//从热备状态变为冷备状态
	void emergencyEnd(std::vector<std::string>& equipments);				//紧急停机
	std::vector<std::string> getEquipments();
	void initGuiStyle();														//样式初始化
	void transformerDispatch();												//变压器显示界面
	int trip;																//跳闸的变压器序号，-1表示没跳闸

private:
	float total_energy;							//总能耗
	ImGuiStyle* style;							//ImGui样式存储
	switchGear gear;							//开关柜结构体
	Transformer transformer;					//变压器结构体
	std::vector<Transformer> transformers;		//变压器容器
	void pre_button(bool choosed);				//按钮样式设置
	void post_button();

};
