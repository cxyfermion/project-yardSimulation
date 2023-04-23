#include "Flow.h"

#define FLUX 0.22f * 3.15f * 0.8f		//流程通用流量
#define RECLAIM_SAFE_START 10.0f		//取料流程启动时堆场最低当前容量
#define RECORD_FLOW "FlowShutdownRecord.txt"

Flow::Flow()
{
	this->window_confirm = false;
	this->onfocus = -1;
	this->emergency_stop = false;
	this->windowGroundChoose = false;
	this->groundSelected = false;
	this->chooseType = 1;
	this->chooseIndex = 1;
	//初始化种类数据
	std::string info_type[3];
	info_type[0] = u8"碎石,水渣,工业盐,纸浆,黄砂,铜矿,石油焦,焦煤,化工煤,混煤,优混,无烟煤,沥青";
	info_type[1] = u8"大混,蒙混,大同,大精,陕煤,俄罗斯煤,俄罗斯无烟,兴煤";
	info_type[2] = u8"新中港,神华精,伊泰,天马,特底灰,神混,神优,优一,神华,富动";
	int last_pos = 0;
	int index_child = 1;
	for (int i = 0; i < 3; i++)
	{
		int last_idx = (int)(info_type[i].rfind(',', info_type[i].size()));
		last_pos = 0;
		index_child = 1;
		this->type_unit.type_type = i + 1;
		while (last_pos != last_idx + 1)
		{
			this->type_unit.type_name = info_type[i].substr(last_pos, info_type[i].find(',', last_pos) - last_pos);
			last_pos = (int)(info_type[i].find(',', last_pos)) + 1;
			this->type_unit.type_index = index_child;
			index_child++;
			this->names.push_back(this->type_unit);
		}
		this->type_unit.type_name = info_type[i].substr(last_pos, info_type[i].find(',', last_pos) - last_pos);
		this->type_unit.type_index = index_child;
		this->names.push_back(this->type_unit);
	}
	std::string info_flow[113];
	//21号泊位流程，上4号泊位堆场
	info_flow[0] = "2101,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC4B,BC5,BC8,SR6";
	info_flow[1] = "2102,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC4B,BC9,SR7";
	info_flow[2] = "2103,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC10,SR8";
	info_flow[3] = "2104,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC6A,BC11,SR9";
	info_flow[4] = "2105,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC4A,BC5,BC8,SR6";
	info_flow[5] = "2106,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC4A,BC9,SR7";
	info_flow[6] = "2107,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC10,SR8";
	info_flow[7] = "2108,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC6B,BC11,SR9";
	info_flow[8] = "2111,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC4B,BC5,BC8,SR6";
	info_flow[9] = "2112,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC4B,BC9,SR7";
	info_flow[10] = "2113,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC10,SR8";
	info_flow[11] = "2114,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC6A,BC11,SR9";
	info_flow[12] = "2115,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC4A,BC5,BC8,SR6";
	info_flow[13] = "2116,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC4A,BC9,SR7";
	info_flow[14] = "2117,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC10,SR8";
	info_flow[15] = "2118,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC6B,BC11,SR9";
	info_flow[16] = "2131,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC4B,BC5,BC8,SR6";
	info_flow[17] = "2132,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC4B,BC9,SR7";
	info_flow[18] = "2133,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC10,SR8";
	info_flow[19] = "2134,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC6A,BC11,SR9";
	info_flow[20] = "2135,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC4A,BC5,BC8,SR6";
	info_flow[21] = "2136,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC4A,BC9,SR7";
	info_flow[22] = "2137,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC10,SR8";
	info_flow[23] = "2138,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC6B,BC11,SR9";
	info_flow[24] = "2141,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC4B,BC5,BC8,SR6";
	info_flow[25] = "2142,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC4B,BC9,SR7";
	info_flow[26] = "2143,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC10,SR8";
	info_flow[27] = "2144,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC6A,BC11,SR9";
	info_flow[28] = "2145,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC4A,BC5,BC8,SR6";
	info_flow[29] = "2146,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC4A,BC9,SR7";
	info_flow[30] = "2147,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC10,SR8";
	info_flow[31] = "2148,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC6B,BC11,SR9";
	info_flow[32] = "2151,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC6A,BC7,BC12,SR10";
	info_flow[33] = "2152,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC6B,BC7,BC12,SR10";
	info_flow[34] = "2153,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC6A,BC7,BC12,SR10";
	info_flow[35] = "2154,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC6B,BC7,BC12,SR10";
	info_flow[36] = "2155,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC6A,BC7,BC12,SR10";
	info_flow[37] = "2156,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC6B,BC7,BC12,SR10";
	info_flow[38] = "2157,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC6A,BC7,BC12,SR10";
	info_flow[39] = "2158,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC6B,BC7,BC12,SR10";
	info_flow[40] = "2161,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC26,BC6A,BC7,BC12,SR10";
	info_flow[41] = "2162,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC26,BC6B,BC7,BC12,SR10";
	info_flow[42] = "2163,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC26,BC6A,BC7,BC12,SR10";
	info_flow[43] = "2164,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC26,BC6B,BC7,BC12,SR10";
	info_flow[44] = "2165,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC26,BC6A,BC7,BC12,SR10";
	info_flow[45] = "2166,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC26,BC6B,BC7,BC12,SR10";
	info_flow[46] = "2167,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC26,BC6A,BC7,BC12,SR10";
	info_flow[47] = "2168,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC26,BC6B,BC7,BC12,SR10";
	info_flow[48] = "2171,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC26,BC6A,BC11,SR9";
	info_flow[49] = "2172,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC26,BC6B,BC11,SR9";
	info_flow[50] = "2173,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC26,BC6A,BC11,SR9";
	info_flow[51] = "2174,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC26,BC6B,BC11,SR9";
	info_flow[52] = "2175,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC26,BC6A,BC11,SR9";
	info_flow[53] = "2176,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC26,BC6B,BC11,SR9";
	info_flow[54] = "2177,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC26,BC6A,BC11,SR9";
	info_flow[55] = "2178,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC26,BC6B,BC11,SR9";
	//上三角场
	info_flow[56] = "2181,BC20A,BC21A,BC22A,BC23A,BC30A,BC31,SR11";
	info_flow[57] = "2182,BC20B,BC21B,BC22B,BC23B,BC30A,BC31,SR11";
	info_flow[58] = "2183,BC20A,BC21A,BC22A,BC23B,BC30B,BC36,S12";
	info_flow[59] = "2184,BC20B,BC21B,BC22B,BC23B,BC30B,BC36,S12";
	//西门二场
	info_flow[60] = "2179,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,XC1,XC2,SR12";
	info_flow[61] = "2180,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,XC1,XC2,SR12";
	//装船
	info_flow[62] = "2191,BC20B,BC35,SL4";
	//4号泊位流程，上堆场
	info_flow[63] = "401,BC1A,BC2A,BC3A,BC4B,BC5,BC8,SR6";
	info_flow[64] = "402,BC1A,BC2A,BC3A,BC4B,BC9,SR7";
	info_flow[65] = "403,BC1A,BC2A,BC3A,BC10,SR8";
	info_flow[66] = "404,BC1A,BC2A,BC3A,BC6A,BC11,SR9";
	info_flow[67] = "405,BC1A,BC2A,BC3A,BC4A,BC5,BC8,SR6";
	info_flow[68] = "406,BC1A,BC2B,BC3B,BC4A,BC9,SR7";
	info_flow[69] = "407,BC1A,BC2B,BC3B,BC10,SR8";
	info_flow[70] = "408,BC1A,BC2B,BC3B,BC6B,BC11,SR9";
	info_flow[71] = "421,BC1B,BC2B,BC3B,BC4B,BC5,BC8,SR6";
	info_flow[72] = "422,BC1B,BC2B,BC3B,BC4B,BC9,SR7";
	info_flow[73] = "423,BC1B,BC2B,BC3B,BC10,SR8";
	info_flow[74] = "424,BC1B,BC2B,BC3B,BC6A,BC11,SR9";
	info_flow[75] = "425,BC1B,BC2B,BC3B,BC4A,BC5,BC8,SR6";
	info_flow[76] = "426,BC1B,BC2B,BC3B,BC4A,BC9,SR7";
	info_flow[77] = "427,BC1B,BC2B,BC3B,BC10,SR8";
	info_flow[78] = "428,BC1B,BC2B,BC3B,BC6B,BC11,SR9";
	info_flow[79] = "431,BC19,BC2A,BC3A,BC4B,BC5,BC8,SR6";
	info_flow[80] = "432,BC19,BC2A,BC3A,BC4B,BC9,SR7";
	info_flow[81] = "433,BC19,BC2A,BC3A,BC10,SR8";
	info_flow[82] = "434,BC19,BC2A,BC3A,BC6A,BC11,SR9";
	info_flow[83] = "435,BC19,BC2B,BC3B,BC4A,BC5,BC8,SR6";
	info_flow[84] = "436,BC19,BC2B,BC3B,BC4A,BC9,SR7";
	info_flow[85] = "437,BC19,BC2B,BC3B,BC10,SR8";
	info_flow[86] = "438,BC19,BC2B,BC3B,BC6B,BC11,SR9";
	info_flow[87] = "441,BC1A,BC2A,BC3A,BC6A,BC7,BC12,SR10";
	info_flow[88] = "442,BC1A,BC2B,BC3B,BC6B,BC7,BC12,SR10";
	info_flow[89] = "443,BC1B,BC2A,BC3A,BC6A,BC7,BC12,SR10";
	info_flow[90] = "444,BC1B,BC2B,BC3B,BC6B,BC7,BC12,SR10";
	info_flow[91] = "445,BC19,BC2A,BC3A,BC6A,BC7,BC12,SR10";
	info_flow[92] = "446,BC19,BC2B,BC3B,BC6B,BC7,BC12,SR10";
	//装火车
	info_flow[93] = "451,SR6,BC8,BC13,BC14,BC15,building_B";
	info_flow[94] = "452,SR7,BC9,BC13,BC14,BC15,building_B";
	info_flow[95] = "453,SR8,BC10,BC13,BC14,BC15,building_B";
	info_flow[96] = "461,SR11,BC31,BC32,BC33,building_A";
	info_flow[97] = "462,BC34,BC33,building_A";
	//至炼化
	info_flow[98] = "471,SR6,BC8,BC17,BC18B,炼化";
	info_flow[99] = "472,SR7,BC9,BC17,BC18B,炼化";
	info_flow[100] = "473,SR8,BC10,BC17,BC18B,炼化";
	info_flow[101] = "474,SR9,BC11,BC18B,炼化";
	info_flow[102] = "475,SR6,BC8,BC17,BC18B,ZW11,ZW12,silo";
	info_flow[103] = "476,SR7,BC9,BC17,BC18B,ZW11,ZW12,silo";
	info_flow[104] = "477,SR8,BC10,BC17,BC18B,ZW11,ZW12,silo";
	info_flow[105] = "478,SR9,BC11,BC18B,ZW11,ZW12,silo";
	info_flow[106] = "481,SR10,BC12,BC18A,ZW11,ZW12,silo";
	info_flow[107] = "482,SR10,BC12,BC18B,ZW11,ZW12,silo";
	info_flow[108] = "483,SR10,BC12,BC18B,炼化";
	info_flow[109] = "485,SR6,BC8,BC17,BC18A,ZW11,ZW12,silo";
	info_flow[110] = "486,SR7,BC9,BC17,BC18A,ZW11,ZW12,silo";
	info_flow[111] = "487,SR8,BC10,BC17,BC18A,ZW11,ZW12,silo";
	info_flow[112] = "488,SR9,BC11,BC18A,ZW11,ZW12,silo";
	//流程读取
	this->flow_attr.flow_state = 0;
	this->flow_attr.flow_time = 0;
	this->flow_attr.scene_ready = false;
	//临时流程代号存储
	std::string name;
	for (int i = 0; i < 113; i++)
	{
		//记录','最后一次出现位置
		int last_idx = (int)(info_flow[i].rfind(',', info_flow[i].size()));
		last_pos = 0;
		this->flow_attr.flow_name = info_flow[i].substr(0, info_flow[i].find(',', 0));
		last_pos = (int)(info_flow[i].find(',', 0)) + 1;
		this->flow_attr.equipments.clear();
		while (last_pos != last_idx + 1)
		{
			name = info_flow[i].substr(last_pos, info_flow[i].find(',', last_pos) - last_pos);
			last_pos = (int)(info_flow[i].find(',', last_pos)) + 1;
			this->flow_attr.equipments.push_back(name);
		}
		name = info_flow[i].substr(last_pos, info_flow[i].size() - last_pos);
		this->flow_attr.equipments.push_back(name);
		this->flow_attr.flow_idx = i;
		this->flows.push_back(this->flow_attr);
	}
}

