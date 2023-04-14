#include "Web.h"

#define SPEEDLAG 0.0001f
#define UNIT_AMOUNT 0.220f	//每米皮带装载煤炭重量（吨）

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
	info_nodes[20] = "炼化,8.BC18B..";			//炼化直出
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
	this->node.convPos = 0.0f;
	this->node.out = 0;
	this->node.out_flux = 0.0f;
	this->node.out_type = 0;
	this->node.out_index = 0;
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

void Web::update(SimuCore& simucore, float simurate)
{
	FindNode findnode;
	for (std::vector<Fragment>::iterator it1 = this->fragments.begin(); it1 != this->fragments.end(); )
	{
		int erased = false;
		//寻找当前片段的起始节点之首
		findnode.target = *it1->froms.begin();
		std::vector<Node>::iterator it_from_first = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		//寻找当前片段的第一节皮带节点
		int temp = 0;
		for (std::vector<PrePos>::iterator it_0 = it_from_first->pos_codes.begin(); it_0 != it_from_first->pos_codes.end(); it_0++)
		{
			if (it_0->act)
			{
				findnode.target = it_0->other_name;
				temp = 1;
				break;
			}
		}
		if (temp == 0)
		{
			//没有找到始源节点的下一个节点，跳过本片段不更新
			continue;
		}
		std::vector<Node>::iterator it_begin = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		//寻找当前片段的起终止节点
		findnode.target = it1->to;
		std::vector<Node>::iterator it_to = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		//寻找当前片段的最后一节皮带节点
		for (std::vector<PrePos>::iterator it_0 = it_to->pre_codes.begin(); it_0 != it_to->pre_codes.end(); it_0++)
		{
			if (it_0->act)
			{
				findnode.target = it_0->other_name;
				break;
			}
		}
		std::vector<Node>::iterator it_end = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		//更新前排
		std::vector<Node>::iterator it_cur;
		if (it1->begin < it1->total_length)
		{
			//前排固定向前推进
			it_cur = it_begin;
			while (it1->begin < it_cur->convPos)
			{
				std::vector<PrePos>::iterator it2 = it_cur->pos_codes.begin();
				for (; it2 != it_cur->pos_codes.end(); it2++)
				{
					if (it2->act)
					{
						break;
					}
				}
				findnode.target = it2->other_name;
				it_cur = find_if(this->nodes.begin(), this->nodes.end(), findnode);
			}
			it1->begin += it_cur->convSpeed * simurate * SPEEDLAG;
			if (it1->begin > it1->total_length)
			{
				it1->begin = it1->total_length;
			}
		}
		//更新后排
		if (it1->disconnected)
		{
			//此时此片段已经不接受来自源头的补给，所以后排向前推进
			it_cur = it_begin;
			while (it1->end < it_cur->convPos)
			{
				std::vector<PrePos>::iterator it2 = it_cur->pos_codes.begin();
				for (; it2 != it_cur->pos_codes.end(); it2++)
				{
					if (it2->act)
					{
						break;
					}
				}
				findnode.target = it2->other_name;
				it_cur = find_if(this->nodes.begin(), this->nodes.end(), findnode);
			}
			it1->end += it_cur->convSpeed * simurate * SPEEDLAG;
			if (it1->end > it1->total_length)
			{
				//当后排到达终止位置，删除此片段
				it1 = this->fragments.erase(it1);
				erased = true;
			}
		}
		//更新存量
		if (!it1->disconnected)
		{
			float delta_amount = 0.0f;		//存量增量
			if (it1->froms.size() == 1)
			{
				//源节点非泊位卸船机
				if (!it_from_first->out)
				{
					//源头输出停止，停止增加货物
					it1->disconnected = !it1->disconnected;
				}
				else
				{
					//源头有输入，增加货物
					delta_amount = it_from_first->out_flux * simurate * SPEEDLAG;
				}
			}
			else
			{
				//源节点是4台泊位卸船机
				std::vector<std::string>::iterator it_name = it1->froms.begin();
				findnode.target = *it_name;
				std::vector<Node>::iterator it_from_1 = find_if(this->nodes.begin(), this->nodes.end(), findnode);
				it_name++;
				findnode.target = *it_name;
				std::vector<Node>::iterator it_from_2 = find_if(this->nodes.begin(), this->nodes.end(), findnode);
				it_name++;
				findnode.target = *it_name;
				std::vector<Node>::iterator it_from_3 = find_if(this->nodes.begin(), this->nodes.end(), findnode);
				it_name++;
				findnode.target = *it_name;
				std::vector<Node>::iterator it_from_4 = find_if(this->nodes.begin(), this->nodes.end(), findnode);
				if (!it_from_1->out && !it_from_2->out && !it_from_3->out && !it_from_4->out)
				{
					//源头输出停止，停止增加货物
					it1->disconnected = !it1->disconnected;
				}
				else
				{
					//源头有输入，增加货物
					delta_amount = (it_from_1->out_flux + it_from_2->out_flux + it_from_3->out_flux + it_from_4->out_flux) * simurate * SPEEDLAG;
				}
			}
			//可以添加流入流量的随机波动
			

			it1->amount += delta_amount;
		}
		if (it1->begin == it1->total_length)
		{
			//末尾流入终止节点，减少货物
			it1->amount -= it_end->convSpeed * simurate * SPEEDLAG * UNIT_AMOUNT;
			if (it1->amount < 0.0f)
			{
				it1->amount = 0.0f;
			}
		}
		if (!erased)
		{
			it1++;
		}
	}
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

void Web::web_dispatch(bool unreal, bool authorised)
{
	static int node_selected = -1;		//服务于显示/隐藏节点
	static int node_choosed = -1;			//服务于强制指定节点属性
	if (ImGui::CollapsingHeader(u8"显示物流网"))
	{
		ImGui::Indent();
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
				if (it1->type == 1 || it1->type > 8 || (it1->type == 3 && it1->wheelMode))
				{
					//显示输出状态
					if (it1->out)
					{
						ImGui::Text(u8"状态：运行");
					}
					else
					{
						ImGui::Text(u8"状态：停机");
					}
					ImGui::SameLine();
					//对于起源节点，显示货物名称
					if (it1->out_type == 0)
					{
						ImGui::Text(u8"	当前无货物");
					}
					else
					{
						for (std::vector<Type_web>::const_iterator it4 = this->names.begin(); it4 != this->names.end(); it4++)
						{
							if (it4->type_index == it1->out_index && it4->type_type == it1->out_type)
							{
								ImGui::Text((u8"	当前货物名：" + it4->type_name).c_str());
								break;
							}
						}
					}
					//强制指定货物类别
					static int define_type = 1;
					static int define_index = 1;
					static float define_flux = 0.0f;
					if (authorised || unreal)
					{
						this->pre_button(node_choosed == it1->this_code);
						if (ImGui::Button((u8"强制指定本(" + std::to_string(it1->this_code) + u8")源节点的属性").c_str()))
						{
							if (node_choosed == it1->this_code)
							{
								node_choosed = -1;
							}
							else
							{
								node_choosed = it1->this_code;
							}
						}
						this->post_button();
						if (node_choosed == it1->this_code)
						{
							if (unreal)
							{
								ImGui::Text(u8"选择源货物大类：");
								ImGui::SameLine();
								ImGui::RadioButton(u8"大类散货", &define_type, 1);
								ImGui::SameLine();
								ImGui::RadioButton(u8"来源散货", &define_type, 2);
								ImGui::SameLine();
								ImGui::RadioButton(u8"企业散货", &define_type, 3);
								ImGui::Text(u8"选择货物小类：");
								int temp_1 = 1;
								for (std::vector<Type_web>::const_iterator it7 = this->names.begin(); it7 != this->names.end(); it7++)
								{
									if (it7->type_type == define_type)
									{
										ImGui::SameLine();
										ImGui::RadioButton(it7->type_name.c_str(), &define_index, temp_1);
										temp_1++;
									}
								}
								this->pre_button(0);
								if (ImGui::Button(u8"设定货物类"))
								{
									it1->out_index = define_index;
									it1->out_type = define_type;
								}
								this->post_button();
							}
							if (it1->out)
							{
								//当前节点正在输出
								this->pre_button(0);
								if (ImGui::Button(u8"停止当前节点的输出"))
								{
									this->end_input(it1->name);
								}
								this->post_button();
								ImGui::Text(u8"指定本节点的输出流量：");
								ImGui::SameLine();
								ImGui::SetNextItemWidth(200.0f);
								ImGui::SliderFloat(u8"吨/米", &define_flux, 0.0f, 10.0f);
								ImGui::SameLine();
								this->pre_button(0);
								if (ImGui::Button(u8"设定当前节点的输出流量"))
								{
									it1->out_flux = define_flux;
								}
								this->post_button();
								ImGui::Separator();
							}
							else
							{
								this->pre_button(0);
								if (ImGui::Button(u8"开始当前节点的输出"))
								{
									this->start_input(it1->name, it1->out_type, it1->out_index, it1->out_flux);
								}
								this->post_button();
								ImGui::Separator();
							}
						}
					}
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
		if (ImGui::CollapsingHeader(u8"片段状态显示"))
		{
			ImGui::Indent();
			int i = 0;
			if (this->fragments.empty())
			{
				ImGui::Text(u8"当前无片段");
			}
			for (std::vector<Fragment>::iterator it5 = this->fragments.begin(); it5 != this->fragments.end(); )
			{
				bool deleted = false;
				ImGui::Text((u8"片段" + std::to_string(i + 1)).c_str());
				i++;
				ImGui::Text(u8"起源设备组：");
				for (std::vector<std::string>::iterator it6 = it5->froms.begin(); it6 != it5->froms.end(); it6++)
				{
					ImGui::SameLine();
					ImGui::Text(it6->c_str());
				}
				ImGui::Text((u8"终止设备名：" + it5->to).c_str());
				ImGui::Text((u8"流程总长度：" + std::to_string(it5->total_length) + u8"米").c_str());
				//煤炭片段进度条
				//ImGui::Text((u8"头部位置：" + std::to_string(it5->begin) + u8"米").c_str());
				float pos_head = it5->begin;
				ImGui::SliderFloat(u8"头部位置（米）", &pos_head, 0.0f, it5->total_length);
				//ImGui::Text((u8"尾部位置：" + std::to_string(it5->end) + u8"米").c_str());
				float pos_tail = it5->end;
				ImGui::SliderFloat(u8"尾部位置（米）", &pos_tail, 0.0f, it5->total_length);
				if (it5->disconnected)
				{
					ImGui::Text(u8"当前已脱钩");
				}
				else
				{
					ImGui::Text(u8"当前未脱钩");
				}
				ImGui::Text((u8"当前载货量：" + std::to_string(it5->amount) + u8"吨").c_str());
				if (it5->cargo_type == 0)
				{
					ImGui::Text(u8"当前无货物");
				}
				else
				{
					for (std::vector<Type_web>::const_iterator it6 = this->names.begin(); it6 != this->names.end(); it6++)
					{
						if (it6->type_index == it5->cargo_index && it6->type_type == it5->cargo_type)
						{
							ImGui::Text((u8"当前货物名：" + it6->type_name).c_str());
							break;
						}
					}
				}
				if (unreal)
				{
					this->pre_button(0);
					if (ImGui::Button((u8"删除片段" + std::to_string(i)).c_str()))
					{
						it5 = this->fragments.erase(it5);
						deleted = true;
					}
					this->post_button();
				}

				if (it5 + 1 != this->fragments.end())
				{
					ImGui::Separator();
				}
				if (!deleted)
				{
					it5++;
				}
			}
			ImGui::Unindent();
		}
		ImGui::Unindent();
	}
}

void Web::start_input(std::string from_name, int type, int index, float flux)
{
	//启动指定一个输出节点的输出（输入起源节点本身的名字，如1#，13#，ground，SR6，DS3），并重新计算流程中皮带的累计长度
	FindNode findnode;
	std::vector<Node>::iterator it;
	findnode.target = from_name;
	it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
	//在之前没有处于输出状态的情况下创建一个新的煤片段
	if (!it->out)
	{
		//寻找终止节点位置，并计算从这个起源节点到其终止节点的总长度
		bool allowed = true;	//是否允许添加片段
		int temp = 0;
		//如果找到最后的节点不是终止节点类型，就不添加煤片段；如果任何一个中间节点的后节点标记全部未激活，也不添加煤片段
		float length_accumulate = 0.0f;
		bool next = true;		//是否允许继续遍历标志符，当当前节点的后面有激活的节点时，继续遍历；或当前节点为终止节点，停止遍历
		std::vector<Node>::iterator it1;		//内部使用node迭代器
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			if (it0->act)
			{
				findnode.target = it0->other_name;
				temp = 1;
				break;
			}
		}
		if (temp == 0)
		{
			//源节点没有激活的后续节点，无法输出，跳过
			allowed = false;
		}
		else
		{
			it1 = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		}
		//it1指向第一段皮带
		while (allowed && next)
		{
			if (it1->type == 2)
			{
				//本节点是皮带节点
				//累计工作
				length_accumulate += it1->convLength;
				it1->convPos = length_accumulate;
				//向下一级迭代
				temp = 0;
				for (std::vector<PrePos>::iterator it0 = it1->pos_codes.begin(); it0 != it1->pos_codes.end(); it0++)
				{
					if (it0->act)
					{
						findnode.target = it0->other_name;
						temp = 1;
						break;
					}
				}
				if (temp == 0)
				{
					//本节点没有激活的后续节点，无法输出，跳过
					allowed = false;
				}
				else
				{
					it1 = find_if(this->nodes.begin(), this->nodes.end(), findnode);
				}
			}
			else
			{
				//本节点不是皮带节点，迭代结束
				next = false;
			}
		}
		//若最终迭代到的节点不是终止节点类型（其中包括处于取料模式的斗轮机），就不允许添加片段
		if (allowed && (it1->type == 0 || it1->type == 1 || it1->type == 5 || it1->type == 9 || it1->type == 10 || (it1->type == 3 && it1->wheelMode == 1)))
		{
			allowed = false;
		}
		//添加新片段
		if (allowed)
		{
			this->fragment.froms.clear();
			//若起源节点是卸船机，就将4个并列打包；否则直接压栈
			if (*from_name.rbegin() == '#')
			{
				std::string unloaderNamesUp[4] = { "1#", "2#", "3#", "4#" };
				std::string unloaderNamesDown[4] = { "11#", "12#", "13#", "14#" };
				if (from_name.size() == 2)
				{
					//通用泊位
					for (int i = 0; i < 4; i++)
					{
						this->fragment.froms.push_back(unloaderNamesUp[i]);
					}
				}
				else
				{
					//4泊位
					for (int i = 0; i < 4; i++)
					{
						this->fragment.froms.push_back(unloaderNamesDown[i]);
					}
				}
			}
			else
			{
				this->fragment.froms.push_back(from_name);
			}
			this->fragment.to = it1->name;
			this->fragment.total_length = length_accumulate;
			this->fragment.begin = 0.0f;
			this->fragment.end = 0.0f;
			this->fragment.disconnected = false;
			this->fragment.amount = 0.0f;
			this->fragment.cargo_type = type;
			this->fragment.cargo_index = index;
			this->fragments.push_back(this->fragment);
		}
	}
	it->out = 1;
	it->out_flux = flux;
	it->out_type = type;
	it->out_index = index;
}

void Web::end_input(std::string from_name)
{
	//结束指定一个输出节点的输出（输入起源节点本身的名字，如1#，13#，ground，SR6，DS3）
	FindNode findnode;
	std::vector<Node>::iterator it;
	findnode.target = from_name;
	it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
	it->out = 0;
	it->out_flux = 0.0f;
}

void Web::begin_flow(std::vector<std::string>& equipments, int type, int index, float flux, bool unloader[4])
{
	//流程开始录入
	FindNode findnode;
	std::vector<Node>::iterator it;
	//处理输入端，处理结束后要求设定源节点属性和第一条皮带的头、尾连接及其初始累计长度
	float length_accumulate = 0.0f;		//当前累计皮带长度
	std::vector<std::string>::iterator it_begin = ++equipments.begin();
	if (*equipments.begin() == "BC20A" || *equipments.begin() == "BC20B")
	{
		//通用泊位
		std::string unloaderNames[4] = { "1#", "2#", "3#", "4#" };
		for (int i = 0; i < 4; i++)
		{
			//设定装船机
			findnode.target = unloaderNames[i];
			it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
			it->out = unloader[i] ? 1 : 0;
			it->out_flux = unloader[i] ? flux : 0.0f;
			it->out_type = type;
			it->out_index = index;
			for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
			{
				it0->act = it0->other_name == *equipments.begin() ? 1 : 0;
			}
		}
		//设定起始皮带
		findnode.target = *equipments.begin();
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		length_accumulate = (float)it->convLength;
		it->convPos = length_accumulate;
		for (std::vector<PrePos>::iterator it0 = it->pre_codes.begin(); it0 != it->pre_codes.end(); it0++)
		{
			it0->act = 1;
		}
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = it0->other_name == *(++equipments.begin()) ? 1 : 0;
		}
	}
	else if (*equipments.begin() == "BC1A" || *equipments.begin() == "BC1B")
	{
		//4泊位
		std::string unloaderNames[4] = { "11#", "12#", "13#", "14#" };
		for (int i = 0; i < 4; i++)
		{
			findnode.target = unloaderNames[i];
			it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
			it->out = unloader[i] ? 1 : 0;
			it->out_flux = unloader[i] ? flux : 0.0f;
			it->out_type = type;
			it->out_index = index;
			for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
			{
				it0->act = it0->other_name == *equipments.begin() ? 1 : 0;
			}
		}
		//设定起始皮带
		findnode.target = *equipments.begin();
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		length_accumulate = (float)it->convLength;
		it->convPos = length_accumulate;
		for (std::vector<PrePos>::iterator it0 = it->pre_codes.begin(); it0 != it->pre_codes.end(); it0++)
		{
			it0->act = 1;
		}
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = it0->other_name == *(++equipments.begin()) ? 1 : 0;
		}
	}
	else if (*equipments.begin() == "BC19" || *equipments.begin() == "BC34")
	{
		//3泊位或地坑
		findnode.target = *equipments.begin() == "BC19" ? "DS3" : "ground";
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		it->out = 1;
		it->out_flux = flux;
		it->out_type = type;
		it->out_index = index;
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = it0->other_name == *equipments.begin() ? 1 : 0;
		}
		//设定起始皮带
		findnode.target = *equipments.begin();
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		length_accumulate = (float)it->convLength;
		it->convPos = length_accumulate;
		std::vector<PrePos>::iterator it0 = it->pre_codes.begin();
		it0->act = 1;
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = it0->other_name == *(++equipments.begin()) ? 1 : 0;
		}
	}
	else if ((*equipments.begin())[0] == 'S')
	{
		//斗轮机
		it_begin++;
		findnode.target = *equipments.begin();
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		it->wheelMode = 1;
		it->out = 1;
		it->out_flux = flux;
		it->out_type = type;
		it->out_index = index;
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = it0->other_name == *(++equipments.begin()) ? 1 : 0;
		}
		//设定起始皮带
		findnode.target = *(++equipments.begin());
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		length_accumulate = (float)it->convLength;
		it->convPos = length_accumulate;
		for (std::vector<PrePos>::iterator it0 = it->pre_codes.begin(); it0 != it->pre_codes.end(); it0++)
		{
			it0->act = it0->other_name == *equipments.begin() ? 1 : 0;
		}
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = it0->other_name == *(++(++equipments.begin())) ? 1 : 0;
		}
	}
	//处理皮带，皮带一律遍历到倒数第二个位置，除去出口，遍历从第二条皮带开始
	for (; it_begin != --equipments.end(); it_begin++)
	{
		findnode.target = *it_begin;
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		length_accumulate += (float)it->convLength;
		it->convPos = length_accumulate;
		for (std::vector<PrePos>::iterator it0 = it->pre_codes.begin(); it0 != it->pre_codes.end(); it0++)
		{
			it0->act = it0->other_name == *(it_begin - 1) ? 1 : 0;
		}
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = it0->other_name == *(it_begin + 1) ? 1 : 0;
		}
	}
	//处理末尾
	if (*equipments.rbegin() == "SL4" || (*equipments.rbegin())[0] == 'b' || *equipments.rbegin() == "炼化" || *equipments.rbegin() == "silo" || (*equipments.rbegin())[0] == 'S')
	{
		//装船机或装车楼或直出或筒仓或斗轮机
		findnode.target = *equipments.rbegin();
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		if ((*equipments.rbegin())[0] == 'S')
		{
			//斗轮机
			it->wheelMode = 0;
		}
		for (std::vector<PrePos>::iterator it0 = it->pre_codes.begin(); it0 != it->pre_codes.end(); it0++)
		{
			it0->act = it0->other_name == *(++equipments.rbegin()) ? 1 : 0;
		}
	}
	if ((*equipments.begin() == "BC20A" || *equipments.begin() == "BC20B" || *equipments.begin() == "BC1A" || *equipments.begin() == "BC1B") && !unloader[0] && !unloader[1] && !unloader[2] && !unloader[3])
	{
		//创建一个煤片段（若此流程是泊位堆料流程，且4台卸船机全不输出，就不创建）
	}
	else
	{
		this->fragment.froms.clear();
		//若起源节点是卸船机，就将4个并列打包；否则直接压栈
		if (*equipments.begin() == "BC20A" || *equipments.begin() == "BC20B" || *equipments.begin() == "BC1A" || *equipments.begin() == "BC1B")
		{
			std::string unloaderNamesUp[4] = { "1#", "2#", "3#", "4#" };
			std::string unloaderNamesDown[4] = { "11#", "12#", "13#", "14#" };
			if (*equipments.begin() == "BC20A" || *equipments.begin() == "BC20B")
			{
				//通用泊位
				for (int i = 0; i < 4; i++)
				{
					this->fragment.froms.push_back(unloaderNamesUp[i]);
				}
			}
			else
			{
				//4泊位
				for (int i = 0; i < 4; i++)
				{
					this->fragment.froms.push_back(unloaderNamesDown[i]);
				}
			}
		}
		else if (*equipments.begin() == "BC19" || *equipments.begin() == "BC34")
		{
			std::string temp = *equipments.begin() == "BC19" ? "DS3" : "ground";
			this->fragment.froms.push_back(temp);
		}
		else if ((*equipments.begin())[0] == 'S')
		{
			this->fragment.froms.push_back(*equipments.begin());
		}
		this->fragment.to = *equipments.rbegin();
		this->fragment.total_length = length_accumulate;
		this->fragment.begin = 0.0f;
		this->fragment.end = 0.0f;
		this->fragment.disconnected = false;
		this->fragment.amount = 0.0f;
		this->fragment.cargo_type = type;
		this->fragment.cargo_index = index;
		this->fragments.push_back(this->fragment);
	}
}

