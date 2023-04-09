#include"Silo.h"

#define CT_WIDTH 4100
#define CT_HEIGHT 2370
#define SCR_WIDTH 1440
#define SCR_HEIGHT 810

#define FLUX 0.0022f						//流程通用流量
#define LAYER_SILO -0.001f
#define LOW_START 10.0f					//最低流程开始存量
#define LOW_APPLY 5.0f						//最低申请作业量

#define PROB_SILO 0.5f						//随机初始化筒仓内有货的概率
#define PROB_HUA 0.5f						//随机初始化筒仓内货物为化工煤的概率
#define PROB_ACT 0.3f						//随机初始化筒仓端口正在工作的概率

Silo::Silo()
{
	this->data_send = false;
	this->selected_index = 0;
	this->select_window = false;
	this->amount_apply = 0.0f;
	this->out_total_0 = 0;
	this->out_total_1 = 0;
	this->straight_state = 0;
	this->straight_flux = 0.0f;
	this->storage_targets = 0.0f;
	this->storage_process = 0.0f;
	this->silo_unit.max_storage = 1000.0f;
	this->silo_unit.cur_storage = 0.0f;
	this->silo_unit.silo_type = false;
	this->silo_unit.silo_flux = FLUX;
	this->silo_unit.silo_focus = false;
	this->silo_unit.silo_state_up = 0;
	this->silo_unit.silo_state_down = 0;
	this->silo_unit.silo_coords[0] = 240.0f / (CT_WIDTH / 10.0f) - 5.0f;
	this->silo_unit.silo_coords[2] = LAYER_SILO;
	this->silo_unit.silo_coords[3] = 0.0f;
	this->silo_unit.silo_coords[4] = 0.0f;
	this->silo_unit.silo_coords[5] = 0.0f;
	this->silo_unit.silo_coords[6] = 0.0f;
	this->silo_unit.silo_coords[7] = 0.0f;
	for (int i = 0; i < 14; i++)
	{
		this->silo_unit.silo_idx = i + 1;
		this->silo_unit.silo_coords[1] = -(1100.0f + i * 80.0f) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
		this->silos.push_back(this->silo_unit);
	}
	/*buffers*/
	glGenVertexArrays(1, &siloVAO);
	glGenBuffers(1, &siloVBO);
	glBindVertexArray(siloVAO);
	glBindBuffer(GL_ARRAY_BUFFER, siloVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->silo_unit.silo_coords), &this->silo_unit.silo_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void Silo::reset(SimuCore& core, bool rand_init)
{
	//重置筒仓
	for (std::vector<Silo_unit>::iterator it1 = this->silos.begin(); it1 != this->silos.end(); it1++)
	{
		if (rand_init && core.dice(PROB_SILO))
		{
			//随机初始化
			if (core.dice(PROB_HUA))
			{
				//化工煤
				it1->silo_type = true;
				it1->silo_coords[6] = 1.0f;
			}
			else
			{
				//大混
				it1->silo_type = false;
				it1->silo_coords[6] = 0.0f;
			}
			it1->cur_storage = core.genRandomFloat(0.0f, it1->max_storage);
			it1->silo_coords[5] = it1->cur_storage / it1->max_storage;
		}
		else
		{
			//零状态初始化
			it1->silo_type = false;
			it1->cur_storage = 0.0f;
			it1->silo_coords[5] = 0.0f;
			it1->silo_coords[6] = 0.0f;
		}
		it1->silo_flux = FLUX;
		it1->silo_state_up = 0;
		it1->silo_state_down = 0;
		it1->silo_coords[3] = 0.0f;
		it1->silo_coords[4] = 0.0f;
	}
	//重置直出
	this->amount_apply = 0.0f;
	this->out_total_0 = 0;
	this->out_total_1 = 0;
	this->straight_state = 0;
	this->straight_flux = 0.0f;
	this->storage_targets = 0.0f;
	this->storage_process = 0.0f;
}

void Silo::silo_random_initiator(SimuCore& core)
{
	//初始与筒仓相连的皮带都不工作，所以筒仓初始态不会有互动
	for (std::vector<Silo_unit>::iterator it1 = this->silos.begin(); it1 != this->silos.end(); it1++)
	{
		if (core.dice(PROB_SILO))
		{
			if (core.dice(PROB_HUA))
			{
				//化工煤
				it1->silo_type = true;
				it1->silo_coords[6] = 1.0f;
			}
			else
			{
				//大混
				it1->silo_type = false;
				it1->silo_coords[6] = 0.0f;
			}
			it1->cur_storage = core.genRandomFloat(0.0f, it1->max_storage);
			it1->silo_coords[5] = it1->cur_storage / it1->max_storage;
		}
	}
}

void Silo::drawSilo(Camera& camera, Shader& siloShader)
{
	siloShader.use();
	this->model = glm::mat4(1.0f);
	siloShader.setMat4("model", this->model);
	this->projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
	siloShader.setMat4("projection", this->projection);
	this->view = glm::mat4(1.0f);
	this->view = camera.GetViewMatrix();
	siloShader.setMat4("view", this->view);
	glBindVertexArray(siloVAO);
	glBindBuffer(GL_ARRAY_BUFFER, siloVBO);
	for (std::vector<Silo_unit>::iterator it1 = this->silos.begin(); it1 != this->silos.end(); it1++)
	{
		if (!it1->silo_focus)
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(it1->silo_coords), &it1->silo_coords, GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, 1);
		}
	}
}

