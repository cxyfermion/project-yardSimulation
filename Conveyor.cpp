#include "Conveyor.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#define CT_WIDTH 4100
#define CT_HEIGHT 2370
#define SCR_WIDTH 1440
#define SCR_HEIGHT 810

#define layer_belts 0.0f		//Ƥ��ͼ��
#define unit_amount 0.220f	//ÿ��Ƥ��װ��ú̿�������֣�

/*
stringתint��stoi(string)
�ָ�����ķ�����string.find()��Ϊ����Ȼ��string.substr()

ע�⣺GLSL�д洢��ʵ����ֵ��ƫ�����float 1.0f��Ҫͨ��>0.9���ж�

���������400����5�����������400����3
���巶Χ������-5��5��������-3��3

ZW12�ĳ���δ֪��������

*/

Conveyor::Conveyor()
{
	//��ʼ����������
	std::string info_type[3];
	info_type[0] = u8"��ʯ,ˮ��,��ҵ��,ֽ��,��ɰ,ͭ��,ʯ�ͽ�,��ú,����ú,��ú,�Ż�,����ú,����";
	info_type[1] = u8"���,�ɻ�,��ͬ,��,��ú,����˹ú,����˹����,��ú";
	info_type[2] = u8"���и�,�񻪾�,��̩,����,�ص׻�,���,����,��һ,��,����";
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
	//Ƥ����Ϣ name x1 y1 x2 y2 pow length
	std::string info_conv[51];
	info_conv[0] = "BC1A,1575,1880,2380,1880,185,380";
	info_conv[1] = "BC1B,1575,1805,2380,1805,185,380";
	info_conv[2] = "BC2A,1510,1410,1510,1915,450,1192";
	info_conv[3] = "BC2B,1640,1335,1640,1915,450,1192";
	info_conv[4] = "BC3A,1435,1370,1685,1370,110,45";
	info_conv[5] = "BC3B,1435,1305,1685,1305,110,45";
	info_conv[6] = "BC4A,1740,1305	,1985,1305,200,205";
	info_conv[7] = "BC4B,1740,1370,1880,1370,200,205";
	info_conv[8] = "BC5,1985,1370,2250,1260,110,280";
	info_conv[9] = "BC6A,1070,1370,1335,1370,185,230";
	info_conv[10] = "BC6B,1070	,1300,1335,1300,185,230";
	info_conv[11] = "BC7,750,1370,965,1370,220,235";
	info_conv[12] = "BC8,2250,900,2250,1230,560,1230";
	info_conv[13] = "BC9,1930,900,1930,1335,560,1380";
	info_conv[14] = "BC10,1385,900,1385,1230,560,1380";
	info_conv[15] = "BC11,1000,900,1000,1335,560,1380";
	info_conv[16] = "BC12,680,900,680,1400,560,1390";
	info_conv[17] = "BC13,1500,930,2645,930,250,665";
	info_conv[18] = "BC14,2640,960,2640,1175,280,610";
	info_conv[19] = "BC15,2680,1175,3900,1175,250,485";
	info_conv[20] = "BC17,1220,860,2315,860,250,665";
	info_conv[21] = "BC18A,610,930,1155,930,250,458";
	info_conv[22] = "BC18B,580,860,1137,860,250,490";
	info_conv[23] = "BC19,1065,1695,1575,1695,200,320";
	info_conv[24] = "BC20A,3210,535,4025,535,355,914";
	info_conv[25] = "BC20B,3160,465,4025,465,355,945";
	info_conv[26] = "BC21A,3210,560,3210,645,200,265";
	info_conv[27] = "BC21B,3120,490,3120,565,200,265";
	info_conv[28] = "BC22A,2530,670,3215,670,500,1416";
	info_conv[29] = "BC22B,2530,590,3120,590,500,1410";
	info_conv[30] = "BC23A,2595,565,2595,825,355,646";
	info_conv[31] = "BC23B,2470,565,2470,745,355,635";
	info_conv[32] = "BC24A,2525,825,2525,1520,355,965";
	info_conv[33] = "BC24B,2410,745,2410,1435,355,970";
	info_conv[34] = "BC25A,1425,1550,2525,1550,500,696";
	info_conv[35] = "BC25B,1425,1460,2410,1465,500,696";
	info_conv[36] = "BC26,1260,1335,1260,1650,200,395";
	info_conv[37] = "BC30A,2440,850,3050,850,185,235";
	info_conv[38] = "BC30B,2470,800,3260,800,200,436";
	info_conv[39] = "BC31,3150,875,3150,1320,355,645";
	info_conv[40] = "BC32,3195,1325,3330,1325,250,155";
	info_conv[41] = "BC33,3395,1325,3630,1325,185,95";
	info_conv[42] = "BC34,3360,1290,3570,1030,315,360";
	info_conv[43] = "BC35,2660,470,3020,470,220,365";
	info_conv[44] = "BC36,3290,800,3455,850,200,255";
	info_conv[45] = "YD1,1330,1610,1555,1610,75,34";
	info_conv[46] = "YD2,1330,1520,1555,1520,75,34";
	info_conv[47] = "XC1,980,1540,1320,1540,110,58";
	info_conv[48] = "XC2,565,1540,930,1540,355,600";
	info_conv[49] = "ZW11,200,985,540,935,280,378";
	info_conv[50] = "ZW12,160,970,160,2180,75,400";
	this->conv.conv_state = 0;
	this->conv.conv_speed = 3.15f;
	this->conv.flow_idx = 100;
	this->conv.flow_name = -1;
	this->conv.conv_cur_storage = 0.0f;
	this->conv.conv_type = 0;
	this->conv.conv_index = 0;
	//Ƥ���߶�����ͼ��߶�
	float layer = layer_belts;
	//Ƥ�����ݶ�ȡ
	for (int i = 0; i < 51; i++)
	{
		last_pos = 0;
		this->conv.conv_idx = i;
		this->conv.conv_name = info_conv[i].substr(0, info_conv[i].find(',', 0));
		last_pos = (int)(info_conv[i].find(',', 0)) + 1;
		if (i > 8 || i < 17)
		{
			this->conv.conv_speed = 3.5f;
		}
		else
		{
			this->conv.conv_speed = 3.15f;
		}
		this->conv_coords[9 * i + 0] = (float)(stoi(info_conv[i].substr(last_pos, info_conv[i].find(',', last_pos) - last_pos))) / (CT_WIDTH / 10.0f) - 5.0f;
		last_pos = (int)(info_conv[i].find(',', last_pos)) + 1;
		this->conv_coords[9 * i + 1] = -(float)(stoi(info_conv[i].substr(last_pos, info_conv[i].find(',', last_pos) - last_pos))) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
		last_pos = (int)(info_conv[i].find(',', last_pos)) + 1;
		this->conv_coords[9 * i + 2] = (float)(stoi(info_conv[i].substr(last_pos, info_conv[i].find(',', last_pos) - last_pos))) / (CT_WIDTH / 10.0f) - 5.0f;
		last_pos = (int)(info_conv[i].find(',', last_pos)) + 1;
		this->conv_coords[9 * i + 3] = -(float)(stoi(info_conv[i].substr(last_pos, info_conv[i].find(',', last_pos) - last_pos))) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
		last_pos = (int)(info_conv[i].find(',', last_pos)) + 1;
		this->conv_coords[9 * i + 4] = layer;
		//color
		this->conv_coords[9 * i + 5] = 1.0f;
		this->conv_coords[9 * i + 6] = 0.0f;
		this->conv_coords[9 * i + 7] = 0.0f;
		//focus
		this->conv_coords[9 * i + 8] = 0.0f;
		this->conv.conv_pow = stoi(info_conv[i].substr(last_pos, info_conv[i].find(',', last_pos) - last_pos));
		last_pos = (int)(info_conv[i].find(',', last_pos)) + 1;
		this->conv.conv_length = stoi(info_conv[i].substr(last_pos, info_conv[i].size() - last_pos));
		this->conv.conv_max_storage = this->conv.conv_length * unit_amount;
		this->belts.push_back(this->conv);
		layer += 0.0001f;
	}
	glGenVertexArrays(1, &convVAO);
	glGenBuffers(1, &convVBO);
	glBindVertexArray(convVAO);
	glBindBuffer(GL_ARRAY_BUFFER, convVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(conv_coords), &conv_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

}

void Conveyor::reset()
{
	this->act_flows.clear();
	std::sort(this->belts.begin(), this->belts.end(), ConvResort());
	for (std::vector<ConvAttrib>::iterator it1 = this->belts.begin(); it1 != this->belts.end(); it1++)
	{
		it1->conv_state = 0;
		it1->conv_cur_storage = 0.0f;
		it1->flow_idx = 100;
		it1->flow_name = -1;
		it1->conv_type = 0;
		it1->conv_index = 0;
		this->conv_coords[9 * it1->conv_idx + 5] = 1.0f;
		this->conv_coords[9 * it1->conv_idx + 6] = 0.0f;
		this->conv_coords[9 * it1->conv_idx + 7] = 0.0f;
	}
}

void Conveyor::draw(Camera& camera, Shader& convShader, float value)
{
	convShader.use();
	//������ģʽ
	glLineWidth(4.0f);
	//VBO��
	glBindBuffer(GL_ARRAY_BUFFER, convVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(conv_coords), &conv_coords, GL_STATIC_DRAW);
	glBindVertexArray(convVAO);
	this->model = glm::mat4(1.0f);
	convShader.setMat4("model", this->model);
	this->projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
	convShader.setMat4("projection", this->projection);
	this->view = glm::mat4(1.0f);
	this->view = camera.GetViewMatrix();
	convShader.setMat4("view", this->view);
	convShader.setFloat("value", value);
	glDrawArrays(GL_POINTS, 0, 51);
}

void Conveyor::initGuiStyle()
{
	this->style = &ImGui::GetStyle();
}

void Conveyor::conv_dispatch(bool unreal)
{
	static int type = 1;				//����
	static int index = 1;				//С��
	static float cur_amount = 0.0f;	//ǿ���趨Ƥ������
	static int conv_select = -1;		//ѡ��Ƥ��
	static bool flow_only = false;		//�Ƿ�ֻ��ʾ������ռ�õ�Ƥ��
	if (ImGui::CollapsingHeader(u8"Ƥ��״̬��ʾ"))
	{
		ImGui::Indent();
		this->pre_button();
		std::string temp = flow_only ? u8"����ʾ������ռ�õ�Ƥ��" : u8"��ʾȫ��Ƥ��";
		if (ImGui::Button(temp.c_str()))
		{
			flow_only = !flow_only;
		}
		this->post_button();
		//�Ƿ�����˳������Ƥ������
		int show_time = 1;		//��ʾѭ������
		if (flow_only)
		{
			show_time = (int)this->act_flows.size();
			std::sort(this->belts.begin(), this->belts.end(), ConvSort());
		}
		else
		{
			std::sort(this->belts.begin(), this->belts.end(), ConvResort());
		}
		std::vector<FlowState>::const_iterator it2 = this->act_flows.begin();
		for (int i = 0; i < show_time; i++)
		{
			for (std::vector<ConvAttrib>::iterator it1 = this->belts.begin(); it1 != this->belts.end(); it1++)
			{
				if (flow_only && it1->flow_name != it2->flow_num)
				{
					continue;
				}
				ImGui::Text((u8"Ƥ�����ţ�" + it1->conv_name + u8" ��").c_str());
				ImGui::SameLine();
				ImGui::Text((u8"��ǰ����" + std::to_string(it1->conv_cur_storage) + u8"��").c_str());
				ImGui::Text((u8"Ƥ�����ȣ�" + std::to_string(it1->conv_length) + u8"��").c_str());
				switch (it1->conv_state)
				{
				case -1:
					ImGui::Text(u8"Ƥ��״̬������");
					break;
				case 0:
					ImGui::Text(u8"Ƥ��״̬��δ�ϵ�");
					break;
				case 1:
					ImGui::Text(u8"Ƥ��״̬��Ƥ���������");
					break;
				case 2:
					ImGui::Text(u8"Ƥ��״̬����������");
					break;
				case 3:
					ImGui::Text(u8"Ƥ��״̬����������");
					break;
				default:
					break;
				}
				//��ʾ������Ϣ
				if (it1->flow_name != -1)
				{
					ImGui::Text((u8"��ǰռ�����̣�" + std::to_string(it1->flow_name)).c_str());
					ImGui::Text((u8"��ǰƤ���������еļ�����" + std::to_string(it1->flow_idx + 1)).c_str());
				}
				else
				{
					ImGui::Text(u8"��ǰ������ռ��");
				}
				//��ʾ��������
				if (it1->conv_type == 0)
				{
					ImGui::Text(u8"��ǰ�޻���");
				}
				else
				{
					for (std::vector<Type_conv>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
					{
						if (it2->type_index == it1->conv_index && it2->type_type == it1->conv_type)
						{
							ImGui::Text((u8"��ǰ��������" + it2->type_name).c_str());
							break;
						}
					}
				}
				//Ƥ��ѡ����ť
				if (unreal)
				{
					this->pre_button();
					if (ImGui::Button((u8"ǿ��ָ��" + it1->conv_name + u8"Ƥ������").c_str()))
					{
						if (conv_select == it1->conv_idx)
						{
							conv_select = -1;
						}
						else
						{
							conv_select = it1->conv_idx;
						}
					}
					this->post_button();
				}
				//ǿ��ָ��Ƥ������
				if (it1->conv_idx == conv_select && unreal)
				{
					ImGui::Text(u8"�ѳ�������ࣺ");
					ImGui::SameLine();
					ImGui::RadioButton(u8"����ɢ��", &type, 1);
					ImGui::SameLine();
					ImGui::RadioButton(u8"��Դɢ��", &type, 2);
					ImGui::SameLine();
					ImGui::RadioButton(u8"��ҵɢ��", &type, 3);
					ImGui::Text(u8"ѡ�����С�ࣺ");
					int temp_1 = 1;
					for (std::vector<Type_conv>::const_iterator it3 = this->names.begin(); it3 != this->names.end(); it3++)
					{
						if (it3->type_type == type)
						{
							ImGui::SameLine();
							ImGui::RadioButton(it3->type_name.c_str(), &index, temp_1);
							temp_1++;
						}
					}
					ImGui::SliderFloat(u8"���õ�ǰ����", &cur_amount, 0.0f, it1->conv_max_storage);
					this->pre_button();
					if (ImGui::Button(u8"�趨"))
					{
						it1->conv_index = index;
						it1->conv_type = type;
						it1->conv_cur_storage = cur_amount;
					}
					this->post_button();
				}
				ImGui::Separator();
			}
			if (it2 != this->act_flows.end())
			{
				it2++;
			}
		}
		ImGui::Unindent();
	}
}

void Conveyor::updateConvs(float simurate)
{
	for (std::vector<FlowState>::iterator it1 = this->act_flows.begin(); it1 != this->act_flows.end(); )
	{
		bool temp = true;
		std::sort(this->belts.begin(), this->belts.end(), ConvSort());		//��Ƥ������˳������Ƥ��
		bool empty_begin = !it1->begin;								//��λƤ������һ���Ƿ��
		bool empty_whole = true;										//ĩλƤ���Ƿ��
		int flag = it1->begin ? 2 : 0;		//��һ����0Ϊ�գ�1������2����
		//bool prev = it1->begin;			//��һ����falseΪ�գ�true����
		//int next = 0;						//��һ����0Ϊ�գ�1������2����
		for (std::vector<ConvAttrib>::iterator it2 = this->belts.begin(); it2 != this->belts.end(); it2++)
		{
			if (it2->flow_name != it1->flow_num)
			{
				continue;
			}
			/**/
			//������ģʽ����һ�����ˣ���Ƥ������װ��������һ�����ˣ���Ƥ������װ����
			if (empty_begin)
			{
				if (flag == 0)
				{
					it2->conv_cur_storage -= unit_amount * simurate / 1;
					if (it2->conv_cur_storage < 0.0f)
					{
						it2->conv_cur_storage = 0.0f;
					}
				}
				else if (flag == 2)
				{
					it2->conv_cur_storage += unit_amount * simurate / 1;
					if (it2->conv_cur_storage > it2->conv_max_storage)
					{
						it2->conv_cur_storage = it2->conv_max_storage;
					}
				}
			}
			//������ģʽ����һ�����ˣ���Ƥ������װ��������һ��û������Ƥ������װ����
			else
			{
				if (flag == 2)
				{
					it2->conv_cur_storage += unit_amount * simurate / 1;
					if (it2->conv_cur_storage > it2->conv_max_storage)
					{
						it2->conv_cur_storage = it2->conv_max_storage;
					}
				}
				else if (flag == 1)
				{
					it2->conv_cur_storage -= unit_amount * simurate / 1;
					if (it2->conv_cur_storage < 0.0f)
					{
						it2->conv_cur_storage = 0.0f;
					}
				}
			}
			if (it2->conv_cur_storage == 0.0f)
			{
				flag = 0;
			}
			else if (it2->conv_cur_storage == it2->conv_max_storage)
			{
				flag = 2;
			}
			else
			{
				flag = 1;
			}
			/**/

			/*
			//����Ƥ����˳�����һ�µ�Ƥ�������һ��������Ѱ����һ�����̵�ʱ����Ҫע������
			std::vector<ConvAttrib>::iterator it3 = it2;
			it3++;
			for (; it3 != this->belts.end(); it3++)
			{
				if (it3->flow_name == it1->flow_num)
				{
					break;
				}
			}
			//���һ������һ����Ϊ��
			if (it3 == this->belts.end())
			{
				next = 0;
			}
			else
			{
				if (it3->conv_cur_storage == it3->conv_max_storage)
				{
					next = 2;
				}
				else if (it3->conv_cur_storage == 0.0f)
				{
					next = 0;
				}
				else
				{
					next = 1;
				}
			}
			//��һ��Ϊ�գ���һ�����գ���һ������
			if (!prev && it2->conv_cur_storage > 0.0f)
			{
				it2->conv_cur_storage -= unit_amount * simurate / 1;
				if (it2->conv_cur_storage < 0.0f)
				{
					it2->conv_cur_storage = 0.0f;
				}
			}
			//��һ��Ϊ�գ���һ��Ϊ�գ���һ������
			//��һ�����գ���һ�����գ���һ����������һ������
			//��һ�����գ���һ�����ˣ���һ�����ˣ���һ������
			//��һ�����գ���һ����������һ�����ˣ���һ������
			//��һ�����գ���һ�����գ���һ��Ϊ�գ���һ������
			//��һ�����գ���һ��Ϊ�գ���һ����������һ������
			//��һ�����գ���һ��Ϊ�գ���һ�����ˣ���һ������
			//��һ�����գ���һ��Ϊ�գ���һ��Ϊ�գ���һ������
			else if (prev && ((it2->conv_cur_storage == 0.0f) || (it2->conv_cur_storage > 0.0f && next == 2)))
			{
				it2->conv_cur_storage += unit_amount * simurate / 1;
				if (it2->conv_cur_storage > it2->conv_max_storage)
				{
					it2->conv_cur_storage = it2->conv_max_storage;
				}
			}
			prev = it2->conv_cur_storage > 0.0f;
			*/
			//������ɫ
			this->conv_coords[9 * it2->conv_idx + 5] = 0.0f;
			this->conv_coords[9 * it2->conv_idx + 6] = 1.0f - 0.6f * it2->conv_cur_storage / it2->conv_max_storage;
			this->conv_coords[9 * it2->conv_idx + 7] = 0.0f;
			if (empty_whole && it2->conv_cur_storage > 0.0f)
			{
				empty_whole = false;
			}
		}
		//��ȫ��Ƥ�����������룩Ϊ�գ��ս����̣�ȫ·Ƥ������������������̱�־
		if (empty_begin && empty_whole)
		{
			for (std::vector<ConvAttrib>::iterator it3 = this->belts.begin(); it3 != this->belts.end(); it3++)
			{
				if (it3->flow_name != it1->flow_num)
				{
					continue;
				}
				it3->conv_type = 0;
				it3->conv_index = 0;
				it3->flow_name = -1;
				it3->flow_idx = 100;
				it3->conv_state = 1;
				//���ö�Ӧ��ɫ
				this->conv_coords[9 * it3->conv_idx + 5] = 0.0f;
				this->conv_coords[9 * it3->conv_idx + 6] = 0.0f;
				this->conv_coords[9 * it3->conv_idx + 7] = 1.0f;
			}
			//��������ȥ��
			it1 = this->act_flows.erase(it1);
			temp = false;
		}
		if (temp)
		{
			it1++;
		}
	}
}

void Conveyor::add_type(std::string str_name, int type_type)
{
	int lst_idx = 1;		//ԭ��Ӧ������С����������
	for (std::vector<Type_conv>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
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

bool Conveyor::startCheck(std::vector<std::string>& equipments)
{
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<ConvAttrib>::const_iterator it2 = this->belts.begin(); it2 != this->belts.end(); it2++)
		{
			if (*it1 == it2->conv_name)
			{
				if (it2->conv_state >= 1)
				{
					return false;
				}
				break;
			}
		}
	}
	return true;
}

bool Conveyor::sceneCheck(std::vector<std::string>& equipments)
{
	//�������Ƥ��Ϊ��ɫ
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<ConvAttrib>::iterator it2 = this->belts.begin(); it2 != this->belts.end(); it2++)
		{
			if (*it1 == it2->conv_name)
			{
				//����Ƥ��״̬
				it2->conv_state = 1;
				//���ö�Ӧ��ɫ
				this->conv_coords[9 * it2->conv_idx + 5] = 0.0f;
				this->conv_coords[9 * it2->conv_idx + 6] = 0.0f;
				this->conv_coords[9 * it2->conv_idx + 7] = 1.0f;
				break;
			}
		}
	}
	return true;
}

