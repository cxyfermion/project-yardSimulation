#include "Web.h"

Web::Web()
{
	//初始化名称容器
	std::string info_type[3];
	info_type[0] = u8"碎石,水渣,工业盐,纸浆,黄砂,铜矿,石油焦,焦煤,化工煤,混煤,优混,无烟煤,沥青";
	info_type[1] = u8"大混,蒙混,大同,大精,陕煤,俄罗斯煤,俄罗斯无烟,兴煤";
	info_type[2] = u8"新中港,神华精,伊泰,天马,特底灰,神混,神优,优一,神华,富动";
	size_t last_pos = 0;
	int index_child = 1;
	for (int i = 0; i < 3; i++)
	{
		size_t last_idx = info_type[i].rfind(',', info_type[i].size());
		last_pos = 0;
		index_child = 1;
		this->type_unit.type_type = i + 1;
		while (last_pos != last_idx + 1)
		{
			this->type_unit.type_name = info_type[i].substr(last_pos, info_type[i].find(',', last_pos) - last_pos);
			last_pos = info_type[i].find(',', last_pos) + 1;
			this->type_unit.type_index = index_child;
			index_child++;
			this->names.push_back(this->type_unit);
		}
		this->type_unit.type_name = info_type[i].substr(last_pos, info_type[i].find(',', last_pos) - last_pos);
		this->type_unit.type_index = index_child;
		this->names.push_back(this->type_unit);
	}
	//上级的开始和结束处用.分隔，其中成员用,分隔
	//节点名称,种类编号.（如果是皮带3：皮带长度,皮带带速（0表示3.15，1表示3.5））前一级(n).后一级(n).
	std::string info_nodes[74];
	info_nodes[0] = "1#,1..BC20A,BC20B.";
	info_nodes[1] = "2#,1..BC20A,BC20B.";
	info_nodes[2] = "3#,1..BC20A,BC20B.";
	info_nodes[3] = "4#,1..BC20A,BC20B.";
	info_nodes[4] = "11#,1..BC1A,BC1B.";
	info_nodes[5] = "12#,1..BC1A,BC1B.";
	info_nodes[6] = "13#,1..BC1A,BC1B.";
	info_nodes[7] = "14#,1..BC1A,BC1B.";
	info_nodes[8] = "SR6,3.BC8.BC8.";
	info_nodes[9] = "SR7,3.BC9.BC9.";
	info_nodes[10] = "SR8,3.BC10.BC10.";
	info_nodes[11] = "SR9,3.BC11.BC11.";
	info_nodes[12] = "SR10,3.BC12.BC12.";
	info_nodes[13] = "SR11,3.BC31.BC31.";
	info_nodes[14] = "SR12,3.XC2.XC2.";
	info_nodes[15] = "S12,3.BC36.BC36.";
	info_nodes[16] = "SL4,4.BC35..";
	info_nodes[17] = "silo,6.ZW12..";
	info_nodes[18] = "building_A,7.BC33..";		//A楼即装车楼2
	info_nodes[19] = "building_B,7.BC15..";		//B楼即装车楼1
	info_nodes[20] = "炼化,8.BC18B..";
	info_nodes[21] = "DS3,9..BC19.";			//来自3泊位
	info_nodes[22] = "BC1A,2.380,0.11#,12#,13#,14#.BC2A,BC2B.";
	info_nodes[23] = "BC1B,2.380,0.11#,12#,13#,14#.BC2A,BC2B.";
	info_nodes[24] = "BC2A,2.1192,0.YD1,YD2,BC1A,BC1B,BC19.BC3A.";
	info_nodes[25] = "BC2B,2.1192,0.YD1,YD2,BC1A,BC1B,BC19.BC3B.";
	info_nodes[26] = "BC3A,2.45,0.BC2A.BC4A,BC4B,BC10,BC6A.";
	info_nodes[27] = "BC3B,2.45,0.BC2B.BC4A,BC4B,BC10,BC6B.";
	info_nodes[28] = "BC4A,2.205,0.BC3A,BC3B.BC5,BC9.";
	info_nodes[29] = "BC4B,2.205,0.BC3A,BC3B.BC5,BC9.";
	info_nodes[30] = "BC5,2.280,0.BC4A,BC4B.BC8.";
	info_nodes[31] = "BC6A,2.230,1.BC3A,BC3B,BC26.BC7,BC11.";
	info_nodes[32] = "BC6B,2.230,1.BC3B,BC26.BC7A,BC7,BC11.";
	info_nodes[33] = "BC7,2.235,1.BC6A,BC6B.BC12.";
	info_nodes[34] = "BC8,2.1230,1.BC5,SR6.SR6,BC13,BC17.";
	info_nodes[35] = "BC9,2.1380,1.BC4A,BC4B,SR7.SR7,BC13,BC17.";
	info_nodes[36] = "BC10,2.1380,1.BC3A,BC3B,SR8.SR8,BC13,BC17.";
	info_nodes[37] = "BC11,2.1380,1.BC6A,BC6B,SR9.SR9,BC18A,BC18B.";
	info_nodes[38] = "BC12,2.1390,1.BC7,SR10.SR10,BC18A,BC18B.";
	info_nodes[39] = "BC13,2.665,0.BC8,BC9,BC10.BC14.";
	info_nodes[40] = "BC14,2.610,0.BC13.BC15.";
	info_nodes[41] = "BC15,2.485,0.BC14.building_B.";
	info_nodes[42] = "BC17,2.665,0.BC8,BC9,BC10.BC18A,BC18B.";
	info_nodes[43] = "BC18A,2.458,0.BC11,BC12,BC17.ZW11.";
	info_nodes[44] = "BC18B,2.490,0.BC11,BC12,BC17.ZW11,炼化.";
	info_nodes[45] = "BC19,2.320,0.DS3.BC2A,BC2B.";
	info_nodes[46] = "BC20A,2.914,0.1#,2#,3#,4#.BC21A.";
	info_nodes[47] = "BC20B,2.945,0.1#,2#,3#,4#.BC21B.";
	info_nodes[48] = "BC21A,2.265,0.BC20A.BC22A.";
	info_nodes[49] = "BC21B,2.265,0.BC20B.BC22B.";
	info_nodes[50] = "BC22A,2.1416,0.BC21A.BC23A,BC23B.";
	info_nodes[51] = "BC22B,2.1410,0.BC21B.BC23A,BC23B.";
	info_nodes[52] = "BC23A,2.646,0.BC22A,BC22B.BC24A.";
	info_nodes[53] = "BC23B,2.635,0.BC22A,BC22B.BC24B,BC30A,BC30B.";
	info_nodes[54] = "BC24A,2.965,0.BC23A.BC25A.";
	info_nodes[55] = "BC24B,2.970,0.BC23B.BC25B.";
	info_nodes[56] = "BC25A,2.696,0.BC24A.YD1.";
	info_nodes[57] = "BC25B,2.696,0.BC24B.YD2.";
	info_nodes[58] = "BC26,2.395,0.YD1,YD2.BC6A,BC6B.";
	info_nodes[59] = "BC30A,2.235,0.BC23A,BC23B.BC31.";
	info_nodes[60] = "BC30B,2.436,0.BC23B.BC36.";
	info_nodes[61] = "BC31,2.645,0.BC30A,SR11.SR11,BC32.";
	info_nodes[62] = "BC32,2.155,0.BC31.BC33.";
	info_nodes[63] = "BC33,2.95,0.BC32,BC34.building_A.";
	info_nodes[64] = "BC34,2.360,0.ground.BC33.";
	info_nodes[65] = "BC35,2.365,0.BC20B.SL4.";
	info_nodes[66] = "BC36,2.255,0.BC30B.S12.";
	info_nodes[67] = "YD1,2.34,0.BC25A.BC2A,BC2B,BC26.";
	info_nodes[68] = "YD2,2.34,0.BC25B.BC2A,BC2B,BC26,XC1.";
	info_nodes[69] = "XC1,2.58,0.YD2.XC2.";
	info_nodes[70] = "XC2,2.600,0.XC1.SR12.";
	info_nodes[71] = "ZW11,2.378,0.BC18A,BC18B.ZW12.";
	info_nodes[72] = "ZW12,2.400,0.ZW11.silo.";
	info_nodes[73] = "ground,10..BC34.";		//地坑
	//数据读取
	this->node.this_code = 0;
	this->node.wheelMode = 0;
	this->node.convLength = 0;
	this->node.convSpeed = 0;
	this->prepos.act = false;
	size_t comma = 0;		//逗号标记
	size_t dot = 0;			//句号标记
	for (int i = 0; i < 74; i++)
	{
		last_pos = 0;
		this->node.name = info_nodes[i].substr(0, info_nodes[i].find(',', 0));
		last_pos = info_nodes[i].find(',', 0) + 1;
		this->node.type = stoi(info_nodes[i].substr(last_pos, info_nodes[i].find('.', last_pos) - last_pos));
		last_pos = info_nodes[i].find('.', last_pos) + 1;
		if (this->node.type == 2)
		{
			this->node.convLength = stoi(info_nodes[i].substr(last_pos, info_nodes[i].find(',', last_pos) - last_pos));
			last_pos = info_nodes[i].find(',', last_pos) + 1;
			this->node.convSpeed = stoi(info_nodes[i].substr(last_pos, info_nodes[i].find('.', last_pos) - last_pos)) ? 3.5f : 3.15f;
			last_pos = info_nodes[i].find('.', last_pos) + 1;
		}
		//录入前端
		this->node.pre_codes.clear();
		dot = info_nodes[i].find('.', last_pos);
		if (last_pos == dot)
		{
			//没有前端时的位置修正
			last_pos++;
		}
		while (last_pos < dot)
		{
			comma = info_nodes[i].find(',', last_pos);
			if (comma == -1)
			{
				//找不到，就设为数组最后一个
				comma = info_nodes[i].size();
			}
			this->prepos.other_name = info_nodes[i].substr(last_pos, std::min(comma, dot) - last_pos);
			this->node.pre_codes.push_back(this->prepos);
			last_pos = std::min(comma, dot) + 1;
		}
		//录入后端
		this->node.pos_codes.clear();
		dot = info_nodes[i].rfind('.');
		while (last_pos < dot)
		{
			comma = info_nodes[i].find(',', last_pos);
			if (comma == -1)
			{
				comma = info_nodes[i].size();
			}
			this->prepos.other_name = info_nodes[i].substr(last_pos, std::min(comma, dot) - last_pos);
			this->node.pos_codes.push_back(this->prepos);
			last_pos = std::min(comma, dot) + 1;
		}
		this->nodes.push_back(this->node);
		this->node.this_code++;
	}

}

