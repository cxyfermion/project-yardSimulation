#include "Yard.h"

#define CT_WIDTH 4100
#define CT_HEIGHT 2370
#define SCR_WIDTH 1440
#define SCR_HEIGHT 810

#define LAYER_YARD -0.001f
#define LOW_STOP 1000.0f			//堆场取料低位报警值

#define PROB_YARD 0.3f				//堆场初始有货的概率
#define PROB_CHILD_1 0.5f			//子堆场连续1个有货的概率
#define PROB_CHILD_2 0.7f			//子堆场连续2个有货的概率
#define PROB_CHILD_3 0.9f			//子堆场连续3个有货的概率

Yard::Yard()
{
	this->flow_yard_choosed_1 = 0;
	this->flow_yard_choosed_2 = 0;
	this->flow_num_choose = 0;
	this->yard_choosed = -1;
	this->child_choosed = -1;
	this->type_choosed = -1;
	this->index_choosed = -1;
	this->amount_choosed = 0.0f;
	this->yard_choosing = false;
	this->data_send = false;
	this->terminate_wheel = "";
	std::string info_yard[10];
	//上4号泊位堆场
	info_yard[0] = "A,SR6,NULL,2350,1100,24000,24000,24000,24000,24000,24000";
	info_yard[1] = "B,SR7,SR6,2080,1100,23000,26000,26000,26000,24000,26000,26000,26000,26000,17000";
	info_yard[2] = "C,SR8,SR7,1650,1100,23000,26000,26000,26000,26000,26000,26000,26000,26000,33000";
	info_yard[3] = "D,SR9,SR8,1190,1100,23000,23000,23000,23000,26000,26000,26000,26000,26000,33000";
	info_yard[4] = "E,SR10,SR9,840,1100,23000,23000,23000,26000,26000,26000,26000,26000,26000,38000";
	info_yard[5] = "F,SR10,NULL,525,1100,23000,30000,25000";
	//上三角场
	info_yard[6] = "G,SR11,NULL,3000,1030,8000,20000,20000,25000";
	info_yard[7] = "H,SR11,NULL,3290,1030,11000,23000,23000,26000";
	info_yard[8] = "K,S12,NULL,3515,885,20000,17000,18000";
	//西门二场
	info_yard[9] = "WEST2,SR12,NULL,680,1540,21000,21000,21000,26000,24000,21000,21000,26000";
	//初始化种类数据
	std::string info_type[3];
	info_type[0] = u8"碎石,水渣,工业盐,纸浆,黄砂,铜矿,石油焦,焦煤,化工煤,混煤,优混,无烟煤,沥青";
	info_type[1] = u8"大混,蒙混,大同,大精,陕煤,俄罗斯煤,俄罗斯无烟,兴煤";
	info_type[2] = u8"新中港,神华精,伊泰,天马,特底灰,神混,神优,优一,神华,富动";
	//数据初始化
	this->father.yard_pow = 100;
	this->father.yard_time = 0;
	this->father.yard_focus = false;
	this->father.yard_coords[2] = LAYER_YARD;
	this->father.yard_coords[3] = 0.0f;
	this->father.yard_coords[4] = 0.0f;
	this->father.yard_coords[5] = 0.0f;
	this->father.yard_coords[6] = 0.0f;
	this->child.child_cur_amount = 0.0f;
	this->child.child_flux_left = 0.0f;
	this->child.child_flux_right = 0.0f;
	this->child.child_index = 0;
	this->child.child_type = 0;
	this->child.child_state_left = 0;
	this->child.child_state_right = 0;
	this->child.child_focus = false;
	int last_pos = 0;
	int index_child = 1;
	std::string name;
	//初始化名称容器
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
	//初始化堆场
	for (int i = 0; i < 10; i++)
	{
		this->father.yard_index = i + 1;
		int last_idx = (int)(info_yard[i].rfind(',', info_yard[i].size()));
		last_pos = 0;
		this->father.yard_name = info_yard[i].substr(0, info_yard[i].find(',', 0));
		last_pos = (int)(info_yard[i].find(',', 0)) + 1;
		this->father.children.clear();
		this->father.wheel_available.clear();
		//斗轮机1
		name = info_yard[i].substr(last_pos, info_yard[i].find(',', last_pos) - last_pos);
		this->father.wheel_available.push_back(name);
		last_pos = (int)(info_yard[i].find(',', last_pos)) + 1;
		//斗轮机2
		name = info_yard[i].substr(last_pos, info_yard[i].find(',', last_pos) - last_pos);
		if (name != "NULL")
		{
			this->father.wheel_available.push_back(name);
		}
		last_pos = (int)(info_yard[i].find(',', last_pos)) + 1;
		//堆场坐标
		this->father.center_x = (float)(stoi(info_yard[i].substr(last_pos, info_yard[i].find(',', last_pos) - last_pos))) / (CT_WIDTH / 10.0f) - 5.0f;
		last_pos = (int)(info_yard[i].find(',', last_pos)) + 1;
		this->father.center_y = -(float)(stoi(info_yard[i].substr(last_pos, info_yard[i].find(',', last_pos) - last_pos))) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
		last_pos = (int)(info_yard[i].find(',', last_pos)) + 1;
		//子堆场
		if (this->father.yard_name == "F")
		{
			index_child = 6;
		}
		else
		{
			index_child = 1;
		}
		while (last_pos != last_idx + 1)
		{
			this->child.child_name = index_child;
			this->child.child_max_amount = (float)(stoi(info_yard[i].substr(last_pos, info_yard[i].find(',', last_pos) - last_pos)));
			this->father.children.push_back(this->child);
			last_pos = (int)(info_yard[i].find(',', last_pos)) + 1;
			index_child++;
		}
		this->child.child_name = index_child;
		this->child.child_max_amount = (float)(stoi(info_yard[i].substr(last_pos, info_yard[i].find(',', last_pos) - last_pos)));
		this->father.children.push_back(this->child);
		this->father.yard_coords[0] = this->father.center_x;
		this->father.yard_coords[1] = this->father.center_y;
		this->yards.push_back(this->father);
	}
	/*buffers*/
	glGenVertexArrays(1, &yardVAO);
	glGenBuffers(1, &yardVBO);
	glBindVertexArray(yardVAO);
	glBindBuffer(GL_ARRAY_BUFFER, yardVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->father.yard_coords), &this->father.yard_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void Yard::reset(SimuCore& core, bool rand_init)
{
	//统计三大类货物总数
	int count_types[3] = { 0,0,0 };
	if (rand_init)
	{
		for (std::vector<Type_yard>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
		{
			count_types[it->type_type - 1]++;
		}
	}
	int random_yard_stack = 0;	//随机生成连续有货的子堆场
	int type_temp = 0;				//临时随机生成货物大类
	int index_temp = 0;			//临时随机生成货物小类
	for (std::vector<Yard_father>::iterator it1 = this->yards.begin(); it1 != this->yards.end(); it1++)
	{
		it1->yard_time = 0;
		for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
		{
			it2->child_flux_left = 0.0f;
			it2->child_flux_right = 0.0f;
			it2->child_state_left = 0;
			it2->child_state_right = 0;
			it2->child_cur_amount = 0.0f;
			it2->child_type = 0;
			it2->child_index = 0;
			if (rand_init)
			{
				if (random_yard_stack == 0 && core.dice(PROB_YARD))
				{
					//判定这个子堆场生成货物
					if (core.dice(PROB_CHILD_1))
					{
						random_yard_stack = 1;
					}
					else
					{
						if (core.dice(PROB_CHILD_2))
						{
							random_yard_stack = 2;
						}
						else
						{
							if (core.dice(PROB_CHILD_3))
							{
								random_yard_stack = 3;
							}
							else
							{
								random_yard_stack = core.genRandomInt(4, 10);
							}
						}
					}
					//判定生成货物的种类
					type_temp = core.genRandomInt(1, 3);
					index_temp = core.genRandomInt(1, count_types[type_temp - 1]);
				}
				if (random_yard_stack > 0)
				{
					//生成货物
					it2->child_cur_amount = core.genRandomFloat(0.0f, it2->child_max_amount);
					it2->child_type = type_temp;
					it2->child_index = index_temp;
					random_yard_stack--;
				}
			}
		}
		if (rand_init)
		{
			//堆场末尾连续性检查
			random_yard_stack = 0;
		}
	}
}

void Yard::yard_random_initiator(SimuCore& core)
{
	//统计三大类货物总数
	int count_types[3] = { 0,0,0 };
	for (std::vector<Type_yard>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
	{
		count_types[it->type_type - 1]++;
	}
	int random_yard_stack = 0;	//随机生成连续有货的子堆场
	int type_temp = 0;				//临时随机生成货物大类
	int index_temp = 0;			//临时随机生成货物小类
	for (std::vector<Yard_father>::iterator it1 = this->yards.begin(); it1 != this->yards.end(); it1++)
	{
		for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
		{
			it2->child_flux_left = 0.0f;
			it2->child_flux_right = 0.0f;
			it2->child_state_left = 0;
			it2->child_state_right = 0;
			it2->child_cur_amount = 0.0f;
			it2->child_type = 0;
			it2->child_index = 0;
			if (random_yard_stack == 0 && core.dice(PROB_YARD))
			{
				//判定这个子堆场生成货物
				if (core.dice(PROB_CHILD_1))
				{
					random_yard_stack = 1;
				}
				else
				{
					if (core.dice(PROB_CHILD_2))
					{
						random_yard_stack = 2;
					}
					else
					{
						if (core.dice(PROB_CHILD_3))
						{
							random_yard_stack = 3;
						}
						else
						{
							random_yard_stack = core.genRandomInt(4, 10);
						}
					}
				}
				//判定生成货物的种类
				type_temp = core.genRandomInt(1, 3);
				index_temp = core.genRandomInt(1, count_types[type_temp - 1]);
			}
			if (random_yard_stack > 0)
			{
				//生成货物
				it2->child_cur_amount = core.genRandomFloat(0.0f, it2->child_max_amount);
				it2->child_type = type_temp;
				it2->child_index = index_temp;
				random_yard_stack--;
			}
		}
		//堆场末尾连续性检查
		random_yard_stack = 0;
	}
}

void Yard::drawYard(Camera& camera, Shader& yardShader)
{
	float value = (float)(0.3 * std::cos((float)glfwGetTime()) + 0.7);
	yardShader.use();
	this->model = glm::mat4(1.0f);
	yardShader.setMat4("model", this->model);
	this->projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
	yardShader.setMat4("projection", this->projection);
	this->view = glm::mat4(1.0f);
	this->view = camera.GetViewMatrix();
	yardShader.setMat4("view", this->view);
	glBindVertexArray(yardVAO);
	glBindBuffer(GL_ARRAY_BUFFER, yardVBO);
	for (std::vector<Yard_father>::iterator it1 = this->yards.begin(); it1 != this->yards.end(); it1++)
	{
		if (!it1->yard_focus)
		{
			for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
			{
				if (!it2->child_focus)
				{
					//子堆场y坐标计算
					if (it1->yard_name == "K")
					{
						if (it2->child_name == 1)
						{
							it1->yard_coords[0] = 3515.0f / (CT_WIDTH / 10.0f) - 5.0f;
							it1->yard_coords[1] = -770.0f / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
						}
						else if (it2->child_name == 2)
						{
							it1->yard_coords[0] = 3435.0f / (CT_WIDTH / 10.0f) - 5.0f;
							it1->yard_coords[1] = -980.0f / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
						}
						else if (it2->child_name == 3)
						{
							it1->yard_coords[0] = 3595.0f / (CT_WIDTH / 10.0f) - 5.0f;
							it1->yard_coords[1] = -980.0f / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
						}
					}
					else if (it1->yard_name == "WEST2")
					{
						it1->yard_coords[0] = (525.0f + (float)((it2->child_name - 1) % 4) * 100.0f) / (CT_WIDTH / 10.0f) - 5.0f;
						it1->yard_coords[1] = -((float)(it2->child_name / 5) * 180.0f + 1450.0f) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
					}
					else if (it1->yard_name == "F")
					{
						it1->yard_coords[0] = it1->center_x;
						it1->yard_coords[1] = -(960.0f + (it2->child_name - 1) * (310.0f / 8)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
					}
					else if (it1->yard_name == "G" || it1->yard_name == "H")
					{
						it1->yard_coords[0] = it1->center_x;
						it1->yard_coords[1] = -(900.0f + (it2->child_name - 1) * (270.0f / 4)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
					}
					else if (it1->yard_name == "A")
					{
						it1->yard_coords[0] = it1->center_x;
						it1->yard_coords[1] = -(960.0f + (it2->child_name - 1) * (310.0f / 6)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
					}
					else
					{
						it1->yard_coords[0] = it1->center_x;
						it1->yard_coords[1] = -(960.0f + (it2->child_name - 1) * (310.0f / 10)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
					}
					//子堆场state坐标计算
					it1->yard_coords[3] = (float)it2->child_state_left;
					it1->yard_coords[4] = (float)it2->child_state_right;
					//子堆场type坐标计算
					if (it2->child_type == 0)
					{
						it1->yard_coords[5] = 0.0f;
					}
					else if (it2->child_type == 1)
					{
						if (it2->child_index < 7)
						{
							it1->yard_coords[5] = (float)it2->child_index;
						}
						else
						{
							it1->yard_coords[5] = 7.0f;
						}
					}
					else
					{
						it1->yard_coords[5] = 7.0f;
					}
					//低位报警机制
					if ((it2->child_state_left == 2 || it2->child_state_right == 2) && it2->child_cur_amount < LOW_STOP)
					{
						yardShader.setFloat("yard_warn", value);
					}
					else
					{
						yardShader.setFloat("yard_warn", 1.0f);
					}
					glBufferData(GL_ARRAY_BUFFER, sizeof(it1->yard_coords), &it1->yard_coords, GL_STATIC_DRAW);
					glDrawArrays(GL_POINTS, 0, 1);
				}
			}
		}
	}
}

void Yard::initGuiStyle()
{
	this->style = &ImGui::GetStyle();
}

void Yard::yard_dispatch(bool unreal)
{
	if (ImGui::CollapsingHeader(u8"堆场状态显示"))
	{
		ImGui::Indent();
		if (ImGui::CollapsingHeader(u8"上4号泊位堆场"))
		{
			ImGui::Indent();
			this->showYardInfo(1, unreal);
			ImGui::Unindent();
		}
		if (ImGui::CollapsingHeader(u8"上三角场"))
		{
			ImGui::Indent();
			this->showYardInfo(2, unreal);
			ImGui::Unindent();
		}
		if (ImGui::CollapsingHeader(u8"西门二场"))
		{
			ImGui::Indent();
			this->showYardInfo(3, unreal);
			ImGui::Unindent();
		}
		ImGui::Unindent();
	}
}

void Yard::add_type(std::string str_name, int type_type)
{
	int lst_idx = 1;		//原对应大类中小类序号最大者
	for (std::vector<Type_yard>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
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

bool Yard::start_stack(std::vector<std::string>& equipments, int yard, int child, int type, int index, float amount, float flux)
{
	//堆料设定
	bool ret = false;
	for (std::vector<Yard_father>::iterator it1 = this->yards.begin(); it1 != this->yards.end(); it1++)
	{
		//定位堆料堆场
		if (it1->yard_index == yard)
		{
			for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
			{
				//定位堆料子堆场
				if (it2->child_name == child)
				{
					if (it2->child_type != 0 && (it2->child_type != type || it2->child_index != index))
					{
						ret = false;
						std::cout << "堆料申请失败::货物类别冲突" << std::endl;
						break;
					}
					if (it2->child_cur_amount + amount > it2->child_max_amount)
					{
						//ret = false;
						std::cout << "堆料申请警告::申请堆放量超过承载上限" << std::endl;
						//break;
					}
					//定位斗轮机
					int pos_wheel = 0;
					std::vector<std::string>::const_iterator it_wheel;
					for (std::vector<std::string>::const_iterator it0 = equipments.begin(); it0 != equipments.end(); it0++)
					{
						if (*it0 == "SR10")
						{
							pos_wheel = 1;
						}
						else if (*it0 == "SR9")
						{
							pos_wheel = 3;
						}
						else if (*it0 == "SR8")
						{
							pos_wheel = 5;
						}
						else if (*it0 == "SR7")
						{
							pos_wheel = 7;
						}
						else if (*it0 == "SR6")
						{
							pos_wheel = 9;
						}
						else if (*it0 == "SR11" || *it0 == "S12" || *it0 == "SR12")
						{
							pos_wheel = -1;
							it_wheel = it0;
						}
					}
					//定位子堆场
					int pos_yard = 0;
					if (it1->yard_name == "F")
					{
						pos_yard = 0;
					}
					else if (it1->yard_name == "E")
					{
						pos_yard = 2;
					}
					else if (it1->yard_name == "D")
					{
						pos_yard = 4;
					}
					else if (it1->yard_name == "C")
					{
						pos_yard = 6;
					}
					else if (it1->yard_name == "B")
					{
						pos_yard = 8;
					}
					else if (it1->yard_name == "A")
					{
						pos_yard = 10;
					}
					else
					{
						pos_yard = -1;
					}
					ret = true;
					//斗轮机作业范围检查
					if (std::abs(pos_yard - pos_wheel) > 1)
					{
						ret = false;
						std::cout << "堆料申请失败::选中的堆场不在斗轮机作业范围内" << std::endl;
						break;
					}
					else if (pos_wheel == -1)
					{
						//独立斗轮机检查
						if (*it_wheel == "SR11" && (it1->yard_name != "G" && it1->yard_name != "H"))
						{
							ret = false;
							std::cout << "堆料申请失败::选中的堆场不在斗轮机SR11作业范围内" << std::endl;
							break;
						}
						else if (*it_wheel == "S12" && it1->yard_name != "K")
						{
							ret = false;
							std::cout << "堆料申请失败::选中的堆场不在斗轮机S12作业范围内" << std::endl;
							break;
						}
						else if (*it_wheel == "SR12" && it1->yard_name != "WEST2")
						{
							ret = false;
							std::cout << "堆料申请失败::选中的堆场不在斗轮机SR12作业范围内" << std::endl;
							break;
						}
					}
					//设定子堆场状态
					if (pos_yard != -1 && pos_wheel != -1)
					{
						//目标是上堆场
						if (pos_yard - pos_wheel > 0)
						{
							//斗轮机在堆场的左边
							it2->child_state_left = 1;
							it2->child_flux_left = flux;
						}
						else
						{
							//斗轮机在堆场的右边
							it2->child_state_right = 1;
							it2->child_flux_right = flux;
						}
					}
					else
					{
						//目标是其他堆场
						it2->child_state_left = 1;
						it2->child_state_right = 1;
						it2->child_flux_left = flux;
						it2->child_flux_right = flux;
					}
					it2->child_type = type;
					it2->child_index = index;
					break;
				}
			}
			break;
		}
	}
	//if (!ret)
	//{
	//	std::cout << "堆料申请失败" << std::endl;
	//}
	return ret;
}

bool Yard::start_reclaim(std::vector<std::string>& equipments, int yard, int child, int type, int index, float flux)
{
	//取料设定
	bool ret = false;
	for (std::vector<Yard_father>::iterator it1 = this->yards.begin(); it1 != this->yards.end(); it1++)
	{
		//定位堆料堆场
		if (it1->yard_index == yard)
		{
			for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
			{
				//定位堆料子堆场
				if (it2->child_name == child)
				{
					if (it2->child_type == 0 || it2->child_cur_amount < 10.0f)
					{
						ret = false;
						std::cout << "取料申请失败::当前堆场堆量过低或空" << std::endl;
						break;
					}
					if (it2->child_type != 0 && (it2->child_type != type || it2->child_index != index))
					{
						ret = false;
						std::cout << "取料申请失败::货物类别冲突" << std::endl;
						break;
					}
					//定位斗轮机
					int pos_wheel = 0;
					std::vector<std::string>::const_iterator it_wheel;
					for (std::vector<std::string>::const_iterator it0 = equipments.begin(); it0 != equipments.end(); it0++)
					{
						if (*it0 == "SR10")
						{
							pos_wheel = 1;
						}
						else if (*it0 == "SR9")
						{
							pos_wheel = 3;
						}
						else if (*it0 == "SR8")
						{
							pos_wheel = 5;
						}
						else if (*it0 == "SR7")
						{
							pos_wheel = 7;
						}
						else if (*it0 == "SR6")
						{
							pos_wheel = 9;
						}
						else if (*it0 == "SR11" || *it0 == "S12" || *it0 == "SR12")
						{
							pos_wheel = -1;
							it_wheel = it0;
						}
					}
					//定位子堆场
					int pos_yard = 0;
					if (it1->yard_name == "E")
					{
						pos_yard = 2;
					}
					else if (it1->yard_name == "D")
					{
						pos_yard = 4;
					}
					else if (it1->yard_name == "C")
					{
						pos_yard = 6;
					}
					else if (it1->yard_name == "B")
					{
						pos_yard = 8;
					}
					else
					{
						pos_yard = -1;
					}
					ret = true;
					//斗轮机作业范围检查
					if (std::abs(pos_yard - pos_wheel) > 1)
					{
						ret = false;
						std::cout << "堆料申请失败::选中的堆场不在斗轮机作业范围内" << std::endl;
						break;
					}
					else if (pos_wheel == -1)
					{
						//独立斗轮机检查
						if (*it_wheel == "SR11" && (it1->yard_name != "G" && it1->yard_name != "H"))
						{
							ret = false;
							std::cout << "堆料申请失败::选中的堆场不在斗轮机SR11作业范围内" << std::endl;
							break;
						}
						else if (*it_wheel == "S12" && it1->yard_name != "K")
						{
							ret = false;
							std::cout << "堆料申请失败::选中的堆场不在斗轮机S12作业范围内" << std::endl;
							break;
						}
						else if (*it_wheel == "SR12" && it1->yard_name != "WEST2")
						{
							ret = false;
							std::cout << "堆料申请失败::选中的堆场不在斗轮机SR12作业范围内" << std::endl;
							break;
						}
					}
					//设定子堆场状态
					if (pos_yard != -1 && pos_wheel != -1)
					{
						//目标是上堆场
						if (pos_yard - pos_wheel > 0)
						{
							//斗轮机在堆场的左边
							it2->child_state_left = 2;
							it2->child_flux_left = flux;
						}
						else
						{
							//斗轮机在堆场的右边
							it2->child_state_right = 2;
							it2->child_flux_right = flux;
						}
					}
					else
					{
						//目标是其他堆场
						it2->child_state_left = 2;
						it2->child_state_right = 2;
						it2->child_flux_left = flux;
						it2->child_flux_right = flux;
					}
					break;
				}
			}
			break;
		}
	}
	if (!ret)
	{
		std::cout << "取料申请失败" << std::endl;
	}
	return ret;
}

void Yard::yard_end(std::vector<std::string>& equipments)
{
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		if (*it1 == "SR12")
		{
			//终止西门二场
			std::vector<Yard_father>::iterator it2 = std::find_if(this->yards.begin(), this->yards.end(), FindYardWEST2());
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_left = 0;
				it3->child_state_right = 0;
			}
			break;
		}
		else if (*it1 == "S12")
		{
			//终止K场
			std::vector<Yard_father>::iterator it2 = std::find_if(this->yards.begin(), this->yards.end(), FindYardK());
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_left = 0;
				it3->child_state_right = 0;
			}
			break;
		}
		else if (*it1 == "SR11")
		{
			//终止G、H场
			std::vector<Yard_father>::iterator it2 = std::find_if(this->yards.begin(), this->yards.end(), FindYardG());
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_left = 0;
				it3->child_state_right = 0;
			}
			it2 = std::find_if(this->yards.begin(), this->yards.end(), FindYardH());
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_left = 0;
				it3->child_state_right = 0;
			}
			break;
		}
		else if (*it1 == "SR6")
		{
			//终止A场、B右场
			std::vector<Yard_father>::iterator it2 = this->yards.begin();
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_left = 0;
				it3->child_state_right = 0;
			}
			it2++;
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_right = 0;
			}
			break;
		}
		else if (*it1 == "SR7")
		{
			//终止C右场、B左场
			std::vector<Yard_father>::iterator it2 = ++this->yards.begin();
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_left = 0;
			}
			it2++;
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_right = 0;
			}
			break;
		}
		else if (*it1 == "SR8")
		{
			//终止D右场、C左场
			std::vector<Yard_father>::iterator it2 = ++this->yards.begin();
			it2++;
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_left = 0;
			}
			it2++;
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_right = 0;
			}
			break;
		}
		else if (*it1 == "SR9")
		{
			//终止E右场、D左场
			std::vector<Yard_father>::iterator it2 = ++this->yards.begin();
			it2++;
			it2++;
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_left = 0;
			}
			it2++;
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_right = 0;
			}
			break;
		}
		else if (*it1 == "SR10")
		{
			//终止F场、E左场
			std::vector<Yard_father>::iterator it2 = ++this->yards.begin();
			it2++;
			it2++;
			it2++;
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_left = 0;
			}
			it2++;
			for (std::vector<Yard_child>::iterator it3 = it2->children.begin(); it3 != it2->children.end(); it3++)
			{
				it3->child_state_left = 0;
				it3->child_state_right = 0;
			}
			break;
		}
	}
}