void Silo::initGuiStyle()
{
	this->style = &ImGui::GetStyle();
}

void Silo::silo_dispatch(bool unreal)
{
	std::string temp;
	static int silo_select = 0;				//选中筒仓
	static bool coal_type = false;			//煤炭类别
	static float silo_cur_amount = 0.0f;	//强制设定堆放量
	if (ImGui::CollapsingHeader(u8"筒仓状态显示"))
	{
		ImGui::Indent();
		for (std::vector<Silo_unit>::iterator it1 = this->silos.begin(); it1 != this->silos.end(); it1++)
		{
			temp = std::to_string(it1->silo_idx);
			ImGui::Text((temp + u8" 号筒仓：").c_str());
			//显示货物名称
			if (it1->cur_storage > 0)
			{
				ImGui::SameLine();
				temp = std::to_string((int)it1->cur_storage);
				ImGui::Text((u8"当前堆货量" + temp + u8"吨").c_str());
				if (it1->silo_type)
				{
					ImGui::Text(u8"煤类别：化工煤");
				}
				else
				{
					ImGui::Text(u8"煤类别：大混");
				}
			}
			else
			{
				ImGui::Text(u8"当前无货物");
			}
			//筒仓选定按钮
			if (unreal)
			{
				this->pre_button(0);
				if (ImGui::Button((u8"强制指定" + std::to_string(it1->silo_idx) + u8"筒仓属性").c_str()))
				{
					if (silo_select == it1->silo_idx)
					{
						silo_select = 0;
					}
					else
					{
						silo_select = it1->silo_idx;
					}
				}
				this->post_button();
			}
			//强制指定堆场属性
			if (it1->silo_idx == silo_select)
			{
				ImGui::Text((std::to_string(it1->silo_idx) + u8" 仓").c_str());
				//顶级权限——设置货物
				if (coal_type)
				{
					ImGui::Checkbox(u8"煤炭类别：化工煤", &coal_type);
				}
				else
				{
					ImGui::Checkbox(u8"煤炭类别：大混", &coal_type);
				}
				ImGui::SliderFloat(u8"设置当前存量", &silo_cur_amount, 0.0f, 1000.0f);
				this->pre_button(false);
				if (ImGui::Button(u8"设定"))
				{
					it1->silo_type = coal_type;
					it1->cur_storage = silo_cur_amount;
					it1->silo_coords[6] = (float)coal_type;
					it1->silo_coords[5] = (float)it1->cur_storage / (float)it1->max_storage;
				}
				this->post_button();
			}
		}
		ImGui::Unindent();
	}
}

void Silo::start_straight(float flux, bool type)
{
	if (type)
	{
		//化工煤直出
		this->straight_state = 2;
		this->straight_flux = flux;
	}
	else
	{
		//大混直出
		this->straight_state = 1;
		this->straight_flux = flux;
	}
}

