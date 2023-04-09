#include"Silo.h"

#define CT_WIDTH 4100
#define CT_HEIGHT 2370
#define SCR_WIDTH 1440
#define SCR_HEIGHT 810

#define FLUX 0.0022f						//����ͨ������
#define LAYER_SILO -0.001f
#define LOW_START 10.0f					//������̿�ʼ����
#define LOW_APPLY 5.0f						//���������ҵ��

#define PROB_SILO 0.5f						//�����ʼ��Ͳ�����л��ĸ���
#define PROB_HUA 0.5f						//�����ʼ��Ͳ���ڻ���Ϊ����ú�ĸ���
#define PROB_ACT 0.3f						//�����ʼ��Ͳ�ֶ˿����ڹ����ĸ���

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
	//����Ͳ��
	for (std::vector<Silo_unit>::iterator it1 = this->silos.begin(); it1 != this->silos.end(); it1++)
	{
		if (rand_init && core.dice(PROB_SILO))
		{
			//�����ʼ��
			if (core.dice(PROB_HUA))
			{
				//����ú
				it1->silo_type = true;
				it1->silo_coords[6] = 1.0f;
			}
			else
			{
				//���
				it1->silo_type = false;
				it1->silo_coords[6] = 0.0f;
			}
			it1->cur_storage = core.genRandomFloat(0.0f, it1->max_storage);
			it1->silo_coords[5] = it1->cur_storage / it1->max_storage;
		}
		else
		{
			//��״̬��ʼ��
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
	//����ֱ��
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
	//��ʼ��Ͳ��������Ƥ����������������Ͳ�ֳ�ʼ̬�����л���
	for (std::vector<Silo_unit>::iterator it1 = this->silos.begin(); it1 != this->silos.end(); it1++)
	{
		if (core.dice(PROB_SILO))
		{
			if (core.dice(PROB_HUA))
			{
				//����ú
				it1->silo_type = true;
				it1->silo_coords[6] = 1.0f;
			}
			else
			{
				//���
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
	static int silo_select = 0;				//ѡ��Ͳ��
	static bool coal_type = false;			//ú̿���
	static float silo_cur_amount = 0.0f;	//ǿ���趨�ѷ���
	if (ImGui::CollapsingHeader(u8"Ͳ��״̬��ʾ"))
	{
		ImGui::Indent();
		for (std::vector<Silo_unit>::iterator it1 = this->silos.begin(); it1 != this->silos.end(); it1++)
		{
			temp = std::to_string(it1->silo_idx);
			ImGui::Text((temp + u8" ��Ͳ�֣�").c_str());
			//��ʾ��������
			if (it1->cur_storage > 0)
			{
				ImGui::SameLine();
				temp = std::to_string((int)it1->cur_storage);
				ImGui::Text((u8"��ǰ�ѻ���" + temp + u8"��").c_str());
				if (it1->silo_type)
				{
					ImGui::Text(u8"ú��𣺻���ú");
				}
				else
				{
					ImGui::Text(u8"ú��𣺴��");
				}
			}
			else
			{
				ImGui::Text(u8"��ǰ�޻���");
			}
			//Ͳ��ѡ����ť
			if (unreal)
			{
				this->pre_button(0);
				if (ImGui::Button((u8"ǿ��ָ��" + std::to_string(it1->silo_idx) + u8"Ͳ������").c_str()))
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
			//ǿ��ָ���ѳ�����
			if (it1->silo_idx == silo_select)
			{
				ImGui::Text((std::to_string(it1->silo_idx) + u8" ��").c_str());
				//����Ȩ�ޡ������û���
				if (coal_type)
				{
					ImGui::Checkbox(u8"ú̿��𣺻���ú", &coal_type);
				}
				else
				{
					ImGui::Checkbox(u8"ú̿��𣺴��", &coal_type);
				}
				ImGui::SliderFloat(u8"���õ�ǰ����", &silo_cur_amount, 0.0f, 1000.0f);
				this->pre_button(false);
				if (ImGui::Button(u8"�趨"))
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
		//����úֱ��
		this->straight_state = 2;
		this->straight_flux = flux;
	}
	else
	{
		//���ֱ��
		this->straight_state = 1;
		this->straight_flux = flux;
	}
}

bool Silo::start_stack(int silo_index, bool type, float amount, float flux)
{
	bool ret = false;
	if (silo_index < 1 || silo_index > 14)
	{
		std::cout << "Ͳ������ʧ��::Ͳ�������Ч" << std::endl;
		return false;
	}
	for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
	{
		if (it->silo_idx == silo_index)
		{
			if (it->silo_type != type && it->cur_storage > 0.0f)
			{
				std::cout << "Ͳ������ʧ��::��������ͻ" << std::endl;
				ret = false;
				break;
			}
			if (it->cur_storage + amount > it->max_storage)
			{
				std::cout << "Ͳ������ʧ��::������������������" << std::endl;
				ret = false;
				break;
			}
			ret = true;
			it->silo_state_up = 1;
			it->silo_coords[3] = 1.0f;
			it->silo_type = type;
			it->silo_flux = flux;
			//Ŀ��ѻ�������
			this->storage_process = 0.0f;
			this->storage_targets = amount;
		}
	}
	return ret;
}

bool Silo::updateSilos(float simurate)
{
	bool ret = false;
	//Ͳ�ָ���
	for (std::vector<Silo_unit>::iterator it1 = this->silos.begin(); it1 != this->silos.end(); it1++)
	{
		if (it1->silo_state_up == 1)
		{
			it1->cur_storage += it1->silo_flux * simurate;
			it1->silo_coords[5] = (float)it1->cur_storage / (float)it1->max_storage;
			if (it1->cur_storage > it1->max_storage)
			{
				//����ͣ��
				it1->cur_storage = it1->max_storage;
				it1->silo_state_up = 0;
				it1->silo_coords[3] = 0.0f;
				ret = true;
			}
			if (this->storage_process > this->storage_targets)
			{
				//���ͣ��
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
			//ж��ͣ��
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
		//ȫ��û�ҵ�ZW12Ͳ�ֶΣ���ȫ������ʧ��
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
		ImGui::Begin(u8"Ͳ��ѡ��");
		static std::vector<Silo_unit>::iterator it_selected;
		for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
		{
			this->pre_button(selected == it->silo_idx);
			if (ImGui::Button((std::to_string(it->silo_idx) + u8"��Ͳ��").c_str()))
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
			ImGui::Text((u8"ѡ��" + std::to_string(it_selected->silo_idx) + u8"��Ͳ�֣�").c_str());
			//��ʾ��������
			if (it_selected->cur_storage > 0)
			{
				ImGui::Text((u8"��ǰ�ѻ���" + std::to_string((int)it_selected->cur_storage) + u8"��").c_str());
				if (it_selected->silo_type)
				{
					ImGui::Text(u8"ú��𣺻���ú");
				}
				else
				{
					ImGui::Text(u8"ú��𣺴��");
				}
			}
			else
			{
				ImGui::Text(u8"��ǰ�޻���");
			}
			ImGui::Text(u8"������������ҵ����");
			ImGui::SameLine();
			ImGui::SliderFloat(" ", &apply_amount, 0.0f, it_selected->max_storage - it_selected->cur_storage);
		}
		ImGui::Text(" ");
		this->pre_button(1);
		if (ImGui::Button(u8"ȷ��"))
		{
			if (selected == 0)
			{
				std::cout << "Ͳ��ѡ��ʧ��::δѡ��Ͳ��" << std::endl;
				this->selected_index = 0;
				this->select_window = false;
			}
			else if (apply_amount < LOW_APPLY)
			{
				std::cout << "Ͳ��ѡ��ʧ��::������ҵ������" << std::endl;
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
		if (ImGui::Button(u8"ȡ��"))
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
		//����úͲ�ֳ�
		for (std::vector<Silo_unit>::iterator it = this->silos.begin(); it != this->silos.end(); it++)
		{
			//����һ��װúһ��жú
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
		//���Ͳ�ֳ�
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
		//����ú
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
		//���
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
	if (*it1 == "����")
	{
		//��ֱֹ��
		this->straight_state = 0;
		this->straight_flux = 0.0f;
	}
	else if (*it1 == "ZW12")
	{
		//��ֹͲ�ֶ���
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