void Conveyor::run_unloaded(std::vector<std::string>& equipments)
{
	//�������Ƥ��Ϊǳ��ɫ
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<ConvAttrib>::iterator it2 = this->belts.begin(); it2 != this->belts.end(); it2++)
		{
			if (*it1 == it2->conv_name)
			{
				//����Ƥ��״̬
				it2->conv_state = 2;
				//���ö�Ӧ��ɫ
				this->conv_coords[9 * it2->conv_idx + 5] = 0.0f;
				this->conv_coords[9 * it2->conv_idx + 6] = 1.0f;
				this->conv_coords[9 * it2->conv_idx + 7] = 0.0f;
				break;
			}
		}
	}
}

bool Conveyor::runCheck(int type, int index, std::vector<std::string>& equipments)
{
	//����⵽�����������̵�Ƥ�����л��һ����Ͳ�ƥ�䣬�Ͳ���������
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<ConvAttrib>::const_iterator it2 = this->belts.begin(); it2 != this->belts.end(); it2++)
		{
			if (*it1 == it2->conv_name)
			{
				if (it2->conv_cur_storage > 0.0f && (it2->conv_type != type || it2->conv_index != index))
				{
					return false;
				}
				break;
			}
		}
	}
	return true;
}

void Conveyor::run_loaded(int index_flow, int type, int index, std::vector<std::string>& equipments)
{
	//�������Ƥ��Ϊǳ��ɫ������ʱ�����ź�Ƥ��ǰ������ɫ�������ռ��������ź�Ƥ��˳��
	this->flow_state.flow_num = index_flow;
	this->flow_state.begin = true;
	this->act_flows.push_back(this->flow_state);
	int temp = 0;
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<ConvAttrib>::iterator it2 = this->belts.begin(); it2 != this->belts.end(); it2++)
		{
			if (*it1 == it2->conv_name)
			{
				it2->flow_idx = temp;
				temp++;
				it2->flow_name = index_flow;
				it2->conv_state = 3;
				it2->conv_type = type;
				it2->conv_index = index;
				//���ö�Ӧ��ɫ
				this->conv_coords[9 * it2->conv_idx + 5] = 0.0f;
				this->conv_coords[9 * it2->conv_idx + 6] = 1.0f;
				this->conv_coords[9 * it2->conv_idx + 7] = 0.0f;
				break;
			}
		}
	}
}