bool Yard::type_check(int type, int index)
{
	bool ret = false;
	for (std::vector<Type_yard>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
	{
		if (it->type_type == type && it->type_index == index)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

bool Yard::updateYards(float simurate)
{
	bool ret = false;
	for (std::vector<Yard_father>::iterator it1 = this->yards.begin(); it1 != this->yards.end(); it1++)
	{
		for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
		{
			if (it1->wheel_available.size() == 1)
			{
				//单斗轮机交互
				if (it2->child_state_left == 1)
				{
					//堆料
					it2->child_cur_amount += it2->child_flux_left * simurate;
					//堆满信号
					if (it2->child_cur_amount > it2->child_max_amount)
					{
						it2->child_cur_amount = it2->child_max_amount;
						it2->child_state_left = 0;
						it2->child_state_right = 0;
						ret = true;
						this->terminate_wheel = *it1->wheel_available.begin();
					}
				}
				else if (it2->child_state_left == 2)
				{
					//取料
					it2->child_cur_amount -= it2->child_flux_left * simurate;
					//取完信号
					if (it2->child_cur_amount < 0.0f)
					{
						it2->child_cur_amount = 0.0f;
						it2->child_state_left = 0;
						it2->child_state_right = 0;
						it2->child_type = 0;
						it2->child_index = 0;
						ret = true;
						this->terminate_wheel = *it1->wheel_available.begin();
					}
				}
			}
			else if(it1->wheel_available.size() == 2)
			{
				//双斗轮机交互
				if (it2->child_state_left == 1)
				{
					//左机堆料
					it2->child_cur_amount += it2->child_flux_left * simurate;
					//左机堆满信号
					if (it2->child_cur_amount > it2->child_max_amount)
					{
						it2->child_cur_amount = it2->child_max_amount;
						it2->child_state_left = 0;
						ret = true;
						this->terminate_wheel = *it1->wheel_available.begin();
					}
				}
				else if (it2->child_state_left == 2)
				{
					//左机取料
					it2->child_cur_amount -= it2->child_flux_left * simurate;
					//左机取完信号
					if (it2->child_cur_amount < 0.0f)
					{
						it2->child_cur_amount = 0.0f;
						it2->child_state_left = 0;
						it2->child_type = 0;
						it2->child_index = 0;
						ret = true;
						this->terminate_wheel = *it1->wheel_available.begin();
					}
				}
				if (it2->child_state_right == 1)
				{
					//右机堆料
					it2->child_cur_amount += it2->child_flux_right * simurate;
					//右机堆满信号
					if (it2->child_cur_amount > it2->child_max_amount)
					{
						it2->child_cur_amount = it2->child_max_amount;
						it2->child_state_right = 0;
						ret = true;
						this->terminate_wheel = *(++it1->wheel_available.begin());
					}
				}
				else if (it2->child_state_right == 2)
				{
					//右机取料
					it2->child_cur_amount -= it2->child_flux_right * simurate;
					//右机取完信号
					if (it2->child_cur_amount < 0.0f)
					{
						it2->child_cur_amount = 0.0f;
						it2->child_state_right = 0;
						it2->child_type = 0;
						it2->child_index = 0;
						ret = true;
						this->terminate_wheel = *(++it1->wheel_available.begin());
					}
				}
			}
		}
	}
	return ret;
}

void Yard::yard_choose()
{
	if (this->yard_choosing)
	{
		//上堆场结束和上三角场开始
		std::vector<Yard_father>::iterator it_G = std::find_if(this->yards.begin(), this->yards.end(), FindYardG());
		//西门二场
		std::vector<Yard_father>::iterator it_WEST = std::find_if(this->yards.begin(), this->yards.end(), FindYardWEST2());
		//其他
		std::vector<Yard_father>::iterator it_begin = this->yards.begin();
		std::vector<Yard_father>::iterator it_end = this->yards.end();
		ImGui::Begin(u8"选择堆场");
		if (ImGui::CollapsingHeader(u8"上4号泊位堆场"))
		{
			ImGui::Indent();
			this->chooseYardInfo(it_begin, it_G, 1);
			ImGui::Unindent();
		}
		if (ImGui::CollapsingHeader(u8"上三角场"))
		{
			ImGui::Indent();
			this->chooseYardInfo(it_G, it_WEST, 2);
			ImGui::Unindent();
		}
		if (ImGui::CollapsingHeader(u8"西门二场"))
		{
			ImGui::Indent();
			this->chooseYardInfo(it_WEST, it_end, 3);
			ImGui::Unindent();
		}
		this->pre_button(1);
		if (ImGui::Button(u8"选择"))
		{
			if (this->yard_choosed != -1 && this->child_choosed != -1)
			{
				this->yard_choosing = false;
				this->data_send = true;
				//所有堆场及子堆场失去焦点，复位为原选中堆场
				for (std::vector<Yard_father>::iterator it1 = this->yards.begin(); it1 != this->yards.end(); it1++)
				{
					it1->yard_focus = true;
					if (this->flow_num_choose == 1)
					{
						if (it1->yard_index == this->flow_yard_choosed_1)
						{
							it1->yard_focus = false;
						}
					}
					else if (this->flow_num_choose == 2)
					{
						if (it1->yard_index == this->flow_yard_choosed_1 || it1->yard_index == this->flow_yard_choosed_2)
						{
							it1->yard_focus = false;
						}
					}
					for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
					{
						it2->child_focus = false;
					}
				}
				this->flow_num_choose = 0;
			}
			else
			{
				std::cout << "无法选中堆场" << std::endl;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"关闭窗口"))
		{
			this->yard_choosing = false;
			this->data_send = false;
			this->yard_choosed = -1;
			this->child_choosed = -1;
			this->type_choosed = -1;
			this->index_choosed = -1;
			this->amount_choosed = 0.0f;
			//所有堆场及子堆场失去焦点，复位为原选中堆场
			for (std::vector<Yard_father>::iterator it1 = this->yards.begin(); it1 != this->yards.end(); it1++)
			{
				it1->yard_focus = true;
				if (this->flow_num_choose == 1)
				{
					if (it1->yard_index == this->flow_yard_choosed_1)
					{
						it1->yard_focus = false;
					}
				}
				else if (this->flow_num_choose == 2)
				{
					if (it1->yard_index == this->flow_yard_choosed_1 || it1->yard_index == this->flow_yard_choosed_2)
					{
						it1->yard_focus = false;
					}
				}
				for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
				{
					it2->child_focus = false;
				}
			}
			//this->flow_num_choose = 0;
		}
		this->post_button();
		ImGui::End();
	}
}

void Yard::set_focus(std::vector<std::string>& equipments)
{
	for (std::vector<Yard_father>::iterator it1 = this->yards.begin(); it1 != this->yards.end(); it1++)
	{
		it1->yard_focus = true;
		for (std::vector<std::string>::const_iterator it2 = equipments.begin(); it2 != equipments.end(); it2++)
		{
			bool ret = false;
			for (std::vector<std::string>::const_iterator it3 = it1->wheel_available.begin(); it3 != it1->wheel_available.end(); it3++)
			{
				if (*it3 == *it2)
				{
					ret = true;
					break;
				}
			}
			if (ret)
			{
				it1->yard_focus = false;
				if (this->flow_num_choose == 0)
				{
					this->flow_yard_choosed_1 = it1->yard_index;
					this->flow_num_choose++;
				}
				else if (this->flow_num_choose == 1)
				{
					this->flow_yard_choosed_2 = it1->yard_index;
					this->flow_num_choose++;
				}
				break;
			}
		}
		//子堆场的焦点情况仅在选择子堆场的情况下改变，而在流程选择情况下不能改变
		/*
		if (it1->yard_focus)
		{
			for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
			{
				it2->child_focus = false;
			}
		}
		*/
	}
}

void Yard::lose_focus()
{
	this->flow_num_choose = 0;
	for (std::vector<Yard_father>::iterator it1 = this->yards.begin(); it1 != this->yards.end(); it1++)
	{
		it1->yard_focus = false;
		for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
		{
			it2->child_focus = false;
		}
	}
}

void Yard::send_reset()
{
	this->data_send = false;
	this->yard_choosed = -1;
	this->child_choosed = -1;
	this->type_choosed = -1;
	this->index_choosed = -1;
	this->amount_choosed = 0.0f;
	this->flow_num_choose = 0;
}

void Yard::showYardInfo(int yard_area, bool unreal)
{
	std::string temp;
	std::vector<Yard_father>::iterator it_begin;
	std::vector<Yard_father>::iterator it_end;
	switch (yard_area)
	{
	case 1:
		it_begin = this->yards.begin();
		it_end = std::find_if(this->yards.begin(), this->yards.end(), FindYardG());
		break;
	case 2:
		it_begin = std::find_if(this->yards.begin(), this->yards.end(), FindYardG());
		it_end = std::find_if(this->yards.begin(), this->yards.end(), FindYardWEST2());
		break;
	case 3:
		it_begin = std::find_if(this->yards.begin(), this->yards.end(), FindYardWEST2());
		it_end = this->yards.end();
		break;
	default:
		break;
	}
	static int type = 1;				//大类
	static int index = 1;				//小类
	static float cur_amount = 0.0f;	//强制设定堆放量
	static int yard_select = 0;			//选中堆场
	static int child_select = 0;		//选中子堆场
	for (std::vector<Yard_father>::iterator it1 = it_begin; it1 != it_end; it1++)
	{
		for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
		{
			temp = std::to_string(it2->child_name);
			ImGui::Text((it1->yard_name + " " + temp + u8" 场：").c_str());
			ImGui::SameLine();
			temp = std::to_string((int)it2->child_cur_amount);
			ImGui::Text((u8"当前堆货量" + temp + u8"吨").c_str());
			//显示货物名称
			if (it2->child_type == 0)
			{
				ImGui::Text(u8"当前无货物");
			}
			else
			{
				for (std::vector<Type_yard>::const_iterator it3 = this->names.begin(); it3 != this->names.end(); it3++)
				{
					if (it3->type_index == it2->child_index && it3->type_type == it2->child_type)
					{
						ImGui::Text((u8"当前货物名：" + it3->type_name).c_str());
						break;
					}
				}
			}
			//显示斗轮机交互情况
			if (it1->wheel_available.size() == 1)
			{
				//单斗轮机交互
				if (it2->child_state_left == 0)
				{
					ImGui::Text(u8"当前无作业");
				}
				else if (it2->child_state_left == 1)
				{
					temp = std::to_string((int)it2->child_flux_left);
					ImGui::Text((u8"当前正在堆料，流量：" + temp).c_str());

				}
				else if (it2->child_state_left == 2)
				{
					temp = std::to_string((int)it2->child_flux_left);
					ImGui::Text((u8"当前正在取料，流量：" + temp).c_str());
				}
			}
			else if (it1->wheel_available.size() == 2)
			{
				//左斗轮机交互
				if (it2->child_state_left == 0)
				{
					ImGui::Text(u8"左斗轮机无作业");
				}
				else if (it2->child_state_left == 1)
				{
					temp = std::to_string((int)it2->child_flux_left);
					ImGui::Text((u8"左斗轮机正在堆料，流量：" + temp).c_str());

				}
				else if (it2->child_state_left == 2)
				{
					temp = std::to_string((int)it2->child_flux_left);
					ImGui::Text((u8"左斗轮机正在取料，流量：" + temp).c_str());
				}
				//右斗轮机交互
				if (it2->child_state_right == 0)
				{
					ImGui::Text(u8"右斗轮机无作业");
				}
				else if (it2->child_state_right == 1)
				{
					temp = std::to_string((int)it2->child_flux_right);
					ImGui::Text((u8"右斗轮机正在堆料，流量：" + temp).c_str());

				}
				else if (it2->child_state_right == 2)
				{
					temp = std::to_string((int)it2->child_flux_right);
					ImGui::Text((u8"右斗轮机正在取料，流量：" + temp).c_str());
				}
			}
			//子堆场选定按钮
			if (unreal)
			{
				this->pre_button(0);
				if (ImGui::Button((u8"强制指定" + it1->yard_name + " " + std::to_string(it2->child_name) + u8"场属性").c_str()))
				{
					if (yard_select == it1->yard_index && child_select == it2->child_name)
					{
						yard_select = 0;
						child_select = 0;
					}
					else
					{
						yard_select = it1->yard_index;
						child_select = it2->child_name;
					}
				}
				this->post_button();
			}
			//强制指定堆场属性
			if (it1->yard_index == yard_select && it2->child_name == child_select && unreal)
			{
				ImGui::Text((it1->yard_name + " " + std::to_string(it2->child_name) + u8" 场").c_str());
				//顶级权限——设置货物
				ImGui::Text(u8"堆场货物大类：");
				ImGui::SameLine();
				ImGui::RadioButton(u8"大类散货", &type, 1);
				ImGui::SameLine();
				ImGui::RadioButton(u8"来源散货", &type, 2);
				ImGui::SameLine();
				ImGui::RadioButton(u8"企业散货", &type, 3);
				ImGui::Text(u8"选择货物小类：");
				int temp_1 = 1;
				for (std::vector<Type_yard>::const_iterator it3 = this->names.begin(); it3 != this->names.end(); it3++)
				{
					if (it3->type_type == type)
					{
						ImGui::SameLine();
						ImGui::RadioButton(it3->type_name.c_str(), &index, temp_1);
						temp_1++;
					}
				}
				ImGui::SliderFloat(u8"设置当前存量", &cur_amount, 0.0f, it2->child_max_amount);
				this->pre_button(false);
				if (ImGui::Button(u8"设定"))
				{
					it2->child_index = index;
					it2->child_type = type;
					it2->child_cur_amount = cur_amount;
				}
				this->post_button();
				break;
			}
		}
		ImGui::Separator();
	}
}

void Yard::chooseYardInfo(std::vector<Yard_father>::iterator& it_begin, std::vector<Yard_father>::iterator& it_end, int schoose)
{
	std::string temp;
	std::vector<Yard_child>::iterator it_child;
	for (std::vector<Yard_father>::iterator it1 = it_begin; it1 != it_end; it1++)
	{
		//ImGui::Text((it1->yard_name + u8" 场：").c_str());
		for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
		{
			temp = std::to_string(it2->child_name);
			this->pre_button(this->yard_choosed == it1->yard_index && this->child_choosed == it2->child_name);
			if (ImGui::Button((it1->yard_name + " " + temp + u8" 场").c_str()))
			{
				if (this->yard_choosed == it1->yard_index && this->child_choosed == it2->child_name)
				{
					//已选择，取消选择
					this->yard_choosed = -1;
					this->child_choosed = -1;
					this->type_choosed = -1;
					this->index_choosed = -1;
					this->amount_choosed = 0.0f;
					//堆场及子堆场失去焦点，焦点返回原先选中的斗轮机的周边堆场
					for (std::vector<Yard_father>::iterator it3 = this->yards.begin(); it3 != this->yards.end(); it3++)
					{
						it3->yard_focus = true;
						if (this->flow_num_choose == 1)
						{
							if (it3->yard_index == this->flow_yard_choosed_1)
							{
								it3->yard_focus = false;
							}
						}
						else if (this->flow_num_choose == 2)
						{
							if (it3->yard_index == this->flow_yard_choosed_1 || it3->yard_index == this->flow_yard_choosed_2)
							{
								it3->yard_focus = false;
							}
						}
						for (std::vector<Yard_child>::iterator it4 = it3->children.begin(); it4 != it3->children.end(); it4++)
						{
							it4->child_focus = false;
						}
					}
				}
				else
				{
					//未选择，设定
					this->yard_choosed = it1->yard_index;
					this->child_choosed = it2->child_name;
					this->type_choosed = it2->child_type;
					this->index_choosed = it2->child_index;
					this->amount_choosed = it2->child_cur_amount;
					//设定焦点
					for (std::vector<Yard_father>::iterator it3 = this->yards.begin(); it3 != this->yards.end(); it3++)
					{
						it3->yard_focus = it3->yard_index != it1->yard_index;
						for (std::vector<Yard_child>::iterator it4 = it3->children.begin(); it4 != it3->children.end(); it4++)
						{
							it4->child_focus = it4->child_name != it2->child_name;
						}
					}
				}
			}
			this->post_button();
			ImGui::SameLine();
			if (this->yard_choosed == it1->yard_index && this->child_choosed == it2->child_name)
			{
				//被选中
				it_child = it2;
			}
		}
		if (this->yard_choosed == it1->yard_index)
		{
			//显示选中的堆场信息
			ImGui::Text("");
			temp = std::to_string((int)it_child->child_name);
			ImGui::Text((it1->yard_name + " " + temp + u8" 场：").c_str());
			ImGui::SameLine();
			temp = std::to_string((int)it_child->child_cur_amount);
			ImGui::Text((u8"当前堆货量" + temp + u8"吨").c_str());
			//显示货物名称
			if (it_child->child_type == 0)
			{
				ImGui::Text(u8"当前无货物");
			}
			else
			{
				for (std::vector<Type_yard>::const_iterator it3 = this->names.begin(); it3 != this->names.end(); it3++)
				{
					if (it3->type_index == it_child->child_index && it3->type_type == it_child->child_type)
					{
						ImGui::Text((u8"当前货物名：" + it3->type_name).c_str());
						break;
					}
				}
			}
			//显示斗轮机交互情况
			if (it1->wheel_available.size() == 1)
			{
				//单斗轮机交互
				if (it_child->child_state_left == 0)
				{
					ImGui::Text(u8"当前无作业");
				}
				else if (it_child->child_state_left == 1)
				{
					temp = std::to_string((int)it_child->child_flux_left);
					ImGui::Text((u8"当前正在堆料，流量：" + temp).c_str());

				}
				else if (it_child->child_state_left == 2)
				{
					temp = std::to_string((int)it_child->child_flux_left);
					ImGui::Text((u8"当前正在取料，流量：" + temp).c_str());
				}
			}
			else if (it1->wheel_available.size() == 2)
			{
				//左斗轮机交互
				if (it_child->child_state_left == 0)
				{
					ImGui::Text(u8"左斗轮机无作业");
				}
				else if (it_child->child_state_left == 1)
				{
					temp = std::to_string((int)it_child->child_flux_left);
					ImGui::Text((u8"左斗轮机正在堆料，流量：" + temp).c_str());

				}
				else if (it_child->child_state_left == 2)
				{
					temp = std::to_string((int)it_child->child_flux_left);
					ImGui::Text((u8"左斗轮机正在取料，流量：" + temp).c_str());
				}
				//右斗轮机交互
				if (it_child->child_state_right == 0)
				{
					ImGui::Text(u8"右斗轮机无作业");
				}
				else if (it_child->child_state_right == 1)
				{
					temp = std::to_string((int)it_child->child_flux_right);
					ImGui::Text((u8"右斗轮机正在堆料，流量：" + temp).c_str());

				}
				else if (it_child->child_state_right == 2)
				{
					temp = std::to_string((int)it_child->child_flux_right);
					ImGui::Text((u8"右斗轮机正在取料，流量：" + temp).c_str());
				}
			}
		}
		ImGui::Separator();
	}
}

void Yard::pre_button(bool choosed)
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

void Yard::post_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}