void Web::end_flow(std::vector<std::string>& equipments)
{
	//使源节点的输出模式关闭
	FindNode findnode;
	std::vector<Node>::iterator it;
	if (*equipments.begin() == "BC20A" || *equipments.begin() == "BC20B")
	{
		//通用泊位
		std::string unloaderNames[4] = { "1#", "2#", "3#", "4#" };
		for (int i = 0; i < 4; i++)
		{
			//设定装船机
			findnode.target = unloaderNames[i];
			it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
			it->out = 0;
			it->out_flux = 0.0f;
		}
	}
	else if (*equipments.begin() == "BC1A" || *equipments.begin() == "BC1B")
	{
		//4泊位
		std::string unloaderNames[4] = { "11#", "12#", "13#", "14#" };
		for (int i = 0; i < 4; i++)
		{
			findnode.target = unloaderNames[i];
			it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
			it->out = 0;
			it->out_flux = 0.0f;
		}
	}
	else if (*equipments.begin() == "BC19" || *equipments.begin() == "BC34")
	{
		//3泊位或地坑
		findnode.target = *equipments.begin() == "BC19" ? "DS3" : "ground";
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		it->out = 0;
		it->out_flux = 0.0f;
	}
	else if ((*equipments.begin())[0] == 'S')
	{
		//斗轮机
		findnode.target = *equipments.begin();
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		it->out = 0;
		it->out_flux = 0.0f;
	}
}