void Flow::reset()
{
	for (std::vector<FlowAttrib>::iterator it = this->flows.begin(); it != this->flows.end(); it++)
	{
		it->flow_state = 0;
		it->flow_time = 0;
		it->scene_ready = 0;
	}
}

void Flow::initGuiStyle()
{
	this->style = &ImGui::GetStyle();
}

void Flow::show_ground()
{
	if (this->windowGroundChoose)
	{
		ImGui::Begin(u8"地坑或3泊位的货物种类选择");
		ImGui::Text(u8"选择货物大类：");
		ImGui::SameLine();
		ImGui::RadioButton(u8"大类散货", &this->chooseType, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"来源散货", &this->chooseType, 2);
		ImGui::SameLine();
		ImGui::RadioButton(u8"企业散货", &this->chooseType, 3);
		ImGui::Text(u8"选择货物小类：");
		int temp_1 = 1;
		for (std::vector<Type_flow>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
		{
			if (it->type_type == this->chooseType)
			{
				ImGui::SameLine();
				ImGui::RadioButton(it->type_name.c_str(), &this->chooseIndex, temp_1);
				temp_1++;
			}
		}
		this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
		this->style->Colors[ImGuiCol_Button] = ImColor(0, 250, 0, 255);
		if (ImGui::Button(u8"确定"))
		{
			this->windowGroundChoose = false;
			this->groundSelected = true;
		}
		this->style->Colors[ImGuiCol_Button] = ImColor(250, 0, 0, 255);
		ImGui::SameLine();
		if (ImGui::Button(u8"取消"))
		{
			this->windowGroundChoose = false;
			this->groundSelected = false;
		}
		this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
		this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
		ImGui::End();
	}
}

void Flow::add_type(Message& message, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Web& web)
{
	static int type_type = 1;			//大类
	static char str_name[50] = "";		//类名
	if (ImGui::CollapsingHeader(u8"新品种货物添加"))
	{
		ImGui::Indent();
		ImGui::Text(u8"输入新类名称：");
		ImGui::SameLine();
		ImGui::InputTextWithHint(" ", u8"在此输入新材料名称", str_name, IM_ARRAYSIZE(str_name));
		ImGui::Text(u8"选择新大类：");
		ImGui::SameLine();
		ImGui::RadioButton(u8"按类分", &type_type, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"按来源分", &type_type, 2);
		ImGui::SameLine();
		ImGui::RadioButton(u8"按企业分", &type_type, 3);
		this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
		this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
		if (ImGui::Button(u8"添加"))
		{
			//自身添加
			int lst_idx = 1;
			for (std::vector<Type_flow>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
			{
				if (it->type_type == type_type && it->type_index > lst_idx)
				{
					lst_idx = it->type_index;
				}
			}
			this->type_unit.type_index = ++lst_idx;
			this->type_unit.type_type = type_type;
			this->type_unit.type_name = str_name;
			this->names.push_back(this->type_unit);
			//其他添加
			conv.add_type(str_name, type_type);
			yard.add_type(str_name, type_type);
			berth.add_type(str_name, type_type);
			train.add_type(str_name, type_type);
			web.add_type(str_name, type_type);
			//std::cout << "新类添加成功！" << std::endl;
			message.push_message(u8"新类添加成功！");
		}
		this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
		this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
		ImGui::Unindent();
	}
}

void Flow::showGui(Message& message, Energy& energy, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web)
{
	if (ImGui::CollapsingHeader(u8"流程控制面板"))
	{
		ImGui::Indent();
		//流程控制按钮
		if (this->onfocus != -1)
		{
			bool temp = false;
			std::vector<FlowAttrib>::iterator it = this->flows.begin();
			while (it->flow_idx != this->onfocus)
			{
				it++;
			}
			ImGui::Text((u8"当前选中：" + it->flow_name + u8" 流程").c_str());
			ImGui::SameLine();
			ImGui::Text(u8"流程设备：");
			for (std::vector<std::string>::const_iterator it1 = it->equipments.begin(); it1 != it->equipments.end(); it1++)
			{
				ImGui::SameLine();
				if (*it1 == "炼化")
				{
					ImGui::Text(u8"炼化");
				}
				else
				{
					ImGui::Text(it1->c_str());
				}
			}
			//状态控件群
			temp = it->flow_state == 0 && conv.startCheck(it->equipments) && wheel.startCheck(it->equipments);
			this->setText(temp);
			if (ImGui::Button(u8"流程选择"))//仅state=0时有效
			{
				if (temp)
				{
					//按钮变为红色
					it->flow_state = 1;
					//对应开关柜变为冷备状态
					energy.coldStart(it->equipments);
					message.push_sound("res/audio/flow.ogg", false);
				}
				else
				{
					if (it->flow_state != 0)
					{
						//std::cout << "启动失败::流程已经运行" << std::endl;
						message.push_message(u8"启动失败::流程已经运行");
					}
					else
					{
						//std::cout << "启动失败::设备被占用" << std::endl;
						message.push_message(u8"启动失败::设备被占用");
					}
				}
			}
			ImGui::SameLine();
			this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
			if (ImGui::Button(u8"取消选择"))//永久有效
			{
				this->onfocus = -1;
				conv.lose_focus();
				wheel.lose_focus();
				berth.lose_focus();
				train.lose_focus();
				yard.lose_focus();
				silo.lose_focus();
				web.lose_focus();
			}
			ImGui::SameLine();
			temp = it->flow_state == 1 && it->scene_ready == false;
			this->setText(temp);
			if (ImGui::Button(u8"现场检查"))//仅state=1时有效
			{
				if (temp)
				{
					//皮带变蓝色
					it->scene_ready = conv.sceneCheck(it->equipments);
				}
				else
				{
					if (it->flow_state == 1)
					{
						//std::cout << "检查失败::流程已经运行或未激活流程" << std::endl;
						message.push_message(u8"检查失败::流程已经运行或未激活流程");
					}
					else
					{
						//std::cout << "检查失败::已经检查完毕" << std::endl;
						message.push_message(u8"检查失败::已经检查完毕");
					}
				}
			}
			ImGui::SameLine();
			temp = it->flow_state == 1 && it->scene_ready == true;
			this->setText(temp);
			if (ImGui::Button(u8"待机运行"))//仅state=1且现场就绪时有效
			{
				if (temp)
				{
					//皮带变浅绿色
					conv.run_unloaded(it->equipments);
					//门机变闲置状态
					berth.run_unloader_unloaded(message, it->equipments);
					//开关柜启动
					energy.hotStart(it->equipments);
					message.push_sound("res/audio/flow.ogg", false);
					it->flow_state = 2;
				}
				else
				{
					if (it->flow_state == 1)
					{
						//std::cout << "待机运行失败::流程已经运行或未激活流程" << std::endl;
						message.push_message(u8"待机运行失败::流程已经运行或未激活流程");
					}
					else
					{
						//std::cout << "待机运行失败::检查未就绪" << std::endl;
						message.push_message(u8"待机运行失败::检查未就绪");
					}
				}
			}
			ImGui::SameLine();
			temp = it->flow_state == 2;
			this->setText(temp);
			std::string name_inst = "";
			if (yard.data_send)
			{
				name_inst = u8"负载运行——流程开始";
				if (!(it->flow_idx < 102 || it->flow_idx == 108))
				{
					if (!silo.data_send)
					{
						name_inst = u8"负载运行——筒仓选择";
					}
				}
			}
			else
			{
				name_inst = u8"负载运行——堆场选择";
			}
			if (ImGui::Button(name_inst.c_str()))//仅在state=2时有效
			{
				if (temp)
				{
					if (it->flow_idx < 93 && it->flow_idx != 62)
					{
						//堆料运行（需要确认堆场）
						int type = 0;
						int index = 0;
						float amount = 0.0f;
						bool berth3 = (it->flow_idx > 78 && it->flow_idx < 87) || it->flow_idx == 91 || it->flow_idx == 92;
						if (it->flow_idx < 62)
						{
							//通用泊位
							type = berth.get_up_type();
							index = berth.get_up_index();
							amount = berth.get_up_amount();
						}
						else if ((it->flow_idx > 62 && it->flow_idx < 79) || (it->flow_idx > 86 && it->flow_idx < 91))
						{
							//4泊位
							type = berth.get_down_type();
							index = berth.get_down_index();
							amount = berth.get_down_amount();
						}
						else if (berth3)
						{
							//3泊位
							if (!this->groundSelected && !this->windowGroundChoose)
							{
								this->windowGroundChoose = true;
							}
							else if (this->groundSelected && !this->windowGroundChoose)
							{
								type = this->chooseType;
								index = this->chooseIndex;
								amount = 10000.0f;
							}
						}
						if (type <= 0 || index <= 0 || amount == 0.0f)
						{
							//数据获取失败
							if (it->flow_idx < 62)
							{
								//std::cout << "流程负载启动失败::通用泊位船舶未就绪" << std::endl;
								message.push_message(u8"流程负载启动失败::通用泊位船舶未就绪");
							}
							else if ((it->flow_idx > 62 && it->flow_idx < 79) || (it->flow_idx > 86 && it->flow_idx < 91))
							{
								//std::cout << "流程负载启动失败::4泊位船舶未就绪" << std::endl;
								message.push_message(u8"流程负载启动失败::4泊位船舶未就绪");
							}
						}
						else if (!conv.runCheck(type, index, it->equipments))
						{
							//std::cout << "流程负载启动失败::当前皮带中存在没有卸完且种类不匹配的货物" << std::endl;
							message.push_message(u8"流程负载启动失败::当前皮带中存在没有卸完且种类不匹配的货物");
						}
						else
						{
							//堆场设定
							if (!yard.data_send)
							{
								//未接收数据，开启选择窗口
								yard.yard_choosing = true;
							}
							else
							{
								//接收数据，启动流程
								if (yard.start_stack(message, it->equipments, yard.yard_choosed, yard.child_choosed, type, index, amount, FLUX))
								{
									//当前泊位无船不允许启动流程，视为不允许空载运行
									if (berth.set_unloading_ship(berth3, message, it->equipments) || berth3)
									{
										yard.flow_num_choose = 0;
										//斗轮机运行
										wheel.run(it->equipments, 0);
										//皮带变深绿色
										conv.run_loaded(stoi(it->flow_name), type, index, it->equipments);
										//泊位设备变绿色
										berth.run_unloader_loaded(it->equipments);
										//物流网启动
										bool temp[4] = { berth.webUnloaders[0],berth.webUnloaders[1],berth.webUnloaders[2],berth.webUnloaders[3] };
										web.begin_flow(it->equipments, type, index, FLUX, temp);
										it->flow_state = 3;
										message.push_sound("res/audio/flow.ogg", false);
									}
								}
								yard.send_reset();
							}
						}
					}
					else if ((it->flow_idx > 92 && it->flow_idx < 97))
					{
						//取料运行（需要确认堆场）
						int type = 0;
						int index = 0;
						float cur_amount = 0.0f;
						if (!yard.data_send)
						{
							yard.yard_choosing = true;
						}
						else
						{
							//接收选中堆场
							type = yard.type_choosed;
							index = yard.index_choosed;
							cur_amount = yard.amount_choosed;
							if (cur_amount < RECLAIM_SAFE_START)
							{
								//std::cout << "流程负载启动失败::堆场当前容量不足以启动流程" << std::endl;
								message.push_message(u8"流程负载启动失败::堆场当前容量不足以启动流程");
								yard.send_reset();
							}
							else if (!conv.runCheck(type, index, it->equipments))
							{
								//std::cout << "流程负载启动失败::当前皮带中存在没有卸完且种类不匹配的货物" << std::endl;
								message.push_message(u8"流程负载启动失败::当前皮带中存在没有卸完且种类不匹配的货物");
							}
							else
							{
								//取料流程启动
								if (yard.start_reclaim(message, it->equipments, yard.yard_choosed, yard.child_choosed, type, index, FLUX))
								{
									//装车楼启动
									if (train.run(it->equipments, type, index))
									{
										yard.flow_num_choose = 0;
										//斗轮机运行
										wheel.run(it->equipments, 1);
										//皮带变深绿色
										conv.run_loaded(stoi(it->flow_name), type, index, it->equipments);
										//物流网启动
										bool temp[4] = { 0,0,0,0 };
										web.begin_flow(it->equipments, type, index, FLUX, temp);
										it->flow_state = 3;
										message.push_sound("res/audio/flow.ogg", false);
									}
									else
									{
										it->flow_state = 2;
										//std::cout << "负载运行失败::没有可以载货的火车" << std::endl;
										message.push_message(u8"负载运行失败::没有可以载货的火车");
									}
								}
								yard.send_reset();
							}
						}
					}
					else if (it->flow_idx > 97)
					{
						//至炼化（需要选择堆场和筒仓（若不是直出））
						static int artifice_type = 0;
						static int artifice_index = 0;
						static float artifice_cur_amount = 0.0f;
						if (!yard.data_send)
						{
							yard.yard_choosing = true;
						}
						else
						{
							//接收选中堆场
							artifice_type = yard.type_choosed;
							artifice_index = yard.index_choosed;
							artifice_cur_amount = yard.amount_choosed;
							if (artifice_cur_amount < RECLAIM_SAFE_START)
							{
								//std::cout << "流程负载启动失败::堆场当前容量不足以启动流程" << std::endl;
								message.push_message(u8"流程负载启动失败::堆场当前容量不足以启动流程");
								yard.send_reset();
							}
							else if (!((artifice_type == 1 && artifice_index == 9) || (artifice_type == 2 && artifice_index == 1)))
							{
								//std::cout << "流程负载启动失败::非大混和化工煤不能输入炼化" << std::endl;
								message.push_message(u8"流程负载启动失败::非大混和化工煤不能输入炼化");
								yard.send_reset();
							}
							else if (!conv.runCheck(artifice_type, artifice_index, it->equipments))
							{
								//std::cout << "流程负载启动失败::当前皮带中存在没有卸完且种类不匹配的货物" << std::endl;
								message.push_message(u8"流程负载启动失败::当前皮带中存在没有卸完且种类不匹配的货物");
							}
							else
							{
								bool coal_type = false;
								if (artifice_type == 1 && artifice_index == 9)
								{
									coal_type = true;
								}
								else if (artifice_type == 2 && artifice_index == 1)
								{
									coal_type = false;
								}
								if (it->flow_idx < 102 || it->flow_idx == 108)
								{
									//直出
									silo.start_straight(FLUX, coal_type);
								}
								else
								{
									//选择筒仓序号
									if (!silo.data_send)
									{
										silo.select_window = true;
									}
									else
									{
										int silo_choosed = silo.selected_index;
										if (silo_choosed == 0)
										{
											//std::cout << "筒仓选择失败::未选中筒仓" << std::endl;
											message.push_message(u8"筒仓选择失败::未选中筒仓");
											silo.send_reset();
											yard.send_reset();
										}
										else
										{
											//启动堆场
											if (yard.start_reclaim(message, it->equipments, yard.yard_choosed, yard.child_choosed, artifice_type, artifice_index, FLUX))
											{
												//启动筒仓
												if (silo.start_stack(silo_choosed, coal_type, silo.amount_apply, FLUX))
												{
													yard.flow_num_choose = 0;
													//斗轮机运行
													wheel.run(it->equipments, 1);
													//皮带变深绿色
													conv.run_loaded(stoi(it->flow_name), artifice_type, artifice_index, it->equipments);
													//物流网启动
													bool temp[4] = { 0,0,0,0 };
													web.begin_flow(it->equipments, artifice_type, artifice_index, FLUX, temp);
													it->flow_state = 3;
													message.push_sound("res/audio/flow.ogg", false);
												}
											}
											silo.send_reset();
											yard.send_reset();
										}
									}
								}
							}
						}
					}
					else if (it->flow_idx == 62)
					{
						//装船
						int type = berth.get_up_type();
						int index = berth.get_up_index();
						float amount = berth.get_up_amount();
						if (type <= 0 || index <= 0 || amount == 0.0f)
						{
							//数据获取失败
							//std::cout << "流程负载启动失败::通用泊位船舶未就绪" << std::endl;
							message.push_message(u8"流程负载启动失败::通用泊位船舶未就绪");
						}
						else if (!conv.runCheck(type, index, it->equipments))
						{
							//std::cout << "流程负载启动失败::当前皮带中存在没有卸完且种类不匹配的货物" << std::endl;
							message.push_message(u8"流程负载启动失败::当前皮带中存在没有卸完且种类不匹配的货物");
						}
						else
						{
							if (berth.set_loading_ship(message, type, index))
							{
								//皮带变深绿色
								conv.run_loaded(stoi(it->flow_name), type, index, it->equipments);
								//泊位设备变绿色
								berth.run_unloader_loaded(it->equipments);
								//物流网启动
								bool temp[4] = { 0,0,0,0 };
								web.begin_flow(it->equipments, type, index, FLUX, temp);
								it->flow_state = 3;
								message.push_sound("res/audio/flow.ogg", false);
							}
						}
					}
					else if (it->flow_idx == 97)
					{
						//地坑=>火车
						if (!this->groundSelected && !this->windowGroundChoose)
						{
							this->windowGroundChoose = true;
						}
						else if (this->groundSelected && !this->windowGroundChoose)
						{
							//装车楼变绿色
							if (!train.run(it->equipments, chooseType, chooseIndex))
							{
								it->flow_state = 2;
								//std::cout << "负载运行失败::没有可以载货的火车" << std::endl;
								message.push_message(u8"负载运行失败::没有可以载货的火车");
							}
							else
							{
								//皮带变深绿色
								conv.run_loaded(stoi(it->flow_name), chooseType, chooseIndex, it->equipments);
								it->flow_state = 3;
								//物流网启动
								bool temp[4] = { 0,0,0,0 };
								web.begin_flow(it->equipments, chooseType, chooseIndex, FLUX, temp);
								message.push_sound("res/audio/flow.ogg", false);
							}
						}
					}
				}
				else
				{
					//std::cout << "负载运行失败::设备未就绪" << std::endl;
					message.push_message(u8"负载运行失败::设备未就绪");
				}
			}
			ImGui::SameLine();
			temp = it->flow_state >= 2;
			this->setText(temp);
			if (ImGui::Button(u8"流程停止"))//仅在state>=2时有效
			{
				if (temp)
				{
					this->windowGroundChoose = false;
					this->groundSelected = false;
					//皮带变蓝色
					//conv.shutDown(stoi(it->flow_name));
					//斗轮机变红色
					//wheel.shutDown(it->equipments);
					//泊位设备变红色
					berth.unloader_shutDown(it->equipments);
					//装车楼变红色
					train.shutDown(it->equipments);
					//堆场停止
					yard.yard_end(it->equipments);
					//筒仓停止
					silo.end_silo(it->equipments);
					//开关柜转热备
					energy.hotEnd(it->equipments);
					//物流网终止
					web.end_flow(it->equipments);
					//按钮变红色
					it->flow_state = 1;
					message.push_sound("res/audio/flow.ogg", false);
				}
				else
				{
					//std::cout << "停止失败::流程未进行" << std::endl;
					message.push_message(u8"停止失败::流程未进行");
				}
			}
			ImGui::SameLine();
			temp = it->flow_state == 1/* && conv.cancelCheck(it->equipments)*/ && web.cancelCheck(it->equipments);
			this->setText(temp);
			if (ImGui::Button(u8"流程取消"))//仅在state=1且对应皮带状态全部变成蓝色时有效
			{
				if (temp)
				{
					//皮带变红色
					conv.cancel(it->equipments);
					//开关柜变冷备
					energy.coldEnd(it->equipments);
					it->flow_state = 0;
					it->scene_ready = false;
				}
				else
				{
					//std::cout << "取消失败::流程进行中" << std::endl;
					message.push_message(u8"取消失败::流程进行中");
				}
			}
			ImGui::SameLine();
			temp = it->flow_state != 0;
			if (!temp)
			{
				this->style->Colors[ImGuiCol_Text] = ImColor(100, 100, 100, 255);
			}
			else
			{
				this->style->Colors[ImGuiCol_Text] = ImColor(255, 0, 0, 255);
			}
			if (ImGui::Button(u8"流程急停"))//仅在state!=0时有效
			{
				if (temp)
				{
					this->emergency_stop = !this->emergency_stop;
				}
				else
				{
					//std::cout << "急停失败::流程未激活" << std::endl;
					message.push_message(u8"急停失败::流程未激活");
				}
			}
			//急停窗口
			this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
			if (this->emergency_stop)
			{
				ImGui::Begin(u8"确认急停");
				ImGui::Text(u8"确认流程急停？");
				static char briefing[500] = "";
				ImGui::InputTextWithHint(" ", u8"在此输入急停原因", briefing, IM_ARRAYSIZE(briefing));
				std::string reason = briefing;
				this->style->Colors[ImGuiCol_Button] = ImColor(250, 0, 0, 255);
				this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
				if (ImGui::Button(u8"确认"))
				{
					this->windowGroundChoose = false;
					this->groundSelected = false;
					//皮带变红色
					conv.emergency_shutDown(stoi(it->flow_name), it->equipments);
					//斗轮机变红色
					wheel.shutDown(it->equipments);
					//泊位设备变红色
					berth.unloader_shutDown(it->equipments);
					//装车楼变红色
					train.shutDown(it->equipments);
					//堆场停止
					yard.yard_end(it->equipments);
					//筒仓停止
					silo.end_silo(it->equipments);
					//开关柜急停
					energy.emergencyEnd(it->equipments);
					//物流网急停
					web.emergencyShutDown(it->equipments);
					this->save_reason(reason);
					it->flow_state = 0;
					it->scene_ready = false;
					this->emergency_stop = false;
					message.push_sound("res/audio/emergency.ogg", false);
				}
				ImGui::SameLine(100.0f);
				this->style->Colors[ImGuiCol_Button] = ImColor(0, 250, 0, 255);
				if (ImGui::Button(u8"取消"))
				{
					this->emergency_stop = false;
				}
				ImGui::End();
				this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
			}
			this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
		}
		//21号泊位流程，上4号泊位堆场
		if (ImGui::CollapsingHeader(u8"21号泊位流程，上4号泊位堆场"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind1());
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					this->showButton(*it, conv, wheel, berth, train, yard, silo, web);
					if (j != 7)
					{
						ImGui::SameLine();
					}
					it++;
				}
			}
			ImGui::Unindent();
		}
		//上三角场
		if (ImGui::CollapsingHeader(u8"上三角场"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind2());
			for (int i = 0; i < 4; i++)
			{
				this->showButton(*it, conv, wheel, berth, train, yard, silo, web);
				if (i != 3)
				{
					ImGui::SameLine();
				}
				it++;
			}
			ImGui::Unindent();
		}
		//西门二场
		if (ImGui::CollapsingHeader(u8"西门二场"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind3());
			for (int i = 0; i < 2; i++)
			{
				this->showButton(*it, conv, wheel, berth, train, yard, silo, web);
				if (i == 0)
				{
					ImGui::SameLine();
				}
				it++;
			}
			ImGui::Unindent();
		}
		//装船
		if (ImGui::CollapsingHeader(u8"装船"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind4());
			this->showButton(*it, conv, wheel, berth, train, yard, silo, web);
			ImGui::Unindent();
		}
		//4号泊位流程，上堆场
		if (ImGui::CollapsingHeader(u8"4号泊位流程，上堆场"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind5());
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (i == 3 && j >= 6)
					{
						break;
					}
					this->showButton(*it, conv, wheel, berth, train, yard, silo, web);
					if (j != 7)
					{
						if (i == 3 && j == 5)
						{
							break;
						}
						ImGui::SameLine();
					}
					it++;
				}
			}
			ImGui::Unindent();
		}
		//装火车
		if (ImGui::CollapsingHeader(u8"装火车"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind6());
			for (int i = 0; i < 5; i++)
			{
				this->showButton(*it, conv, wheel, berth, train, yard, silo, web);
				if (i != 4)
				{
					ImGui::SameLine();
				}
				it++;
			}
			ImGui::Unindent();
		}
		//至炼化
		if (ImGui::CollapsingHeader(u8"至炼化"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind7());
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (i == 1 && j == 3)
					{
						continue;
					}
					this->showButton(*it, conv, wheel, berth, train, yard, silo, web);
					if (it->flow_name == "483")
					{
						ImGui::SameLine(338.5f);
					}
					else if (it->flow_name != "478" && it->flow_name != "488")
					{
						ImGui::SameLine();
					}
					it++;
				}
			}
			//堆场直出控制台
			this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
			bool temp = false;
			temp = silo.check_silo_out(0);
			this->setText(temp);
			if (ImGui::Button(u8"启动炼化筒仓出煤——大混"))
			{
				if (temp)
				{
					silo.start_silo_out(FLUX, 0);
				}
				else
				{
					//std::cout << "启动筒仓出煤失败::当前筒仓没有大混煤或已经有筒仓在出煤" << std::endl;
					message.push_message(u8"启动筒仓出煤失败::当前筒仓没有大混煤或已经有筒仓在出煤");
				}
			}
			ImGui::SameLine();
			temp = silo.check_silo_out(1);
			this->setText(temp);
			if (ImGui::Button(u8"启动炼化筒仓出煤——化工煤"))
			{
				if (temp)
				{
					silo.start_silo_out(FLUX, 1);
				}
				else
				{
					//std::cout << "启动筒仓出煤失败::当前筒仓没有化工煤或已经有筒仓在出煤" << std::endl;
					message.push_message(u8"启动筒仓出煤失败::当前筒仓没有化工煤或已经有筒仓在出煤");
				}
			}
			ImGui::SameLine();
			temp = silo.out_check();
			this->setText(temp);
			if (ImGui::Button(u8"终止炼化筒仓出煤"))
			{
				if (temp)
				{
					silo.silo_end_out();
				}
				else
				{
					//std::cout << "终止筒仓出煤失败::当前筒仓没有正在出煤" << std::endl;
					message.push_message(u8"终止筒仓出煤失败::当前筒仓没有正在出煤");
				}
			}
			this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
			this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
			ImGui::Unindent();
			ImGui::Separator();
		}
		static bool showReason = false;
		this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
		this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
		if (ImGui::Button(u8"切换历史急停原因显示"))
		{
			if (!showReason)
			{
				showReason = true;
				this->load_reason(message);
			}
			else
			{
				showReason = false;
			}
		}
		this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
		if (showReason)
		{
			for (std::vector<std::string>::const_iterator it1 = this->reasons.begin(); it1 != this->reasons.end(); it1++)
			{
				ImGui::Text(it1->c_str());
			}
		}
		ImGui::Text("  ");
		ImGui::Unindent();
	}
}