void Conveyor::shutDown(int index_flow)
{
	//����ֹͣ������Ӧ����ʼԴ������Ϊ��
	for (std::vector<FlowState>::iterator it1 = this->act_flows.begin(); it1 != this->act_flows.end(); it1++)
	{
		if (it1->flow_num == index_flow)
		{
			it1->begin = false;
		}
	}
}

void Conveyor::cancel(std::vector<std::string>& equipments)
{
	//����ֹͣ���������Ƥ��Ϊ��ɫ
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<ConvAttrib>::iterator it2 = this->belts.begin(); it2 != this->belts.end(); it2++)
		{
			if (*it1 == it2->conv_name && it2->conv_state == 1)
			{
				it2->conv_state = 0;
				this->conv_coords[9 * it2->conv_idx + 5] = 1.0f;
				this->conv_coords[9 * it2->conv_idx + 6] = 0.0f;
				this->conv_coords[9 * it2->conv_idx + 7] = 0.0f;
				break;
			}
		}
	}
}

bool Conveyor::cancelCheck(std::vector<std::string>& equipments)
{
	bool ret = false;
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<ConvAttrib>::const_iterator it2 = this->belts.begin(); it2 != this->belts.end(); it2++)
		{
			if (*it1 == it2->conv_name && it2->conv_state == 1)
			{
				ret = true;
				break;
			}
		}
	}
	return ret;
}

