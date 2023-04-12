#include "Flow.h"

#define FLUX 0.0022f					//����ͨ������
#define RECLAIM_SAFE_START 10.0f		//ȡ����������ʱ�ѳ���͵�ǰ����

Flow::Flow()
{
	this->window_confirm = false;
	this->onfocus = -1;
	this->emergency_stop = false;
	std::string info_flow[113];
	//21�Ų�λ���̣���4�Ų�λ�ѳ�
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
	info_flow[32] = "2151,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC6A,BC7A,BC12,SR10";
	info_flow[33] = "2152,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC6B,BC7A,BC12,SR10";
	info_flow[34] = "2153,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC6A,BC7A,BC12,SR10";
	info_flow[35] = "2154,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC6B,BC7B,BC12,SR10";
	info_flow[36] = "2155,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2A,BC3A,BC6A,BC7A,BC12,SR10";
	info_flow[37] = "2156,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC2B,BC3B,BC6B,BC7B,BC12,SR10";
	info_flow[38] = "2157,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2A,BC3A,BC6A,BC7A,BC12,SR10";
	info_flow[39] = "2158,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC2B,BC3B,BC6B,BC7B,BC12,SR10";
	info_flow[40] = "2161,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC26,BC6A,BC7A,BC12,SR10";
	info_flow[41] = "2162,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC26,BC6B,BC7A,BC12,SR10";
	info_flow[42] = "2163,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC26,BC6A,BC7A,BC12,SR10";
	info_flow[43] = "2164,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC26,BC6B,BC7B,BC12,SR10";
	info_flow[44] = "2165,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC26,BC6A,BC7A,BC12,SR10";
	info_flow[45] = "2166,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC26,BC6B,BC7B,BC12,SR10";
	info_flow[46] = "2167,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC26,BC6A,BC7A,BC12,SR10";
	info_flow[47] = "2168,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC26,BC6B,BC7B,BC12,SR10";
	info_flow[48] = "2171,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC26,BC6A,BC11,SR9";
	info_flow[49] = "2172,BC20A,BC21A,BC22A,BC23A,BC24A,BC25A,YD1,BC26,BC6B,BC11,SR9";
	info_flow[50] = "2173,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC26,BC6A,BC11,SR9";
	info_flow[51] = "2174,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,BC26,BC6B,BC11,SR9";
	info_flow[52] = "2175,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC26,BC6A,BC11,SR9";
	info_flow[53] = "2176,BC20B,BC21B,BC22B,BC23A,BC24A,BC25A,YD1,BC26,BC6B,BC11,SR9";
	info_flow[54] = "2177,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC26,BC6A,BC11,SR9";
	info_flow[55] = "2178,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,BC26,BC6B,BC11,SR9";
	//�����ǳ�
	info_flow[56] = "2181,BC20A,BC21A,BC22A,BC23A,BC30A,BC31,SR11";
	info_flow[57] = "2182,BC20B,BC21B,BC22B,BC23B,BC30A,BC31,SR11";
	info_flow[58] = "2183,BC20A,BC21A,BC22A,BC23B,BC30B,BC36,S12";
	info_flow[59] = "2184,BC20B,BC21B,BC22B,BC23B,BC30B,BC36,S12";
	//���Ŷ���
	info_flow[60] = "2179,BC20A,BC21A,BC22A,BC23B,BC24B,BC25B,YD2,XBC1,XBC2,SR12";
	info_flow[61] = "2180,BC20B,BC21B,BC22B,BC23B,BC24B,BC25B,YD2,XBC1,XBC2,SR12";
	//װ��
	info_flow[62] = "2191,BC20B,BC21B,SL4";
	//4�Ų�λ���̣��϶ѳ�
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
	info_flow[87] = "441,BC1A,BC2A,BC3A,BC6A,BC7A,BC12,SR10";
	info_flow[88] = "442,BC1A,BC2B,BC3B,BC6B,BC7B,BC12,SR10";
	info_flow[89] = "443,BC1B,BC2A,BC3A,BC6A,BC7A,BC12,SR10";
	info_flow[90] = "444,BC1B,BC2B,BC3B,BC6B,BC7B,BC12,SR10";
	info_flow[91] = "445,BC19,BC2A,BC3A,BC6A,BC7A,BC12,SR10";
	info_flow[92] = "446,BC19,BC2B,BC3B,BC6B,BC7B,BC12,SR10";
	//װ��
	info_flow[93] = "451,SR6,BC8,BC13,BC14,BC15";
	info_flow[94] = "452,SR7,BC9,BC13,BC14,BC15";
	info_flow[95] = "453,SR8,BC10,BC13,BC14,BC15";
	info_flow[96] = "461,SR11,BC31,BC32,BC33";
	info_flow[97] = "462,BC34,BC33";
	//������
	info_flow[98] = "471,SR6,BC8,BC17,BC18B,����";
	info_flow[99] = "472,SR7,BC9,BC17,BC18B,����";
	info_flow[100] = "473,SR8,BC10,BC17,BC18B,����";
	info_flow[101] = "474,SR9,BC11,BC18B,����";
	info_flow[102] = "475,SR6,BC8,BC17,BC18B,ZW11,ZW12";
	info_flow[103] = "476,SR7,BC9,BC17,BC18B,ZW11,ZW12";
	info_flow[104] = "477,SR8,BC10,BC17,BC18B,ZW11,ZW12";
	info_flow[105] = "478,SR9,BC11,BC18B,ZW11,ZW12";
	info_flow[106] = "481,SR10,BC12,BC18A,ZW11,ZW12";
	info_flow[107] = "482,SR10,BC12,BC18B,ZW11,ZW12";
	info_flow[108] = "483,SR10,BC12,BC18B,����";
	info_flow[109] = "485,SR6,BC8,BC17,BC18A,ZW11,ZW12";
	info_flow[110] = "486,SR7,BC9,BC17,BC18A,ZW11,ZW12";
	info_flow[111] = "487,SR8,BC10,BC17,BC18A,ZW11,ZW12";
	info_flow[112] = "488,SR9,BC11,BC18A,ZW11,ZW12";
	//���̶�ȡ
	this->flow_attr.flow_state = 0;
	this->flow_attr.flow_time = 0;
	this->flow_attr.scene_ready = false;
	//��������
	int last_pos = 0;
	//��ʱ���̴��Ŵ洢
	std::string name;
	for (int i = 0; i < 113; i++)
	{
		//��¼','���һ�γ���λ��
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

void Flow::add_type(Message& message, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard)
{
	static int type_type = 1;			//����
	static char str_name[50] = "";		//����
	if (ImGui::CollapsingHeader(u8"��Ʒ�ֻ������"))
	{
		ImGui::Indent();
		ImGui::Text(u8"�����������ƣ�");
		ImGui::SameLine();
		ImGui::InputTextWithHint(" ", u8"�ڴ������²�������", str_name, IM_ARRAYSIZE(str_name));
		ImGui::Text(u8"ѡ���´��ࣺ");
		ImGui::SameLine();
		ImGui::RadioButton(u8"�����", &type_type, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"����Դ��", &type_type, 2);
		ImGui::SameLine();
		ImGui::RadioButton(u8"����ҵ��", &type_type, 3);
		this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
		this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
		if (ImGui::Button(u8"���"))
		{
			conv.add_type(str_name, type_type);
			yard.add_type(str_name, type_type);
			berth.add_type(str_name, type_type);
			train.add_type(str_name, type_type);
			//std::cout << "������ӳɹ���" << std::endl;
			message.push_message(u8"������ӳɹ���");
		}
		this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
		this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
		ImGui::Unindent();
	}
}

void Flow::showGui(Message& message, Energy& energy, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo)
{
	if (ImGui::CollapsingHeader(u8"���̿������"))
	{
		ImGui::Indent();
		//���̿��ư�ť
		if (this->onfocus != -1)
		{
			bool temp = false;
			std::vector<FlowAttrib>::iterator it = this->flows.begin();
			while (it->flow_idx != this->onfocus)
			{
				it++;
			}
			ImGui::Text((u8"��ǰѡ�У�" + it->flow_name + u8" ����").c_str());
			ImGui::SameLine();
			ImGui::Text(u8"�����豸��");
			for (std::vector<std::string>::const_iterator it1 = it->equipments.begin(); it1 != it->equipments.end(); it1++)
			{
				ImGui::SameLine();
				if (*it1 == "����")
				{
					ImGui::Text(u8"����");
				}
				else
				{
					ImGui::Text(it1->c_str());
				}
			}
			//״̬�ؼ�Ⱥ
			temp = it->flow_state == 0 && conv.startCheck(it->equipments) && wheel.startCheck(it->equipments);
			this->setText(temp);
			if (ImGui::Button(u8"����ѡ��"))//��state=0ʱ��Ч
			{
				if (temp)
				{
					//��ť��Ϊ��ɫ
					it->flow_state = 1;
					//��Ӧ���ع��Ϊ�䱸״̬
					energy.coldStart(it->equipments);
				}
				else
				{
					if (it->flow_state != 0)
					{
						//std::cout << "����ʧ��::�����Ѿ�����" << std::endl;
						message.push_message(u8"����ʧ��::�����Ѿ�����");
					}
					else
					{
						//std::cout << "����ʧ��::�豸��ռ��" << std::endl;
						message.push_message(u8"����ʧ��::�豸��ռ��");
					}
				}
			}
			ImGui::SameLine();
			this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
			if (ImGui::Button(u8"ȡ��ѡ��"))//������Ч
			{
				this->onfocus = -1;
				conv.lose_focus();
				wheel.lose_focus();
				berth.lose_focus();
				train.lose_focus();
				yard.lose_focus();
				silo.lose_focus();
			}
			ImGui::SameLine();
			temp = it->flow_state == 1 && it->scene_ready == false;
			this->setText(temp);
			if (ImGui::Button(u8"�ֳ����"))//��state=1ʱ��Ч
			{
				if (temp)
				{
					//Ƥ������ɫ
					it->scene_ready = conv.sceneCheck(it->equipments);
				}
				else
				{
					if (it->flow_state == 1)
					{
						//std::cout << "���ʧ��::�����Ѿ����л�δ��������" << std::endl;
						message.push_message(u8"���ʧ��::�����Ѿ����л�δ��������");
					}
					else
					{
						//std::cout << "���ʧ��::�Ѿ�������" << std::endl;
						message.push_message(u8"���ʧ��::�Ѿ�������");
					}
				}
			}
			ImGui::SameLine();
			temp = it->flow_state == 1 && it->scene_ready == true;
			this->setText(temp);
			if (ImGui::Button(u8"��������"))//��state=1���ֳ�����ʱ��Ч
			{
				if (temp)
				{
					//Ƥ����ǳ��ɫ
					conv.run_unloaded(it->equipments);
					//�Ż�������״̬
					berth.run_unloader_unloaded(message, it->equipments);
					//���ع�����
					energy.hotStart(it->equipments);
					it->flow_state = 2;
				}
				else
				{
					if (it->flow_state == 1)
					{
						//std::cout << "��������ʧ��::�����Ѿ����л�δ��������" << std::endl;
						message.push_message(u8"��������ʧ��::�����Ѿ����л�δ��������");
					}
					else
					{
						//std::cout << "��������ʧ��::���δ����" << std::endl;
						message.push_message(u8"��������ʧ��::���δ����");
					}
				}
			}
			ImGui::SameLine();
			temp = it->flow_state == 2;
			this->setText(temp);
			std::string name_inst = "";
			if (yard.data_send)
			{
				name_inst = u8"�������С������̿�ʼ";
				if (!(it->flow_idx < 102 || it->flow_idx == 108))
				{
					if (!silo.data_send)
					{
						name_inst = u8"�������С���Ͳ��ѡ��";
					}
				}
			}
			else
			{
				name_inst = u8"�������С����ѳ�ѡ��";
			}
			if (ImGui::Button(name_inst.c_str()))//����state=2ʱ��Ч
			{
				if (temp)
				{
					if (it->flow_idx < 93 && it->flow_idx != 62)
					{
						//�������У���Ҫȷ�϶ѳ���
						int type = 0;
						int index = 0;
						float amount = 0.0f;
						if (it->flow_idx < 62)
						{
							//ͨ�ò�λ
							type = berth.get_up_type();
							index = berth.get_up_index();
							amount = berth.get_up_amount();
						}
						else if ((it->flow_idx > 62 && it->flow_idx < 79) || (it->flow_idx > 86 && it->flow_idx < 91))
						{
							//4��λ
							type = berth.get_down_type();
							index = berth.get_down_index();
							amount = berth.get_down_amount();
						}
						if (type <= 0 || index <= 0 || amount == 0.0f)
						{
							//���ݻ�ȡʧ��
							if (it->flow_idx < 62)
							{
								//std::cout << "���̸�������ʧ��::ͨ�ò�λ����δ����" << std::endl;
								message.push_message(u8"���̸�������ʧ��::ͨ�ò�λ����δ����");
							}
							else if ((it->flow_idx > 62 && it->flow_idx < 79) || (it->flow_idx > 86 && it->flow_idx < 91))
							{
								//std::cout << "���̸�������ʧ��::4��λ����δ����" << std::endl;
								message.push_message(u8"���̸�������ʧ��::4��λ����δ����");
							}
						}
						else if (!conv.runCheck(type, index, it->equipments))
						{
							//std::cout << "���̸�������ʧ��::��ǰƤ���д���û��ж�������಻ƥ��Ļ���" << std::endl;
							message.push_message(u8"���̸�������ʧ��::��ǰƤ���д���û��ж�������಻ƥ��Ļ���");
						}
						else
						{
							//�ѳ��趨
							if (!yard.data_send)
							{
								//δ�������ݣ�����ѡ�񴰿�
								yard.yard_choosing = true;
							}
							else
							{
								//�������ݣ���������
								if (yard.start_stack(message, it->equipments, yard.yard_choosed, yard.child_choosed, type, index, amount, FLUX))
								{
									//��ǰ��λ�޴��������������̣���Ϊ�������������
									if (berth.set_unloading_ship(message, it->equipments))
									{
										//���ֻ�����
										wheel.run(it->equipments, 0);
										//Ƥ��������ɫ
										conv.run_loaded(stoi(it->flow_name), type, index, it->equipments);
										//��λ�豸����ɫ
										berth.run_unloader_loaded(it->equipments);
										it->flow_state = 3;
									}
								}
								yard.send_reset();
							}
						}
					}
					else if ((it->flow_idx > 92 && it->flow_idx < 97))
					{
						//ȡ�����У���Ҫȷ�϶ѳ���
						int type = 0;
						int index = 0;
						float cur_amount = 0.0f;
						if (!yard.data_send)
						{
							yard.yard_choosing = true;
						}
						else
						{
							//����ѡ�жѳ�
							type = yard.type_choosed;
							index = yard.index_choosed;
							cur_amount = yard.amount_choosed;
							if (cur_amount < RECLAIM_SAFE_START)
							{
								//std::cout << "���̸�������ʧ��::�ѳ���ǰ������������������" << std::endl;
								message.push_message(u8"���̸�������ʧ��::�ѳ���ǰ������������������");
								yard.send_reset();
							}
							else if (!conv.runCheck(type, index, it->equipments))
							{
								//std::cout << "���̸�������ʧ��::��ǰƤ���д���û��ж�������಻ƥ��Ļ���" << std::endl;
								message.push_message(u8"���̸�������ʧ��::��ǰƤ���д���û��ж�������಻ƥ��Ļ���");
							}
							else
							{
								//ȡ����������
								if (yard.start_reclaim(message, it->equipments, yard.yard_choosed, yard.child_choosed, type, index, FLUX))
								{
									//װ��¥����
									if (train.run(it->equipments, type, index))
									{
										//���ֻ�����
										wheel.run(it->equipments, 1);
										//Ƥ��������ɫ
										conv.run_loaded(stoi(it->flow_name), type, index, it->equipments);
										it->flow_state = 3;
									}
									else
									{
										it->flow_state = 2;
										//std::cout << "��������ʧ��::û�п����ػ��Ļ�" << std::endl;
										message.push_message(u8"��������ʧ��::û�п����ػ��Ļ�");
									}
								}
								yard.send_reset();
							}
						}
					}
					else if (it->flow_idx > 97)
					{
						//����������Ҫѡ��ѳ���Ͳ�֣�������ֱ������
						static int artifice_type = 0;
						static int artifice_index = 0;
						static float artifice_cur_amount = 0.0f;
						if (!yard.data_send)
						{
							yard.yard_choosing = true;
						}
						else
						{
							//����ѡ�жѳ�
							artifice_type = yard.type_choosed;
							artifice_index = yard.index_choosed;
							artifice_cur_amount = yard.amount_choosed;
							if (artifice_cur_amount < RECLAIM_SAFE_START)
							{
								//std::cout << "���̸�������ʧ��::�ѳ���ǰ������������������" << std::endl;
								message.push_message(u8"���̸�������ʧ��::�ѳ���ǰ������������������");
								yard.send_reset();
							}
							else if (!((artifice_type == 1 && artifice_index == 9) || (artifice_type == 2 && artifice_index == 1)))
							{
								//std::cout << "���̸�������ʧ��::�Ǵ��ͻ���ú������������" << std::endl;
								message.push_message(u8"���̸�������ʧ��::�Ǵ��ͻ���ú������������");
								yard.send_reset();
							}
							else if (!conv.runCheck(artifice_type, artifice_index, it->equipments))
							{
								//std::cout << "���̸�������ʧ��::��ǰƤ���д���û��ж�������಻ƥ��Ļ���" << std::endl;
								message.push_message(u8"���̸�������ʧ��::��ǰƤ���д���û��ж�������಻ƥ��Ļ���");
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
									//ֱ��
									silo.start_straight(FLUX, coal_type);
								}
								else
								{
									//ѡ��Ͳ�����
									if (!silo.data_send)
									{
										silo.select_window = true;
									}
									else
									{
										int silo_choosed = silo.selected_index;
										if (silo_choosed == 0)
										{
											//std::cout << "Ͳ��ѡ��ʧ��::δѡ��Ͳ��" << std::endl;
											message.push_message(u8"Ͳ��ѡ��ʧ��::δѡ��Ͳ��");
											silo.send_reset();
											yard.send_reset();
										}
										else
										{
											//�����ѳ�
											if (yard.start_reclaim(message, it->equipments, yard.yard_choosed, yard.child_choosed, artifice_type, artifice_index, FLUX))
											{
												//����Ͳ��
												if (silo.start_stack(silo_choosed, coal_type, silo.amount_apply, FLUX))
												{
													//���ֻ�����
													wheel.run(it->equipments, 1);
													//Ƥ��������ɫ
													conv.run_loaded(stoi(it->flow_name), artifice_type, artifice_index, it->equipments);
													it->flow_state = 3;
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
						//װ��
						int type = berth.get_up_type();
						int index = berth.get_up_index();
						float amount = berth.get_up_amount();
						if (type <= 0 || index <= 0 || amount == 0.0f)
						{
							//���ݻ�ȡʧ��
							//std::cout << "���̸�������ʧ��::ͨ�ò�λ����δ����" << std::endl;
							message.push_message(u8"���̸�������ʧ��::ͨ�ò�λ����δ����");
						}
						else if (!conv.runCheck(type, index, it->equipments))
						{
							//std::cout << "���̸�������ʧ��::��ǰƤ���д���û��ж�������಻ƥ��Ļ���" << std::endl;
							message.push_message(u8"���̸�������ʧ��::��ǰƤ���д���û��ж�������಻ƥ��Ļ���");
						}
						else
						{
							if (berth.set_loading_ship(message, type, index))
							{
								//Ƥ��������ɫ
								conv.run_loaded(stoi(it->flow_name), type, index, it->equipments);
								//��λ�豸����ɫ
								berth.run_unloader_loaded(it->equipments);
								it->flow_state = 3;
							}
						}
					}
					else if (it->flow_idx == 97)
					{
						//�ؿ�=>��
						//Ƥ��������ɫ
						conv.run_loaded(stoi(it->flow_name), 2, 1, it->equipments);
						//װ��¥����ɫ����ʱָ����װ����죩
						if (!train.run(it->equipments, 2, 1))
						{
							it->flow_state = 2;
							//std::cout << "��������ʧ��::û�п����ػ��Ļ�" << std::endl;
							message.push_message(u8"��������ʧ��::û�п����ػ��Ļ�");
						}
						it->flow_state = 3;
					}
				}
				else
				{
					//std::cout << "��������ʧ��::�豸δ����" << std::endl;
					message.push_message(u8"��������ʧ��::�豸δ����");
				}
			}
			ImGui::SameLine();
			temp = it->flow_state >= 2;
			this->setText(temp);
			if (ImGui::Button(u8"����ֹͣ"))//����state>=2ʱ��Ч
			{
				if (temp)
				{
					//Ƥ������ɫ
					conv.shutDown(stoi(it->flow_name));
					//���ֻ����ɫ
					wheel.shutDown(it->equipments);
					//��λ�豸���ɫ
					berth.unloader_shutDown(it->equipments);
					//װ��¥���ɫ
					train.shutDown(it->equipments);
					//�ѳ�ֹͣ
					yard.yard_end(it->equipments);
					//Ͳ��ֹͣ
					silo.end_silo(it->equipments);
					//���ع�ת�ȱ�
					energy.hotEnd(it->equipments);
					//��ť���ɫ
					it->flow_state = 1;
				}
				else
				{
					//std::cout << "ֹͣʧ��::����δ����" << std::endl;
					message.push_message(u8"ֹͣʧ��::����δ����");
				}
			}
			ImGui::SameLine();
			temp = it->flow_state == 1 && conv.cancelCheck(it->equipments);
			this->setText(temp);
			if (ImGui::Button(u8"����ȡ��"))//����state=1�Ҷ�ӦƤ��״̬ȫ�������ɫʱ��Ч
			{
				if (temp)
				{
					//Ƥ�����ɫ
					conv.cancel(it->equipments);
					//���ع���䱸
					energy.coldEnd(it->equipments);
					it->flow_state = 0;
					it->scene_ready = false;
				}
				else
				{
					//std::cout << "ȡ��ʧ��::���̽�����" << std::endl;
					message.push_message(u8"ȡ��ʧ��::���̽�����");
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
			if (ImGui::Button(u8"���̼�ͣ"))//����state!=0ʱ��Ч
			{
				if (temp)
				{
					this->emergency_stop = !this->emergency_stop;
				}
				else
				{
					//std::cout << "��ͣʧ��::����δ����" << std::endl;
					message.push_message(u8"��ͣʧ��::����δ����");
				}
			}
			//��ͣ����
			this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
			if (this->emergency_stop)
			{
				ImGui::Begin(u8"ȷ�ϼ�ͣ");
				ImGui::Text(u8"ȷ�����̼�ͣ��");
				this->style->Colors[ImGuiCol_Button] = ImColor(250, 0, 0, 255);
				this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
				if (ImGui::Button(u8"ȷ��"))
				{
					//Ƥ�����ɫ
					conv.emergency_shutDown(stoi(it->flow_name), it->equipments);
					//���ֻ����ɫ
					wheel.shutDown(it->equipments);
					//��λ�豸���ɫ
					berth.unloader_shutDown(it->equipments);
					//װ��¥���ɫ
					train.shutDown(it->equipments);
					//�ѳ�ֹͣ
					yard.yard_end(it->equipments);
					//Ͳ��ֹͣ
					silo.end_silo(it->equipments);
					//���ع�ͣ
					energy.emergencyEnd(it->equipments);
					it->flow_state = 0;
					it->scene_ready = false;
					this->emergency_stop = false;
				}
				ImGui::SameLine(100.0f);
				this->style->Colors[ImGuiCol_Button] = ImColor(0, 250, 0, 255);
				if (ImGui::Button(u8"ȡ��"))
				{
					this->emergency_stop = false;
				}
				ImGui::End();
				this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
			}
			this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
		}
		//21�Ų�λ���̣���4�Ų�λ�ѳ�
		if (ImGui::CollapsingHeader(u8"21�Ų�λ���̣���4�Ų�λ�ѳ�"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind1());
			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					this->showButton(*it, conv, wheel, berth, train, yard, silo);
					if (j != 7)
					{
						ImGui::SameLine();
					}
					it++;
				}
			}
			ImGui::Unindent();
		}
		//�����ǳ�
		if (ImGui::CollapsingHeader(u8"�����ǳ�"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind2());
			for (int i = 0; i < 4; i++)
			{
				this->showButton(*it, conv, wheel, berth, train, yard, silo);
				if (i != 3)
				{
					ImGui::SameLine();
				}
				it++;
			}
			ImGui::Unindent();
		}
		//���Ŷ���
		if (ImGui::CollapsingHeader(u8"���Ŷ���"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind3());
			for (int i = 0; i < 2; i++)
			{
				this->showButton(*it, conv, wheel, berth, train, yard, silo);
				if (i == 0)
				{
					ImGui::SameLine();
				}
				it++;
			}
			ImGui::Unindent();
		}
		//װ��
		if (ImGui::CollapsingHeader(u8"װ��"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind4());
			this->showButton(*it, conv, wheel, berth, train, yard, silo);
			ImGui::Unindent();
		}
		//4�Ų�λ���̣��϶ѳ�
		if (ImGui::CollapsingHeader(u8"4�Ų�λ���̣��϶ѳ�"))
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
					this->showButton(*it, conv, wheel, berth, train, yard, silo);
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
		//װ��
		if (ImGui::CollapsingHeader(u8"װ��"))
		{
			ImGui::Indent();
			std::vector<FlowAttrib>::iterator it = std::find_if(this->flows.begin(), this->flows.end(), Myfind6());
			for (int i = 0; i < 5; i++)
			{
				this->showButton(*it, conv, wheel, berth, train, yard, silo);
				if (i != 4)
				{
					ImGui::SameLine();
				}
				it++;
			}
			ImGui::Unindent();
		}
		//������
		if (ImGui::CollapsingHeader(u8"������"))
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
					this->showButton(*it, conv, wheel, berth, train, yard, silo);
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
			//�ѳ�ֱ������̨
			this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
			bool temp = false;
			temp = silo.check_silo_out(0);
			this->setText(temp);
			if (ImGui::Button(u8"��������Ͳ�ֳ�ú�������"))
			{
				if (temp)
				{
					silo.start_silo_out(FLUX, 0);
				}
				else
				{
					//std::cout << "����Ͳ�ֳ�úʧ��::��ǰͲ��û�д��ú���Ѿ���Ͳ���ڳ�ú" << std::endl;
					message.push_message(u8"����Ͳ�ֳ�úʧ��::��ǰͲ��û�д��ú���Ѿ���Ͳ���ڳ�ú");
				}
			}
			ImGui::SameLine();
			temp = silo.check_silo_out(1);
			this->setText(temp);
			if (ImGui::Button(u8"��������Ͳ�ֳ�ú��������ú"))
			{
				if (temp)
				{
					silo.start_silo_out(FLUX, 1);
				}
				else
				{
					//std::cout << "����Ͳ�ֳ�úʧ��::��ǰͲ��û�л���ú���Ѿ���Ͳ���ڳ�ú" << std::endl;
					message.push_message(u8"����Ͳ�ֳ�úʧ��::��ǰͲ��û�л���ú���Ѿ���Ͳ���ڳ�ú");
				}
			}
			ImGui::SameLine();
			temp = silo.out_check();
			this->setText(temp);
			if (ImGui::Button(u8"��ֹ����Ͳ�ֳ�ú"))
			{
				if (temp)
				{
					silo.silo_end_out();
				}
				else
				{
					//std::cout << "��ֹͲ�ֳ�úʧ��::��ǰͲ��û�����ڳ�ú" << std::endl;
					message.push_message(u8"��ֹͲ�ֳ�úʧ��::��ǰͲ��û�����ڳ�ú");
				}
			}
			this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
			this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
			ImGui::Unindent();
		}
		ImGui::Text("  ");
		ImGui::Unindent();
	}
}

void Flow::train_check(Energy& energy, int end_train_1, int end_train_2, Conveyor& conv, SlewingWheel& wheel, TrainLoader& train, Yard& yard)
{
	if (end_train_1 == 1 || end_train_2 == 1)
	{
		//BC15ֹͣ
		for (std::vector<FlowAttrib>::iterator it = this->flows.begin(); it != this->flows.end(); it++)
		{
			if ((it->flow_idx > 92 && it->flow_idx < 96) && it->flow_state == 3)
			{
				//Ƥ������ɫ
				conv.shutDown(stoi(it->flow_name));
				//���ֻ����ɫ
				wheel.shutDown(it->equipments);
				//װ��¥���ɫ
				train.shutDown(it->equipments);
				//�ѳ�ֹͣ
				yard.yard_end(it->equipments);
				//ת�ȱ�
				energy.hotEnd(it->equipments);
				//��ť���ɫ
				it->flow_state = 1;
			}
		}
	}
	else if (end_train_1 == 2 || end_train_2 == 2)
	{
		//BC33ֹͣ
		for (std::vector<FlowAttrib>::iterator it = this->flows.begin(); it != this->flows.end(); it++)
		{
			if ((it->flow_idx > 95 && it->flow_idx < 98) && it->flow_state == 3)
			{
				//Ƥ������ɫ
				conv.shutDown(stoi(it->flow_name));
				//���ֻ����ɫ
				wheel.shutDown(it->equipments);
				//װ��¥���ɫ
				train.shutDown(it->equipments);
				//�ѳ�ֹͣ
				yard.yard_end(it->equipments);
				//ת�ȱ�
				energy.hotEnd(it->equipments);
				//��ť���ɫ
				it->flow_state = 1;
			}
		}
	}
}

void Flow::stop_yard_flow(Energy& energy, std::string name_wheel, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Silo& silo)
{
	for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
	{
		if (it1->flow_state == 3)
		{
			for (std::vector<std::string>::iterator it2 = it1->equipments.begin(); it2 != it1->equipments.end(); it2++)
			{
				if (*it2 == name_wheel)
				{
					//��ֹ����
					//Ƥ������ɫ
					conv.shutDown(stoi(it1->flow_name));
					//���ֻ����ɫ
					wheel.shutDown(it1->equipments);
					//��λ�豸���ɫ
					berth.unloader_shutDown(it1->equipments);
					//װ��¥���ɫ
					train.shutDown(it1->equipments);
					//Ͳ��ֹͣ
					silo.end_silo(it1->equipments);
					//ת�ȱ�
					energy.hotEnd(it1->equipments);
					it1->flow_state = 1;
					break;
				}
			}
		}
	}
}

void Flow::stop_silo_flow(Energy& energy, Conveyor& conv, SlewingWheel& wheel, Yard& yard)
{
	for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
	{
		if (it1->flow_state == 3)
		{
			std::vector<std::string>::reverse_iterator it2 = it1->equipments.rbegin();
			if (*it2 == "ZW12")
			{
				//Ƥ������ɫ
				conv.shutDown(stoi(it1->flow_name));
				//���ֻ����ɫ
				wheel.shutDown(it1->equipments);
				//�ѳ�ֹͣ
				yard.yard_end(it1->equipments);
				//ת�ȱ�
				energy.hotEnd(it1->equipments);
				it1->flow_state = 1;
				break;
			}
		}
	}
}

void Flow::ship_leave(Energy& energy, int berth_idx, Conveyor& conv, SlewingWheel& wheel, Yard& yard)
{
	//��Ӧ��λ���̿�������
	switch (berth_idx)
	{
	case 0:
		//21��λ
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end() && it1->flow_idx < 62; it1++)
		{
			if (it1->flow_state == 3)
			{
				it1->flow_state = 2;
				//Ƥ������ɫ
				conv.shutDown(stoi(it1->flow_name));
				//���ֻ����ɫ
				wheel.shutDown(it1->equipments);
				//�ѳ�ֹͣ
				yard.yard_end(it1->equipments);
				//ת�ȱ�
				energy.hotEnd(it1->equipments);
			}
		}
		break;
	case 1:
		//22��λ
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end() && it1->flow_idx < 62; it1++)
		{
			if (it1->flow_state == 3)
			{
				it1->flow_state = 2;
				//Ƥ������ɫ
				conv.shutDown(stoi(it1->flow_name));
				//���ֻ����ɫ
				wheel.shutDown(it1->equipments);
				//�ѳ�ֹͣ
				yard.yard_end(it1->equipments);
				//ת�ȱ�
				energy.hotEnd(it1->equipments);
			}
		}
		break;
	case 2:
		//4�Ҳ�λ
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state == 3 && ((it1->flow_idx > 62 && it1->flow_idx < 79) || (it1->flow_idx > 86 && it1->flow_idx < 91)))
			{
				it1->flow_state = 2;
				//Ƥ������ɫ
				conv.shutDown(stoi(it1->flow_name));
				//���ֻ����ɫ
				wheel.shutDown(it1->equipments);
				//�ѳ�ֹͣ
				yard.yard_end(it1->equipments);
				//ת�ȱ�
				energy.hotEnd(it1->equipments);
			}
		}
		break;
	case 3:
		//4��λ
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state == 3 && ((it1->flow_idx > 62 && it1->flow_idx < 79) || (it1->flow_idx > 86 && it1->flow_idx < 91)))
			{
				it1->flow_state = 2;
				//Ƥ������ɫ
				conv.shutDown(stoi(it1->flow_name));
				//���ֻ����ɫ
				wheel.shutDown(it1->equipments);
				//�ѳ�ֹͣ
				yard.yard_end(it1->equipments);
				//ת�ȱ�
				energy.hotEnd(it1->equipments);
			}
		}
		break;
	case 4:
		//23��λ
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state == 3 && it1->flow_idx == 62)
			{
				it1->flow_state = 2;
				//Ƥ������ɫ
				conv.shutDown(stoi(it1->flow_name));
				//ת�ȱ�
				energy.hotEnd(it1->equipments);
			}
		}
		break;
	default:
		break;
	}
}