void Flow::weatherCheck(Message& message, Environment& environment, Energy& energy, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web)
{
	if (environment.weather == 3 || environment.weather == 4 || environment.weather == 5 || environment.weather == 8)
	{
		bool temp = false;
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state > 0)
			{
				temp = true;
				//皮带变红色
				conv.emergency_shutDown(stoi(it1->flow_name), it1->equipments);
				//斗轮机变红色
				wheel.shutDown(it1->equipments);
				//泊位设备变红色
				berth.unloader_shutDown(it1->equipments);
				//装车楼变红色
				train.shutDown(it1->equipments);
				//堆场停止
				yard.yard_end(it1->equipments);
				//筒仓停止
				silo.end_silo(it1->equipments);
				//开关柜急停
				energy.emergencyEnd(it1->equipments);
				//物流网急停
				web.emergencyShutDown(it1->equipments);
				it1->flow_state = 0;
				it1->scene_ready = false;
			}
		}
		if (temp)
		{
			message.push_sound("res/audio/emergency.ogg", false);
			message.push_message(u8"警告::极端天气导致全部流程急停");
		}
	}
}

void Flow::train_check(Message& message, Energy& energy, int end_train_1, int end_train_2, TrainLoader& train, Yard& yard, Web& web)
{
	if (end_train_1 == 1 || end_train_2 == 1)
	{
		//BC15停止
		for (std::vector<FlowAttrib>::iterator it = this->flows.begin(); it != this->flows.end(); it++)
		{
			if ((it->flow_idx > 92 && it->flow_idx < 96) && it->flow_state == 3)
			{
				//皮带变蓝色
				//conv.shutDown(stoi(it->flow_name));
				//斗轮机变红色
				//wheel.shutDown(it->equipments);
				//装车楼变红色
				train.shutDown(it->equipments);
				//堆场停止
				yard.yard_end(it->equipments);
				//转热备
				energy.hotEnd(it->equipments);
				//物流网终止
				web.end_flow(it->equipments);
				//按钮变红色
				it->flow_state = 1;
				message.push_sound("res/audio/flow.ogg", false);
			}
		}
	}
	else if (end_train_1 == 2 || end_train_2 == 2)
	{
		//BC33停止
		for (std::vector<FlowAttrib>::iterator it = this->flows.begin(); it != this->flows.end(); it++)
		{
			if ((it->flow_idx > 95 && it->flow_idx < 98) && it->flow_state == 3)
			{
				//皮带变蓝色
				//conv.shutDown(stoi(it->flow_name));
				//斗轮机变红色
				//wheel.shutDown(it->equipments);
				//装车楼变红色
				train.shutDown(it->equipments);
				//堆场停止
				yard.yard_end(it->equipments);
				//转热备
				energy.hotEnd(it->equipments);
				//物流网终止
				web.end_flow(it->equipments);
				//按钮变红色
				it->flow_state = 1;
				message.push_sound("res/audio/flow.ogg", false);
			}
		}
	}
}