void Conveyor::emergency_shutDown(int index_flow, std::vector<std::string>& equipments)
{
	//���̼�ͣ�����ռ��������ź�Ƥ��˳��
	for (std::vector<FlowState>::iterator it1 = this->act_flows.begin(); it1 != this->act_flows.end(); )
	{
		bool temp = true;
		if (it1->flow_num == index_flow)
		{
			it1 = this->act_flows.erase(it1);
			temp = false;
		}
		if (temp)
		{
			it1++;
		}
	}
	//���̼�ͣ���������Ƥ��Ϊ��ɫ
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<ConvAttrib>::iterator it2 = this->belts.begin(); it2 != this->belts.end(); it2++)
		{
			if (*it1 == it2->conv_name)
			{
				it2->flow_name = -1;
				it2->flow_idx = 100;
				it2->conv_state = 0;
				this->conv_coords[9 * it2->conv_idx + 5] = 1.0f;
				this->conv_coords[9 * it2->conv_idx + 6] = 0.0f;
				this->conv_coords[9 * it2->conv_idx + 7] = 0.0f;
				break;
			}
		}
	}
}

void Conveyor::set_focus(std::vector<std::string>& equipments)
{
	for (std::vector<ConvAttrib>::iterator it2 = this->belts.begin(); it2 != this->belts.end(); it2++)
	{
		//ʹʧ��
		this->conv_coords[9 * it2->conv_idx + 8] = 1.0f;
		for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
		{
			if (*it1 == it2->conv_name)
			{
				this->conv_coords[9 * it2->conv_idx + 8] = 0.0f;
				break;
			}
		}
	}
}

void Conveyor::lose_focus()
{
	for (std::vector<ConvAttrib>::iterator it = this->belts.begin(); it != this->belts.end(); it++)
	{
		this->conv_coords[9 * it->conv_idx + 8] = 0.0f;
	}
}

void Conveyor::pre_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}

void Conveyor::post_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}