void Flow::end_shiploading(Energy& energy, Conveyor& conv, Berth& berth)
{
	for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
	{
		if (it1->flow_idx == 62)
		{
			it1->flow_state = 1;
			//Ƥ����ǳ��ɫ
			conv.run_unloaded(it1->equipments);
			//��λ�豸���ɫ
			berth.unloader_shutDown(it1->equipments);
			//ת�ȱ�
			energy.hotEnd(it1->equipments);
			break;
		}
	}
}

void Flow::end_shipunloading(Message& message, Energy& energy, int berth_finished, Conveyor& conv, SlewingWheel& wheel, Berth& berth, Yard& yard)
{
	if (berth_finished < 4)
	{
		//AƤ������ͣ��
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state == 3 && it1->equipments.begin()[4] == "A")
			{
				//Ƥ����ǳ��ɫ
				conv.run_unloaded(it1->equipments);
				//���ֻ����ɫ
				wheel.shutDown(it1->equipments);
				//�ѳ�ֹͣ
				yard.yard_end(it1->equipments);
				//��λ������
				berth.run_unloader_unloaded(message, it1->equipments);
				//ת�ȱ�
				energy.hotEnd(it1->equipments);
				it1->flow_state = 2;
				break;
			}
		}
	}
	else
	{
		//BƤ��ͣ��
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state == 3 && it1->equipments.begin()[4] == "B")
			{
				//Ƥ����ǳ��ɫ
				conv.run_unloaded(it1->equipments);
				//���ֻ����ɫ
				wheel.shutDown(it1->equipments);
				//�ѳ�ֹͣ
				yard.yard_end(it1->equipments);
				//��λ������
				berth.run_unloader_unloaded(message, it1->equipments);
				//ת�ȱ�
				energy.hotEnd(it1->equipments);
				it1->flow_state = 2;
				break;
			}
		}
	}
}