void Flow::stop_yard_flow(Message& message, Energy& energy, std::string name_wheel, Berth& berth, TrainLoader& train, Silo& silo, Web& web)
{
	for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
	{
		if (it1->flow_state == 3)
		{
			for (std::vector<std::string>::iterator it2 = it1->equipments.begin(); it2 != it1->equipments.end(); it2++)
			{
				if (*it2 == name_wheel)
				{
					//终止流程
					//皮带变蓝色
					//conv.shutDown(stoi(it1->flow_name));
					//斗轮机变红色
					//wheel.shutDown(it1->equipments);
					//泊位设备变红色
					berth.unloader_shutDown(it1->equipments);
					//装车楼变红色
					train.shutDown(it1->equipments);
					//筒仓停止
					silo.end_silo(it1->equipments);
					//转热备
					energy.hotEnd(it1->equipments);
					//物流网终止
					web.end_flow(it1->equipments);
					it1->flow_state = 1;
					message.push_sound("res/audio/flow.ogg", false);
					break;
				}
			}
		}
	}
}

void Flow::stop_silo_flow(Message& message, Energy& energy, Yard& yard, Web& web)
{
	for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
	{
		if (it1->flow_state == 3)
		{
			std::vector<std::string>::reverse_iterator it2 = it1->equipments.rbegin();
			if (*it2 == "ZW12")
			{
				//皮带变蓝色
				//conv.shutDown(stoi(it1->flow_name));
				//斗轮机变红色
				//wheel.shutDown(it1->equipments);
				//堆场停止
				yard.yard_end(it1->equipments);
				//转热备
				energy.hotEnd(it1->equipments);
				//物流网终止
				web.end_flow(it1->equipments);
				it1->flow_state = 1;
				message.push_sound("res/audio/flow.ogg", false);
				break;
			}
		}
	}
}