bool Silo::start_stack(int silo_index, bool type, float amount, float flux)
{
	bool ret = false;
	if (silo_index < 1 || silo_index > 14)
	{
		std::cout << "筒仓申请失败::筒仓序号无效" << std::endl;
		return false;
	}
	for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
	{
		if (it->silo_idx == silo_index)
		{
			if (it->silo_type != type && it->cur_storage > 0.0f)
			{
				std::cout << "筒仓申请失败::货物类别冲突" << std::endl;
				ret = false;
				break;
			}
			if (it->cur_storage + amount > it->max_storage)
			{
				std::cout << "筒仓申请失败::申请量超过承载上限" << std::endl;
				ret = false;
				break;
			}
			ret = true;
			it->silo_state_up = 1;
			it->silo_coords[3] = 1.0f;
			it->silo_type = type;
			it->silo_flux = flux;
			//目标堆货量设置
			this->storage_process = 0.0f;
			this->storage_targets = amount;
		}
	}
	return ret;
}

bool Silo::updateSilos(float simurate)
{
	bool ret = false;
	//筒仓更新
	for (std::vector<Silo_unit>::iterator it1 = this->silos.begin(); it1 != this->silos.end(); it1++)
	{
		if (it1->silo_state_up == 1)
		{
			it1->cur_storage += it1->silo_flux * simurate;
			it1->silo_coords[5] = (float)it1->cur_storage / (float)it1->max_storage;
			if (it1->cur_storage > it1->max_storage)
			{
				//堆满停机
				it1->cur_storage = it1->max_storage;
				it1->silo_state_up = 0;
				it1->silo_coords[3] = 0.0f;
				ret = true;
			}
			if (this->storage_process > this->storage_targets)
			{
				//达标停机
				it1->silo_state_up = 0;
				it1->silo_coords[3] = 0.0f;
				ret = true;
			}
		}
		else if (it1->silo_state_down == 1)
		{
			it1->cur_storage -= it1->silo_flux * simurate;
			it1->silo_coords[5] = (float)it1->cur_storage / (float)it1->max_storage;
			if (it1->silo_type)
			{
				this->out_total_1 += (int)(it1->silo_flux * simurate);
			}
			else
			{
				this->out_total_0 += (int)(it1->silo_flux * simurate);
			}
			//卸空停机
			if (it1->cur_storage < 0.0f)
			{
				it1->cur_storage = 0.0f;
				it1->silo_state_down = 0;
				it1->silo_coords[4] = 0.0f;
				ret = true;
			}
		}
	}
	return ret;
}

void Silo::updateStraight(float simurate)
{
	if (this->straight_state = 1)
	{
		this->out_total_0 += (int)(this->straight_flux * simurate);
	}
	else if (this->straight_state = 2)
	{
		this->out_total_1 += (int)(this->straight_flux * simurate);
	}
}

void Silo::silo_end_out()
{
	for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
	{
		if (it->silo_state_down == 1)
		{
			it->silo_state_down = 0;
			it->silo_coords[4] = 0.0f;
		}
	}
}