void Flow::trip_end(bool all, std::vector<std::string> equipments, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo)
{
	if (all)
	{
		//��ͣ��
		for (std::vector<FlowAttrib>::iterator it1 = this->flows.begin(); it1 != this->flows.end(); it1++)
		{
			if (it1->flow_state >= 2)
			{
				//Ƥ�����ɫ
				conv.emergency_shutDown(stoi(it1->flow_name), it1->equipments);
				//���ֻ����ɫ
				wheel.shutDown(it1->equipments);
				//��λ�豸���ɫ
				berth.unloader_shutDown(it1->equipments);
				//װ��¥���ɫ
				train.shutDown(it1->equipments);
				//�ѳ�ֹͣ
				yard.yard_end(it1->equipments);
				//Ͳ��ֹͣ
				silo.end_silo(it1->equipments);
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
							//Ƥ�����ɫ
							conv.emergency_shutDown(stoi(it1->flow_name), it1->equipments);
							//���ֻ����ɫ
							wheel.shutDown(it1->equipments);
							//��λ�豸���ɫ
							berth.unloader_shutDown(it1->equipments);
							//װ��¥���ɫ
							train.shutDown(it1->equipments);
							//�ѳ�ֹͣ
							yard.yard_end(it1->equipments);
							//Ͳ��ֹͣ
							silo.end_silo(it1->equipments);
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

void Flow::showButton(FlowAttrib& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo)
{
	//����״̬��0��ɫδ���ã�1��ɫ���̱�ѡ�еȴ��ֳ�������2��ɫ��������
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
	if (ImGui::Button((flow.flow_name + u8" ����").c_str()))
	{
		this->pressButton(flow, conv, wheel, berth, train, yard, silo);
	}
	this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}

void Flow::pressButton(FlowAttrib& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo)
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
	}
}