void Web::reset()
{
	this->fragments.clear();
	for (std::vector<Node>::iterator it1 = this->nodes.begin(); it1 != this->nodes.end(); it1++)
	{
		for (std::vector<PrePos>::iterator it2 = it1->pre_codes.begin(); it2 != it1->pre_codes.end(); it2++)
		{
			it2->act = false;
		}
		for (std::vector<PrePos>::iterator it3 = it1->pos_codes.begin(); it3 != it1->pos_codes.end(); it3++)
		{
			it3->act = false;
		}
		it1->wheelMode = false;
	}
}

void Web::initGuiStyle()
{
	this->style = &ImGui::GetStyle();
}

void Web::update()
{






}

void Web::add_type(std::string str_name, int type_type)
{
	int lst_idx = 1;		//原对应大类中小类序号最大者
	for (std::vector<Type_web>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
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
}

void Web::web_dispatch(bool unreal)
{
	static int node_selected = -1;
	if (ImGui::CollapsingHeader(u8"节点状态显示"))
	{
		ImGui::Indent();
		for (std::vector<Node>::iterator it1 = this->nodes.begin(); it1 != this->nodes.end(); it1++)
		{
			if (it1->name == "炼化")
			{
				ImGui::Text(u8"节点名称：炼化");
			}
			else
			{
				ImGui::Text((u8"节点名称：" + it1->name).c_str());
			}
			ImGui::SameLine();
			ImGui::Text((u8"	节点代码：" + std::to_string(it1->this_code)).c_str());
			switch (it1->type)
			{
			case 0:
				ImGui::Text(u8"节点种类：船舶");
				break;
			case 1:
				ImGui::Text(u8"节点种类：卸船机");
				break;
			case 2:
				ImGui::Text(u8"节点种类：皮带");
				ImGui::SameLine();
				ImGui::Text((u8"	带速：" + std::to_string(it1->convSpeed) + u8"米/秒").c_str());
				ImGui::SameLine();
				ImGui::Text((u8"	带长：" + std::to_string(it1->convLength) + u8"米").c_str());
				break;
			case 3:
				ImGui::Text(u8"节点种类：斗轮机");
				ImGui::SameLine();
				if (it1->wheelMode)
				{
					ImGui::Text(u8"	运行模式：取料运行");
				}
				else
				{
					ImGui::Text(u8"	运行模式：堆料运行");
				}
				break;
			case 4:
				ImGui::Text(u8"节点种类：装船机");
				break;
			case 5:
				ImGui::Text(u8"节点种类：堆场");
				break;
			case 6:
				ImGui::Text(u8"节点种类：筒仓");
				break;
			case 7:
				ImGui::Text(u8"节点种类：装车楼");
				break;
			case 8:
				ImGui::Text(u8"节点种类：炼化直出");
				break;
			case 9:
				ImGui::Text(u8"节点种类：来自3泊位的输入");
				break;
			case 10:
				ImGui::Text(u8"节点种类：地坑");
				break;
			default:
				break;
			}
			std::string temp = node_selected == it1->this_code ? u8"隐藏" : u8"显示";
			this->pre_button(node_selected == it1->this_code);
			if (ImGui::Button((temp + u8"本(" + std::to_string(it1->this_code) + u8")节点的关联节点").c_str()))
			{
				if (node_selected == it1->this_code)
				{
					node_selected = -1;
				}
				else
				{
					node_selected = it1->this_code;
				}
			}
			this->post_button();
			if (node_selected == it1->this_code)
			{
				if (it1->pre_codes.size())
				{
					ImGui::Text(u8"上一级节点：");
					for (std::vector<PrePos>::iterator it2 = it1->pre_codes.begin(); it2 != it1->pre_codes.end(); it2++)
					{
						ImGui::Text(it2->other_name.c_str());
						ImGui::SameLine();
						if (it2->act)
						{
							ImGui::Text(u8"	状态：激活");
						}
						else
						{
							ImGui::Text(u8"	状态：断开");
						}
					}
				}
				else
				{
					ImGui::Text(u8"本节点为起始节点");
				}
				if (it1->pos_codes.size())
				{
					ImGui::Text(u8"下一级节点：");
					for (std::vector<PrePos>::iterator it3 = it1->pos_codes.begin(); it3 != it1->pos_codes.end(); it3++)
					{
						if (it3->other_name == "炼化")
						{
							ImGui::Text(u8"炼化");
						}
						else
						{
							ImGui::Text(it3->other_name.c_str());
						}
						ImGui::SameLine();
						if (it3->act)
						{
							ImGui::Text(u8"	状态：激活");
						}
						else
						{
							ImGui::Text(u8"	状态：断开");
						}
					}
				}
				else
				{
					ImGui::Text(u8"本节点为结束节点");
				}
			}
			if (it1->this_code != 73)
			{
				ImGui::Separator();
			}
		}
		ImGui::Unindent();
	}
}

void Web::pre_button(bool choosed)
{
	this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
	if (choosed)
	{
		this->style->Colors[ImGuiCol_Button] = ImColor(0, 200, 0, 255);
	}
	else
	{
		this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
	}
}

void Web::post_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}