void Flow::ship_leave(Energy& energy, int berth_idx, Yard& yard, Web& web)
{
	//对应泊位流程空载运行
	switch (berth_idx)
	{
	case 0:
		//21泊位
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end() && it1->flow_idx < 62; it1++)
		{
			if (it1->flow_state == 3)
			{
				it1->flow_state = 2;
				//皮带变蓝色
				//conv.shutDown(stoi(it1->flow_name));
				//斗轮机变红色
				//wheel.shutDown(it1->equipments);
				//堆场停止
				yard.yard_end(it1->equipments);
				//转热备
				energy.hotEnd(it1->equipments);
				//物流网终止
				web.end_flow(it1->equipments);
			}
		}
		break;
	case 1:
		//22泊位
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end() && it1->flow_idx < 62; it1++)
		{
			if (it1->flow_state == 3)
			{
				it1->flow_state = 2;
				//皮带变蓝色
				//conv.shutDown(stoi(it1->flow_name));
				//斗轮机变红色
				//wheel.shutDown(it1->equipments);
				//堆场停止
				yard.yard_end(it1->equipments);
				//转热备
				energy.hotEnd(it1->equipments);
				//物流网终止
				web.end_flow(it1->equipments);
			}
		}
		break;
	case 2:
		//4右泊位
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state == 3 && ((it1->flow_idx > 62 && it1->flow_idx < 79) || (it1->flow_idx > 86 && it1->flow_idx < 91)))
			{
				it1->flow_state = 2;
				//皮带变蓝色
				//conv.shutDown(stoi(it1->flow_name));
				//斗轮机变红色
				//wheel.shutDown(it1->equipments);
				//堆场停止
				yard.yard_end(it1->equipments);
				//转热备
				energy.hotEnd(it1->equipments);
				//物流网终止
				web.end_flow(it1->equipments);
			}
		}
		break;
	case 3:
		//4左泊位
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state == 3 && ((it1->flow_idx > 62 && it1->flow_idx < 79) || (it1->flow_idx > 86 && it1->flow_idx < 91)))
			{
				it1->flow_state = 2;
				//皮带变蓝色
				//conv.shutDown(stoi(it1->flow_name));
				//斗轮机变红色
				//wheel.shutDown(it1->equipments);
				//堆场停止
				yard.yard_end(it1->equipments);
				//转热备
				energy.hotEnd(it1->equipments);
				//物流网终止
				web.end_flow(it1->equipments);
			}
		}
		break;
	case 4:
		//23泊位
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state == 3 && it1->flow_idx == 62)
			{
				it1->flow_state = 2;
				//皮带变蓝色
				//conv.shutDown(stoi(it1->flow_name));
				//转热备
				energy.hotEnd(it1->equipments);
				//物流网终止
				web.end_flow(it1->equipments);
			}
		}
		break;
	default:
		break;
	}
}