void Web::emergencyShutDown(std::vector<std::string>& equipments)
{
	//流程紧急停机，遍历对应流程，将对应所有节点的前后链接全部脱钩
	FindNode findnode;
	std::vector<Node>::iterator it;
	std::vector<std::string>::iterator it_begin = ++equipments.begin();
	if (*equipments.begin() == "BC20A" || *equipments.begin() == "BC20B")
	{
		//通用泊位
		std::string unloaderNames[4] = { "1#", "2#", "3#", "4#" };
		for (int i = 0; i < 4; i++)
		{
			//设定装船机
			findnode.target = unloaderNames[i];
			it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
			it->out = 0;
			it->out_flux = 0.0f;
			for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
			{
				it0->act = 0;
			}
		}
		//设定起始皮带
		findnode.target = *equipments.begin();
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		for (std::vector<PrePos>::iterator it0 = it->pre_codes.begin(); it0 != it->pre_codes.end(); it0++)
		{
			it0->act = 0;
		}
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = 0;
		}
	}
	else if (*equipments.begin() == "BC1A" || *equipments.begin() == "BC1B")
	{
		//4泊位
		std::string unloaderNames[4] = { "11#", "12#", "13#", "14#" };
		for (int i = 0; i < 4; i++)
		{
			findnode.target = unloaderNames[i];
			it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
			it->out = 0;
			it->out_flux = 0.0f;
			for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
			{
				it0->act = 0;
			}
		}
		//设定起始皮带
		findnode.target = *equipments.begin();
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		for (std::vector<PrePos>::iterator it0 = it->pre_codes.begin(); it0 != it->pre_codes.end(); it0++)
		{
			it0->act = 0;
		}
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = 0;
		}
	}
	else if (*equipments.begin() == "BC19" || *equipments.begin() == "BC34")
	{
		//3泊位或地坑
		findnode.target = *equipments.begin() == "BC19" ? "DS3" : "ground";
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		it->out = 0;
		it->out_flux = 0.0f;
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = 0;
		}
		//设定起始皮带
		findnode.target = *equipments.begin();
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		std::vector<PrePos>::iterator it0 = it->pre_codes.begin();
		it0->act = 0;
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = 0;
		}
	}
	else if ((*equipments.begin())[0] == 'S')
	{
		//斗轮机
		it_begin++;
		findnode.target = *equipments.begin();
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		it->out = 0;
		it->out_flux = 0.0f;
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = 0;
		}
		//设定起始皮带
		findnode.target = *(++equipments.begin());
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		for (std::vector<PrePos>::iterator it0 = it->pre_codes.begin(); it0 != it->pre_codes.end(); it0++)
		{
			it0->act = 0;
		}
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = 0;
		}
	}
	//处理皮带，皮带一律遍历到倒数第二个位置，除去出口，遍历从第二条皮带开始
	for (; it_begin != --equipments.end(); it_begin++)
	{
		findnode.target = *it_begin;
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		for (std::vector<PrePos>::iterator it0 = it->pre_codes.begin(); it0 != it->pre_codes.end(); it0++)
		{
			it0->act = 0;
		}
		for (std::vector<PrePos>::iterator it0 = it->pos_codes.begin(); it0 != it->pos_codes.end(); it0++)
		{
			it0->act = 0;
		}
	}
	//处理末尾
	if (*equipments.rbegin() == "SL4" || (*equipments.rbegin())[0] == 'b' || *equipments.rbegin() == "炼化" || *equipments.rbegin() == "silo" || (*equipments.rbegin())[0] == 'S')
	{
		//装船机或装车楼或直出或筒仓或斗轮机
		findnode.target = *equipments.rbegin();
		it = find_if(this->nodes.begin(), this->nodes.end(), findnode);
		for (std::vector<PrePos>::iterator it0 = it->pre_codes.begin(); it0 != it->pre_codes.end(); it0++)
		{
			it0->act = 0;
		}
	}
}

bool Web::cancelCheck(std::vector<std::string>& equipments)
{
	bool ret = true;
	for (std::vector<Fragment>::iterator it1 = this->fragments.begin(); it1 != this->fragments.end(); it1++)
	{
		if (*equipments.rbegin() == it1->to)
		{
			ret = false;
		}
	}
	return ret;
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