bool Silo::out_check()
{
	bool ret = false;
	for (std::vector<Silo_unit>::const_iterator it = this->silos.begin(); it != this->silos.end(); it++)
	{
		if (it->silo_state_down == 1)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

void Silo::set_focus(std::vector<std::string>& equipments)
{
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		if (*it1 == "ZW12")
		{
			for (std::vector<Silo_unit>::iterator it2 = this->silos.begin(); it2 != this->silos.end(); it2++)
			{
				it2->silo_focus = false;
				it2->silo_coords[7] = 0.0f;
			}
			break;
		}
		//全程没找到ZW12筒仓段，就全部设置失焦
		for (std::vector<Silo_unit>::iterator it2 = this->silos.begin(); it2 != this->silos.end(); it2++)
		{
			it2->silo_focus = true;
			it2->silo_coords[7] = 1.0f;
		}
	}
}

void Silo::lose_focus()
{
	for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
	{
		it->silo_focus = false;
		it->silo_coords[7] = 0.0f;
	}
}

void Silo::show_select()
{
	static int selected = 0;
	static float apply_amount = 0.0f;
	if (this->select_window)
	{
		ImGui::Begin(u8"筒仓选择");
		static std::vector<Silo_unit>::iterator it_selected;
		for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
		{
			this->pre_button(selected == it->silo_idx);
			if (ImGui::Button((std::to_string(it->silo_idx) + u8"号筒仓").c_str()))
			{
				if (selected != it->silo_idx)
				{
					selected = it->silo_idx;
					it_selected = it;
				}
				else
				{
					selected = 0;
				}
			}
			this->post_button();
			if (it->silo_idx < 14)
			{
				ImGui::SameLine();
			}
		}
		if (selected)
		{
			ImGui::Text((u8"选中" + std::to_string(it_selected->silo_idx) + u8"号筒仓：").c_str());
			//显示货物名称
			if (it_selected->cur_storage > 0)
			{
				ImGui::Text((u8"当前堆货量" + std::to_string((int)it_selected->cur_storage) + u8"吨").c_str());
				if (it_selected->silo_type)
				{
					ImGui::Text(u8"煤类别：化工煤");
				}
				else
				{
					ImGui::Text(u8"煤类别：大混");
				}
			}
			else
			{
				ImGui::Text(u8"当前无货物");
			}
			ImGui::Text(u8"请输入申请作业量：");
			ImGui::SameLine();
			ImGui::SliderFloat(" ", &apply_amount, 0.0f, it_selected->max_storage - it_selected->cur_storage);
		}
		ImGui::Text(" ");
		this->pre_button(1);
		if (ImGui::Button(u8"确认"))
		{
			if (selected == 0)
			{
				std::cout << "筒仓选择失败::未选择筒仓" << std::endl;
				this->selected_index = 0;
				this->select_window = false;
			}
			else if (apply_amount < LOW_APPLY)
			{
				std::cout << "筒仓选择失败::申请作业量过低" << std::endl;
				this->selected_index = 0;
				this->select_window = false;
			}
			else
			{
				this->selected_index = selected;
				this->amount_apply = apply_amount;
				this->data_send = true;
				this->select_window = false;
			}
		}
		ImGui::SameLine();
		this->pre_button(0);
		if (ImGui::Button(u8"取消"))
		{
			this->data_send = false;
			this->selected_index = 0;
			this->select_window = false;
		}
		this->post_button();
		ImGui::End();
	}
}

void Silo::start_silo_out(float flux, bool type)
{
	if (type)
	{
		//化工煤筒仓出
		for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
		{
			//可以一边装煤一边卸煤
			if (it->silo_type && it->cur_storage > LOW_START)
			{
				it->silo_flux = flux;
				it->silo_state_down = 1;
				it->silo_coords[4] = 1.0f;
				break;
			}
		}
	}
	else
	{
		//大混筒仓出
		for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
		{
			if (!it->silo_type && it->cur_storage > LOW_START)
			{
				it->silo_flux = flux;
				it->silo_state_down = 1;
				it->silo_coords[4] = 1.0f;
				break;
			}
		}
	}
}

bool Silo::check_silo_out(bool type)
{
	bool ret = false;
	if (type)
	{
		//化工煤
		for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
		{
			if (it->silo_state_down == 1)
			{
				return false;
			}
			if (it->silo_type == true && it->cur_storage > LOW_APPLY)
			{
				ret = true;
				break;
			}
		}
	}
	else
	{
		//大混
		for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
		{
			if (it->silo_state_down == 1)
			{
				return false;
			}
			if (it->silo_type == false && it->cur_storage > LOW_APPLY)
			{
				ret = true;
				break;
			}
		}
	}
	return ret;
}

void Silo::end_silo(std::vector<std::string>& equipments)
{
	std::vector<std::string>::reverse_iterator it1 = equipments.rbegin();
	if (*it1 == "炼化")
	{
		//终止直出
		this->straight_state = 0;
		this->straight_flux = 0.0f;
	}
	else if (*it1 == "ZW12")
	{
		//终止筒仓堆料
		this->storage_targets = 0.0f;
		this->storage_process = 0.0f;
		for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
		{
			if (it->silo_state_up == 1)
			{
				it->silo_state_up = 0;
				it->silo_flux = 0.0f;
				it->silo_coords[3] = 0.0f;
			}
		}
	}
}

void Silo::send_reset()
{
	this->data_send = false;
	this->selected_index = 0;
}

void Silo::pre_button(bool choosed)
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

void Silo::post_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}