void Flow::end_shiploading(Message& message, Energy& energy, Berth& berth, Web& web)
{
	for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
	{
		if (it1->flow_idx == 62)
		{
			it1->flow_state = 1;
			//皮带变浅绿色
			//conv.run_unloaded(it1->equipments);
			//泊位设备变红色
			berth.unloader_shutDown(it1->equipments);
			//转热备
			energy.hotEnd(it1->equipments);
			//物流网终止
			web.end_flow(it1->equipments);
			message.push_sound("res/audio/flow.ogg", false);
			break;
		}
	}
}

void Flow::end_shipunloading(Message& message, Energy& energy, int berth_finished, Conveyor& conv, SlewingWheel& wheel, Berth& berth, Yard& yard)
{
	if (berth_finished < 4)
	{
		//A皮带流程停机
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state == 3 && it1->equipments.begin()[4] == "A")
			{
				//皮带变浅绿色
				conv.run_unloaded(it1->equipments);
				//斗轮机变红色
				wheel.shutDown(it1->equipments);
				//堆场停止
				yard.yard_end(it1->equipments);
				//泊位变闲置
				berth.run_unloader_unloaded(message, it1->equipments);
				//转热备
				energy.hotEnd(it1->equipments);
				it1->flow_state = 2;
				message.push_sound("res/audio/flow.ogg", false);
				break;
			}
		}
	}
	else
	{
		//B皮带停机
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state == 3 && it1->equipments.begin()[4] == "B")
			{
				//皮带变浅绿色
				conv.run_unloaded(it1->equipments);
				//斗轮机变红色
				wheel.shutDown(it1->equipments);
				//堆场停止
				yard.yard_end(it1->equipments);
				//泊位变闲置
				berth.run_unloader_unloaded(message, it1->equipments);
				//转热备
				energy.hotEnd(it1->equipments);
				it1->flow_state = 2;
				message.push_sound("res/audio/flow.ogg", false);
				break;
			}
		}
	}
}

void Flow::trip_end(Message& message, bool all, std::vector<std::string> equipments, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web)
{
	message.push_sound("res/audio/emergency.ogg", false);
	if (all)
	{
		//大停电
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state >= 2)
			{
				//皮带变红色
				conv.emergency_shutDown(stoi(it1->flow_name), it1->equipments);
				//斗轮机变红色
				wheel.shutDown(it1->equipments);
				//泊位设备变红色
				berth.unloader_shutDown(it1->equipments);
				//装车楼变红色
				train.shutDown(it1->equipments);
				//堆场停止
				yard.yard_end(it1->equipments);
				//筒仓停止
				silo.end_silo(it1->equipments);
				//物流网急停
				web.emergencyShutDown(it1->equipments);
				it1->flow_state = 0;
				it1->scene_ready = false;
			}
		}
	}
	else
	{
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state >= 2)
			{
				for (std::vector<std::string>::iterator it2 = equipments.begin(); it2 != equipments.end(); it2++)
				{
					for (std::vector<std::string>::iterator it3 = it1->equipments.begin(); it3 != it1->equipments.end(); it3++)
					{
						if (*it2 == *it3)
						{
							//皮带变红色
							conv.emergency_shutDown(stoi(it1->flow_name), it1->equipments);
							//斗轮机变红色
							wheel.shutDown(it1->equipments);
							//泊位设备变红色
							berth.unloader_shutDown(it1->equipments);
							//装车楼变红色
							train.shutDown(it1->equipments);
							//堆场停止
							yard.yard_end(it1->equipments);
							//筒仓停止
							silo.end_silo(it1->equipments);
							//物流网急停
							web.emergencyShutDown(it1->equipments);
							it1->flow_state = 0;
							it1->scene_ready = false;
							break;
						}
					}
				}
			}
		}
	}
}

void Flow::end_web(Conveyor& conv, SlewingWheel& wheel, std::string finishEndName)
{
	for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
	{
		if (it1->flow_state == 1 && *it1->equipments.rbegin() == finishEndName)
		{
			conv.cancel_web(stoi(it1->flow_name));
			wheel.shutDown(it1->equipments);
			break;
		}
	}
}

void Flow::setText(bool temp)
{
	if (!temp)
	{
		this->style->Colors[ImGuiCol_Text] = ImColor(100, 100, 100, 255);
	}
	else
	{
		this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
	}
}

void Flow::showButton(FlowAttrib& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web)
{
	//流程状态，0白色未启用，1红色流程被选中等待现场就绪，2绿色正常运行
	this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
	switch (flow.flow_state)
	{
	case 0:
		this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
		break;
	case 1:
		this->style->Colors[ImGuiCol_Button] = ImColor(250, 0, 0, 255);
		break;
	case 2:
		this->style->Colors[ImGuiCol_Button] = ImColor(0, 250, 0, 255);
		break;
	case 3:
		this->style->Colors[ImGuiCol_Button] = ImColor(0, 200, 0, 255);
		break;
	default:
		this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
		break;
	}
	if (ImGui::Button((flow.flow_name + u8" 流程").c_str()))
	{
		this->pressButton(flow, conv, wheel, berth, train, yard, silo, web);
	}
	this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}

void Flow::pressButton(FlowAttrib& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo, Web& web)
{
	if (this->onfocus != flow.flow_idx)
	{
		this->onfocus = flow.flow_idx;
		conv.set_focus(flow.equipments);
		wheel.set_focus(flow.equipments);
		train.set_focus(flow.equipments);
		yard.set_focus(flow.equipments);
		silo.set_focus(flow.equipments);
		berth.set_focus(flow.equipments);
		web.set_focus(flow.equipments);
	}
	else
	{
		this->onfocus = -1;
		conv.lose_focus();
		wheel.lose_focus();
		train.lose_focus();
		yard.lose_focus();
		silo.lose_focus();
		berth.lose_focus();
		web.lose_focus();
	}
}

void Flow::save_reason(std::string reason)
{
	std::ofstream file(RECORD_FLOW, std::ios::app);
	file << reason << std::endl;
	file.close();
}

void Flow::load_reason(Message& message)
{
	this->reasons.clear();
	std::ifstream file;
	file.open(RECORD_FLOW, std::ios::in);
	if (!file.is_open())
	{
		message.push_message(u8"紧急停机原因文件打开失败");
		return;
	}
	std::string buf;
	while (file >> buf)
	{
		this->reasons.push_back(buf);
	}
	file.close();
}
