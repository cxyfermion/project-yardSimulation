#include "Energy.h"

#define POW_INCREASING 0.001f			//״̬�л����������ٶ�

Energy::Energy()
{
	this->total_energy = 0.0f;
	this->trip = -1;
	//��ѹ�����ݣ����ƣ��ͺţ�����
	std::string info_transformer[8];
	info_transformer[0] = "center,SCB10-800/10,80000";
	info_transformer[1] = "1#,SCB10-500/10,50000";
	info_transformer[2] = "2#,SCB10-400/10,40000";
	info_transformer[3] = "3#,SCB10-800/10,80000";
	info_transformer[4] = "4#,SCB10-400/10,40000";
	info_transformer[5] = "5#,SCB10-500/10,50000";
	info_transformer[6] = "6#,SCB10-1000/10,100000";
	info_transformer[7] = "8#,SCB10-800/10,80000";
	//info_transformer[0] = "center,SCB10-800/10,800";
	//info_transformer[1] = "1#,SCB10-500/10,500";
	//info_transformer[2] = "2#,SCB10-400/10,400";
	//info_transformer[3] = "3#,SCB10-800/10,800";
	//info_transformer[4] = "4#,SCB10-400/10,400";
	//info_transformer[5] = "5#,SCB10-500/10,500";
	//info_transformer[6] = "6#,SCB10-1000/10,1000";
	//info_transformer[7] = "8#,SCB10-800/10,800";
	//���ع����ݣ�������ѹ�����ͺţ���ȣ��豸������
	std::string info_gear[153];
	info_gear[0] = "center,KYN-28,4,BC1A,185";
	info_gear[1] = "center,XGN15-12,10,BC2A,450";
	info_gear[2] = "center,XGN15-12,4,BC4A,200";
	info_gear[3] = "center,XGN15-12,4,BC6A,185";
	info_gear[4] = "center,KYN-28,10,BC7,220";
	info_gear[5] = "center,XGN15-12,4,BC19,200";
	info_gear[6] = "center,KYN-28,4,BC6B,185";
	info_gear[7] = "center,KYN-28,4,BC4B,200";
	info_gear[8] = "center,KYN-28,10,BC2B,450";
	info_gear[9] = "center,KYN-28,4,BC1B,185";
	//���100Ϊ��ѹ���
	info_gear[10] = "center,KYN-28,100,BC3A,110";
	info_gear[11] = "center,KYN-28,100,BC3B,110";
	info_gear[12] = "center,KYN-28,100,BC5,110";
	info_gear[13] = "center,KYN-28,15,11#,595";
	info_gear[14] = "center,KYN-28,15,12#,595";
	info_gear[15] = "center,KYN-28,15,13#,595";
	info_gear[16] = "center,KYN-28,15,14#,595";
	info_gear[17] = "center,KYN-28,10,SR6,400";
	info_gear[18] = "center,KYN-28,10,SR7,400";
	info_gear[19] = "center,KYN-28,10,SR8,400";
	info_gear[20] = "center,KYN-28,10,SR9,400";
	info_gear[21] = "center,KYN-28,60,SR10,400";
	info_gear[22] = "center,KYN-28,240,@1#����,0";
	info_gear[23] = "center,KYN-28,@PT,@PT����,0";
	info_gear[24] = "center,KYN-28,20,@5#��䣨���ã�,0";
	info_gear[25] = "center,KYN-28,60,@����B��,0";
	info_gear[26] = "center,KYN-28,60,@����,0";
	info_gear[27] = "center,KYN-28,40,@1#�ֱ�,0";
	info_gear[28] = "center,KYN-28,15,@2#�ֱ�,0";
	info_gear[29] = "center,KYN-28,15,@����,0";
	info_gear[30] = "center,KYN-28,15,@������ѹ��,0";
	info_gear[31] = "center,KYN-28,15,@1���޹�������,0";
	info_gear[32] = "center,KYN-28,@50KVA,@���ñ�,0";
	info_gear[33] = "center,KYN-28,240,@�ֶ�����,0";
	info_gear[34] = "center,KYN-28,@50KVA,@���ñ�,0";
	info_gear[35] = "center,KYN-28,@PT,@PT����,0";
	info_gear[36] = "center,KYN-28,10,@����,0";
	info_gear[37] = "center,KYN-28,15,@2���޹�������,0";
	info_gear[38] = "center,KYN-28,15,@2#�ֱ�,0";
	info_gear[39] = "center,KYN-28,40,@1#�ֱ�,0";
	info_gear[40] = "center,KYN-28,80,@7#��8#��λ,0";
	info_gear[41] = "center,KYN-28,20,@6#�ֱ�,0";
	info_gear[42] = "center,KYN-28,20,@6#�ֱ�,0";
	info_gear[43] = "center,KYN-28,@PT,@PT����,0";
	info_gear[44] = "center,KYN-28,240,@2#����,0";
	info_gear[45] = "center,KYN-28,120,@3#�ֱ�,0";
	info_gear[46] = "center,KYN-28,120,@1��3#�ֱ�,0";
	info_gear[47] = "1#,KYN-28,6,BC12,280";
	info_gear[48] = "1#,KYN-28,6,BC12,280";
	info_gear[49] = "1#,KYN-28,6,BC8,280";
	info_gear[50] = "1#,KYN-28,6,BC8,280";
	info_gear[51] = "1#,KYN-28,6,BC9,280";
	info_gear[52] = "1#,KYN-28,6,BC9,280";
	info_gear[53] = "1#,KYN-28,6,BC10,280";
	info_gear[54] = "1#,KYN-28,6,BC10,280";
	info_gear[55] = "1#,KYN-28,6,BC11,280";
	info_gear[56] = "1#,KYN-28,6,BC11,280";
	info_gear[57] = "1#,KYN-28,6,BC13,250";
	info_gear[58] = "1#,KYN-28,6,BC17,250";
	info_gear[59] = "1#,KYN-28,15,BC18A,250";
	info_gear[60] = "1#,KYN-28,6,BC18B,250";
	info_gear[61] = "1#,KYN-28,20,@1#����,0";
	info_gear[62] = "1#,KYN-28,@PT,@PT����,0";
	info_gear[63] = "1#,KYN-28,6,@����,0";
	info_gear[64] = "1#,KYN-28,40,@�ֶ�����,0";
	info_gear[65] = "1#,KYN-28,@12KV,@�ֶ�����,0";
	info_gear[66] = "1#,KYN-28,8,@23#��λʩ��,0";
	info_gear[67] = "1#,KYN-28,8,@������ѹ��,0";
	info_gear[68] = "1#,KYN-28,@PT,@PT����,0";
	info_gear[69] = "1#,KYN-28,20,@2#����,0";
	info_gear[70] = "2#,KYN-28,6,BC14,280";
	info_gear[71] = "2#,KYN-28,6,BC15,250";
	info_gear[72] = "2#,KYN-28,80,building_A,400";
	info_gear[73] = "2#,KYN-28,80,building_B,400";

	info_gear[74] = "2#,KYN-28,15,@1#����,0";
	info_gear[75] = "2#,KYN-28,@PT,@PT����,0";
	info_gear[76] = "2#,KYN-28,6,@������ѹ��,0";
	info_gear[77] = "2#,KYN-28,6,@����,0";
	info_gear[78] = "2#,KYN-28,15,@�ֶ�����,0";
	info_gear[79] = "2#,KYN-28,8,@����,0";
	info_gear[80] = "2#,KYN-28,6,@������ѹ��,0";
	info_gear[81] = "2#,KYN-28,@PT,@PT����,0";
	info_gear[82] = "2#,KYN-28,15,@2#����,0";
	info_gear[83] = "3#,KYN-28,6,BC22A,250";
	info_gear[84] = "3#,KYN-28,6,BC22A,250";
	info_gear[85] = "3#,KYN-28,6,BC25A,250";
	info_gear[86] = "3#,KYN-28,6,BC25A,250";
	info_gear[87] = "3#,KYN-28,6,BC26,200";
	info_gear[88] = "3#,KYN-28,8,BC31,355";
	info_gear[89] = "3#,KYN-28,6,BC32,250";
	info_gear[90] = "3#,KYN-28,6,BC33,185";
	info_gear[91] = "3#,KYN-28,6,BC22B,250";
	info_gear[92] = "3#,KYN-28,6,BC22B,250";
	info_gear[93] = "3#,KYN-28,8,BC23A,355";
	info_gear[94] = "3#,KYN-28,8,BC23B,355";
	info_gear[95] = "3#,KYN-28,8,BC24A,355";
	info_gear[96] = "3#,KYN-28,8,BC24B,355";
	info_gear[97] = "3#,KYN-28,6,BC25B,250";
	info_gear[98] = "3#,KYN-28,6,BC25B,250";
	info_gear[99] = "3#,KYN-28,6,BC30A,185";
	info_gear[100] = "3#,KYN-28,8,SR11,800";
	info_gear[101] = "3#,KYN-28,6,BC30B,200";
	info_gear[102] = "3#,KYN-28,6,BC36,200";
	info_gear[103] = "3#,KYN-28,8,BC34,315";
	//YDΪ��ѹ��������80
	info_gear[104] = "3#,KYN-28,80,YD1,75";
	info_gear[105] = "3#,KYN-28,80,YD2,75";
	info_gear[106] = "3#,KYN-28,120,@1#����,0";
	info_gear[107] = "3#,KYN-28,@PT,@PT����,0";
	info_gear[108] = "3#,KYN-28,15,@��ѹ��1,0";
	info_gear[109] = "3#,KYN-28,15,@1���޹�����,0";
	info_gear[110] = "3#,KYN-28,80,@4#�ֱ�,0";
	info_gear[111] = "3#,KYN-28,6,@����,0";
	info_gear[112] = "3#,KYN-28,120,@�ֶ�����,0";
	info_gear[113] = "3#,KYN-28,15,@����,0";
	info_gear[114] = "3#,KYN-28,15,@8#�ֱ�,0";
	info_gear[115] = "3#,KYN-28,@12KV,@���ñ�,0";
	info_gear[116] = "3#,KYN-28,8,@4#�ֱ�,0";
	info_gear[117] = "3#,KYN-28,15,@2���޹�����,0";
	info_gear[118] = "3#,KYN-28,@PT,@PT����,0";
	info_gear[119] = "3#,KYN-28,120,@2#����,0";
	info_gear[120] = "4#,KYN-28,30,4#,1915";
	info_gear[121] = "4#,KYN-28,30,3#,1915";
	info_gear[122] = "4#,KYN-28,6,BC21A,200";
	info_gear[123] = "4#,KYN-28,6,BC21B,200";
	info_gear[124] = "4#,KYN-28,6,BC35,220";
	info_gear[125] = "4#,KYN-28,8,loader,334";
	info_gear[126] = "4#,KYN-28,8,BC20A,355";
	info_gear[127] = "4#,KYN-28,8,BC20B,355";
	info_gear[128] = "4#,KYN-28,30,2#,1915";
	info_gear[129] = "4#,KYN-28,30,1#,1915";
	info_gear[130] = "4#,KYN-28,80,@1#����,0";
	info_gear[131] = "4#,KYN-28,@PT,@PT����,0";
	info_gear[132] = "4#,KYN-28,8,@��ѹ��,0";
	info_gear[133] = "4#,KYN-28,6,@����,0";
	info_gear[134] = "4#,KYN-28,80,@�ֶ�����,0";
	info_gear[135] = "4#,KYN-28,8,@��ѹ��,0";
	info_gear[136] = "4#,KYN-28,30,@����,0";
	info_gear[137] = "4#,KYN-28,@PT,@PT����,0";
	info_gear[138] = "4#,KYN-28,80,@2#����,0";
	info_gear[139] = "5#,JGN15-12,15,@������,0";
	info_gear[140] = "5#,JGN15-12,15,@������,0";
	info_gear[141] = "6#,KYN-28,10,SR12,280";
	info_gear[142] = "6#,KYN-28,15,S12,420";
	info_gear[143] = "6#,KYN-28,50,XC1,110";
	info_gear[144] = "6#,KYN-28,50,XC2,355";
	//ZWƤ�������������ڱ�ϵͳ�л������
	info_gear[145] = "6#,KYN-28,6,ZW11,280";
	info_gear[146] = "6#,KYN-28,6,ZW12,75";
	info_gear[147] = "6#,KYN-28,30,@���߹�,0";
	info_gear[148] = "6#,KYN-28,@PT,@PT����,0";
	info_gear[149] = "6#,KYN-28,15,@��ѹ��,0";
	info_gear[150] = "6#,KYN-28,10,@����,0";
	info_gear[151] = "6#,KYN-28,4,@����,0";
	info_gear[152] = "6#,KYN-28,4,@����,0";
	//��ѹ�����ݶ�ȡ
	int last_pos_1 = 0;
	int last_pos_2 = 0;
	this->transformer.transformer_provider = u8"����Ƽ�";
	this->transformer.transformer_idx = 0;
	this->transformer.cur_pow = 0.0f;
	this->transformer.cur_energy = 0.0f;
	int index_gear = 0;
	std::string temp;
	for (int i = 0; i < 8; i++)
	{
		last_pos_1 = 0;
		this->transformer.transformer_name = info_transformer[i].substr(0, info_transformer[i].find(',', 0));
		last_pos_1 = (int)(info_transformer[i].find(',', 0)) + 1;
		this->transformer.transformer_type = info_transformer[i].substr(last_pos_1, info_transformer[i].find(',', last_pos_1) - last_pos_1);
		last_pos_1 = (int)(info_transformer[i].find(',', last_pos_1)) + 1;
		this->transformer.max_pow = stoi(info_transformer[i].substr(last_pos_1, info_transformer[i].size() - last_pos_1));
		//���ع����ݶ�ȡ
		this->transformer.gears.clear();
		this->gear.gear_provider = u8"��ͨ";
		this->gear.pow = 0;
		this->gear.gear_idx = 0;
		this->gear.state = 0;
		this->gear.time = 0;
		this->gear.pow_rate = 1.0f;
		while (index_gear <= 152 && info_gear[index_gear].substr(0, info_gear[index_gear].find(',', 0)) == this->transformer.transformer_name)
		{
			last_pos_2 = 0;
			this->gear.gear_transformer = info_gear[index_gear].substr(0, info_gear[index_gear].find(',', 0));
			last_pos_2 = (int)(info_gear[index_gear].find(',', 0)) + 1;
			this->gear.gear_type = info_gear[index_gear].substr(last_pos_2, info_gear[index_gear].find(',', last_pos_2) - last_pos_2);
			last_pos_2 = (int)(info_gear[index_gear].find(',', last_pos_2)) + 1;
			//��ȶ�ȡ
			temp = info_gear[index_gear].substr(last_pos_2, info_gear[index_gear].find(',', last_pos_2) - last_pos_2);
			if (*temp.begin() == '@')
			{
				if (temp == "@PT")
				{
					//PT
					this->gear.transformation_ratio = 0;
				}
				else
				{
					//KV
					this->gear.transformation_ratio = -stoi(temp.substr(1, 2));
				}
			}
			else
			{
				this->gear.transformation_ratio = stoi(temp);
			}
			last_pos_2 = (int)(info_gear[index_gear].find(',', last_pos_2)) + 1;
			//�豸��ȡ
			this->gear.equipment = info_gear[index_gear].substr(last_pos_2, info_gear[index_gear].find(',', last_pos_2) - last_pos_2);
			last_pos_2 = (int)(info_gear[index_gear].find(',', last_pos_2)) + 1;
			//���ʶ�ȡ
			this->gear.pow = stoi(info_gear[index_gear].substr(last_pos_2, info_gear[index_gear].size() - last_pos_2));
			this->transformer.gears.push_back(this->gear);
			this->gear.gear_idx++;
			index_gear++;
		}
		this->transformers.push_back(this->transformer);
		this->transformer.transformer_idx++;
	}
}

void Energy::reset()
{
	this->total_energy = 0.0f;
	this->trip = -1;
	for (std::vector<Transformer>::iterator it1 = this->transformers.begin(); it1 != this->transformers.end(); it1++)
	{
		it1->cur_pow = 0.0f;
		it1->cur_energy = 0.0f;
		for (std::vector<switchGear>::iterator it2 = it1->gears.begin(); it2 != it1->gears.end(); it2++)
		{
			it2->state = 0;
			it2->time = 0;
		}
	}
}

void Energy::update(Message& message, float simurate, float gapTime)
{
	for (std::vector<Transformer>::iterator it1 = this->transformers.begin(); it1 != this->transformers.end(); it1++)
	{
		//it1->cur_pow = 0.0f;
		for (std::vector<switchGear>::iterator it2 = it1->gears.begin(); it2 != it1->gears.end(); it2++)
		{
			if (it2->state == 2)
			{
				//����״̬
				it2->time += (float)(simurate * gapTime);
				//ע��@�Ĵ���
				if (*it2->equipment.begin() != '@')
				{
					if (it2->pow_rate < 1.0f)
					{
						it2->pow_rate += POW_INCREASING * simurate * gapTime;
						it1->cur_pow += it2->pow * POW_INCREASING * simurate * gapTime;
					}
					else if (it2->pow_rate > 1.0f)
					{
						it2->pow_rate = 1.0f;
					}
					it1->cur_energy += (float)(simurate * gapTime) * it2->pow * it2->transformation_ratio * 0.001f;
				}
			}
		}
		if (it1->cur_pow > it1->max_pow)
		{
			//��ǰ���ʳ�����ѹ���������ޣ���ѹ����բ���뵱ǰ��ѹ�������������豸ȫ���ϵ磬�������ı���բ��ȫ��ͷ�ϵ�
			this->trip = it1->transformer_idx;
			if (it1->transformer_idx == 0)
			{
				message.push_message(u8"���ı���բ");
				//���ı�ϵ磬��ͷ�ϵ�
				for (std::vector<Transformer>::iterator it3 = this->transformers.begin(); it3 != this->transformers.end(); it3++)
				{
					it3->cur_pow = 0;
					for (std::vector<switchGear>::iterator it2 = it1->gears.begin(); it2 != it1->gears.end(); it2++)
					{
						it2->state = 0;
					}
				}
			}
			else
			{
				message.push_message(it1->transformer_name + u8"��ѹ����բ");
				for (std::vector<switchGear>::iterator it2 = it1->gears.begin(); it2 != it1->gears.end(); it2++)
				{
					it2->state = 0;
				}
			}
			break;
		}
	}
}

void Energy::coldStart(std::vector<std::string>& equipments)
{
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<Transformer>::iterator it2 = this->transformers.begin(); it2 != this->transformers.end(); it2++)
		{
			for (std::vector<switchGear>::iterator it3 = it2->gears.begin(); it3 != it2->gears.end(); it3++)
			{
				//ֻ���䱸״̬���л����ȱ�״̬
				if (it3->state == 0)
				{
					if (*it1 == it3->equipment)
					{
						it3->state = 1;
					}
					if (*it1 == "BC35")
					{
						//װ����
						if (it3->equipment == "loader")
						{
							it3->state = 1;
						}
					}
					else if (*it1 == "BC20B" || *it1 == "BC20A")
					{
						//ͨ�ò�λж��������ǰ����һ����λ��ʹȫ��4̨ж��������������״̬��������ʾ�����е�ж����û��������
						if (it3->equipment == "1#" || it3->equipment == "2#" || it3->equipment == "3#" || it3->equipment == "4#")
						{
							it3->state = 1;
						}
					}
					else if (*it1 == "BC1B" || *it1 == "BC1A")
					{
						//4��λж����
						if (it3->equipment == "11#" || it3->equipment == "12#" || it3->equipment == "13#" || it3->equipment == "14#")
						{
							it3->state = 1;
						}
					}
				}
			}
		}
	}
}

void Energy::hotStart(std::vector<std::string>& equipments)
{
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<Transformer>::iterator it2 = this->transformers.begin(); it2 != this->transformers.end(); it2++)
		{
			for (std::vector<switchGear>::iterator it3 = it2->gears.begin(); it3 != it2->gears.end(); it3++)
			{
				//ֻ���ȱ�״̬���л�������״̬
				if (it3->state == 1)
				{
					if (*it1 == it3->equipment)
					{
						it3->state = 2;
						it3->pow_rate = 0.7f;
						it2->cur_pow += it3->pow * it3->pow_rate;
					}
					if (*it1 == "BC35")
					{
						//װ����
						if (it3->equipment == "loader")
						{
							it3->state = 2;
							it3->pow_rate = 0.7f;
							it2->cur_pow += it3->pow * it3->pow_rate;
						}
					}
					else if (*it1 == "BC20B" || *it1 == "BC20A")
					{
						//ͨ�ò�λж��������ǰ����һ����λ��ʹȫ��4̨ж��������������״̬��������ʾ�����е�ж����û��������
						if (it3->equipment == "1#" || it3->equipment == "2#" || it3->equipment == "3#" || it3->equipment == "4#")
						{
							it3->state = 2;
							it3->pow_rate = 0.7f;
							it2->cur_pow += it3->pow * it3->pow_rate;
						}
					}
					else if (*it1 == "BC1B" || *it1 == "BC1A")
					{
						//4��λж����
						if (it3->equipment == "11#" || it3->equipment == "12#" || it3->equipment == "13#" || it3->equipment == "14#")
						{
							it3->state = 2;
							it3->pow_rate = 0.7f;
							it2->cur_pow += it3->pow * it3->pow_rate;
						}
					}
				}
			}
		}
	}
}

void Energy::hotEnd(std::vector<std::string>& equipments)
{
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<Transformer>::iterator it2 = this->transformers.begin(); it2 != this->transformers.end(); it2++)
		{
			for (std::vector<switchGear>::iterator it3 = it2->gears.begin(); it3 != it2->gears.end(); it3++)
			{
				//ֻ������״̬���л����ȱ�״̬
				if (it3->state == 2)
				{
					if (*it1 == it3->equipment)
					{
						it3->state = 1;
						it2->cur_pow -= it3->pow * it3->pow_rate;
					}
					if (*it1 == "BC35")
					{
						//װ����
						if (it3->equipment == "loader")
						{
							it3->state = 1;
							it2->cur_pow -= it3->pow * it3->pow_rate;
						}
					}
					else if (*it1 == "BC20B" || *it1 == "BC20A")
					{
						//ͨ�ò�λж����
						if (it3->equipment == "1#" || it3->equipment == "2#" || it3->equipment == "3#" || it3->equipment == "4#")
						{
							it3->state = 1;
							it2->cur_pow -= it3->pow * it3->pow_rate;
						}
					}
					else if (*it1 == "BC1B" || *it1 == "BC1A")
					{
						//4��λж����
						if (it3->equipment == "11#" || it3->equipment == "12#" || it3->equipment == "13#" || it3->equipment == "14#")
						{
							it3->state = 1;
							it2->cur_pow -= it3->pow * it3->pow_rate;
						}
					}
				}
			}
		}
	}
}

void Energy::coldEnd(std::vector<std::string>& equipments)
{
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<Transformer>::iterator it2 = this->transformers.begin(); it2 != this->transformers.end(); it2++)
		{
			for (std::vector<switchGear>::iterator it3 = it2->gears.begin(); it3 != it2->gears.end(); it3++)
			{
				//ֻ���ȱ�״̬���л����䱸״̬
				if (it3->state == 1)
				{
					if (*it1 == it3->equipment)
					{
						it3->state = 0;
					}
					if (*it1 == "BC35")
					{
						//װ����
						if (it3->equipment == "loader")
						{
							it3->state = 0;
						}
					}
					else if (*it1 == "BC20B" || *it1 == "BC20A")
					{
						//ͨ�ò�λж����
						if (it3->equipment == "1#" || it3->equipment == "2#" || it3->equipment == "3#" || it3->equipment == "4#")
						{
							it3->state = 0;
						}
					}
					else if (*it1 == "BC1B" || *it1 == "BC1A")
					{
						//4��λж����
						if (it3->equipment == "11#" || it3->equipment == "12#" || it3->equipment == "13#" || it3->equipment == "14#")
						{
							it3->state = 0;
						}
					}
				}
			}
		}
	}
}

void Energy::emergencyEnd(std::vector<std::string>& equipments)
{
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<Transformer>::iterator it2 = this->transformers.begin(); it2 != this->transformers.end(); it2++)
		{
			for (std::vector<switchGear>::iterator it3 = it2->gears.begin(); it3 != it2->gears.end(); it3++)
			{
				if (*it1 == it3->equipment)
				{
					it3->state = 0;
				}
				if (*it1 == "BC35")
				{
					//װ����
					if (it3->equipment == "loader")
					{
						it3->state = 0;
					}
				}
				else if (*it1 == "BC20B" || *it1 == "BC20A")
				{
					//ͨ�ò�λж����
					if (it3->equipment == "1#" || it3->equipment == "2#" || it3->equipment == "3#" || it3->equipment == "4#")
					{
						it3->state = 0;
					}
				}
				else if (*it1 == "BC1B" || *it1 == "BC1A")
				{
					//4��λж����
					if (it3->equipment == "11#" || it3->equipment == "12#" || it3->equipment == "13#" || it3->equipment == "14#")
					{
						it3->state = 0;
					}
				}
			}
		}
	}
}

std::vector<std::string> Energy::getEquipments()
{
	std::vector<std::string> ret;
	for (std::vector<Transformer>::const_iterator it1 = this->transformers.begin(); it1 != this->transformers.end(); it1++)
	{
		if (it1->transformer_idx == this->trip)
		{
			for (std::vector<switchGear>::const_iterator it2 = it1->gears.begin(); it2 != it1->gears.end(); it2++)
			{
				if (*it2->equipment.begin() != '@')
				{
					ret.push_back(it2->equipment);
				}
			}
			break;
		}
	}
	return ret;
}

void Energy::initGuiStyle()
{
	this->style = &ImGui::GetStyle();
}

void Energy::transformerDispatch()
{
	if (ImGui::CollapsingHeader(u8"�����"))
	{
		ImGui::Indent();
		static int choose = -1;		//ѡ���ı��վ���
		std::string temp = "";
		ImGui::Text((u8"���ܺģ�" + std::to_string(this->total_energy)).c_str());
		for (std::vector<Transformer>::const_iterator it1 = this->transformers.begin(); it1 != this->transformers.end(); it1++)
		{
			if (it1->transformer_name == "center")
			{
				ImGui::Text(u8"���ı�");
			}
			else
			{
				ImGui::Text((it1->transformer_name + u8"�ֱ�").c_str());
			}
			ImGui::SameLine();
			ImGui::Text((u8"	�ͺţ�" + it1->transformer_type).c_str());
			ImGui::SameLine();
			ImGui::Text((u8"	�����̣�" + it1->transformer_provider).c_str());
			ImGui::Text((u8"��ǰ���ʣ�" + std::to_string(it1->cur_pow)).c_str());
			ImGui::SameLine();
			ImGui::Text((u8"	��߹��ʣ�" + std::to_string(it1->max_pow)).c_str());
			ImGui::SameLine();
			ImGui::Text((u8"��ѹ����ǰ�ܺģ�" + std::to_string(it1->cur_energy)).c_str());
			this->pre_button(it1->transformer_idx == choose);
			if (it1->transformer_idx == 0)
			{
				temp = u8"���ı�";
			}
			else
			{
				temp = it1->transformer_name + u8"�ֱ�";
			}
			if (ImGui::Button((u8"�鿴" + temp + u8"�Ŀ��ع�").c_str()))
			{
				if (it1->transformer_idx == choose)
				{
					choose = -1;
				}
				else
				{
					choose = it1->transformer_idx;
				}
			}
			this->post_button();
			if (it1->transformer_idx == choose)
			{
				for (std::vector<switchGear>::const_iterator it2 = it1->gears.begin(); it2 != it1->gears.end(); it2++)
				{
					ImGui::Separator();
					ImGui::Text(u8"���ع�");
					ImGui::Text((u8"�ͺţ�" + it2->gear_type).c_str());
					ImGui::SameLine();
					ImGui::Text((u8"		�����̣�" + it2->gear_provider).c_str());
					//@����
					if (*it2->equipment.begin() == '@')
					{
						if (it2->equipment == "@PT����")
						{
							ImGui::Text(u8"�豸��;��PT����");
						}
						if (it2->equipment == "@5#��䣨���ã�")
						{
							ImGui::Text(u8"�豸��;��5#��䣨���ã�");
						}
						if (it2->equipment == "@����B��")
						{
							ImGui::Text(u8"�豸��;������B��");
						}
						if (it2->equipment == "@7#��8#��λ")
						{
							ImGui::Text(u8"�豸��;��7#��8#��λ");
						}
						if (it2->equipment == "@23#��λʩ��")
						{
							ImGui::Text(u8"�豸��;��23#��λʩ��");
						}
						else if (it2->equipment == "@����")
						{
							ImGui::Text(u8"�豸��;������");
						}
						else if (it2->equipment == "@1���޹�������")
						{
							ImGui::Text(u8"�豸��;��1���޹�������");
						}
						else if (it2->equipment == "@2���޹�������")
						{
							ImGui::Text(u8"�豸��;��2���޹�������");
						}
						else if (it2->equipment == "@������ѹ��")
						{
							ImGui::Text(u8"�豸��;��������ѹ��");
						}
						else if (it2->equipment == "@��ѹ��")
						{
							ImGui::Text(u8"�豸��;����ѹ��");
						}
						else if (it2->equipment == "@���ñ�")
						{
							ImGui::Text(u8"�豸��;�����ñ�");
						}
						else if (it2->equipment == "@������")
						{
							ImGui::Text(u8"�豸��;��������");
						}
						else if (it2->equipment == "@���߹�")
						{
							ImGui::Text(u8"�豸��;�����߹�");
						}
						else if (it2->equipment == "@�ֶ�����")
						{
							ImGui::Text(u8"�豸��;���ֶ�����");
						}
						else if (it2->equipment == "@1#����")
						{
							ImGui::Text(u8"�豸��;��1#����");
						}
						else if (it2->equipment == "@2#����")
						{
							ImGui::Text(u8"�豸��;��2#����");
						}
						else if (it2->equipment == "@1#�ֱ�")
						{
							ImGui::Text(u8"�豸��;��1#�ֱ�");
						}
						else if (it2->equipment == "@2#�ֱ�")
						{
							ImGui::Text(u8"�豸��;��2#�ֱ�");
						}
						else if (it2->equipment == "@3#�ֱ�")
						{
							ImGui::Text(u8"�豸��;��3#�ֱ�");
						}
						else if (it2->equipment == "@1��3#�ֱ�")
						{
							ImGui::Text(u8"�豸��;��1��3#�ֱ�");
						}
						else if (it2->equipment == "@4#�ֱ�")
						{
							ImGui::Text(u8"�豸��;��4#�ֱ�");
						}
						else if (it2->equipment == "@6#�ֱ�")
						{
							ImGui::Text(u8"�豸��;��6#�ֱ�");
						}
						else if (it2->equipment == "@7#�ֱ�")
						{
							ImGui::Text(u8"�豸��;��7#�ֱ�");
						}
						else if (it2->equipment == "@8#�ֱ�")
						{
							ImGui::Text(u8"�豸��;��8#�ֱ�");
						}
					}
					else
					{
						if (it2->equipment == "loader")
						{
							ImGui::Text(u8"���Ƶ��豸��װ����");
						}
						if (it2->equipment == "building_A")
						{
							ImGui::Text(u8"���Ƶ��豸��װ��¥2");
						}
						if (it2->equipment == "building_B")
						{
							ImGui::Text(u8"���Ƶ��豸��װ��¥1");
						}
						else if (it2->equipment[1] == '#' || it2->equipment[2] == '#')
						{
							ImGui::Text((u8"���Ƶ��豸��" + it2->equipment + u8"ж����").c_str());
						}
						else if (it2->equipment[0] == 'S')
						{
							if (it2->equipment[1] == '1')
							{
								ImGui::Text((u8"���Ƶ��豸��" + it2->equipment + u8"Բ�ѻ�").c_str());
							}
							else
							{
								ImGui::Text((u8"���Ƶ��豸��" + it2->equipment + u8"���ֻ�").c_str());
							}
						}
						else
						{
							ImGui::Text((u8"���Ƶ��豸��" + it2->equipment).c_str());
						}
					}
					switch (it2->state)
					{
					case 0:
						temp = u8"�䱸״̬";
						break;
					case 1:
						temp = u8"�ȱ�״̬";
						break;
					case 2:
						temp = u8"����״̬";
						break;
					default:
						break;
					}
					ImGui::SameLine();
					ImGui::Text((u8"	״̬��" + temp).c_str());
					ImGui::Text((u8"���й��ʣ�" + std::to_string(it2->pow)).c_str());
					ImGui::SameLine();
					ImGui::Text((u8"		����ʱ����" + std::to_string(it2->time)).c_str());
					//���
					if (it2->transformation_ratio == 0)
					{
						ImGui::Text(u8"��ȣ�PT");
					}
					else if (it2->transformation_ratio < 0)
					{
						ImGui::Text((u8"��ȣ�" + std::to_string(-it2->transformation_ratio) + u8"KV").c_str());
					}
					else
					{
						ImGui::Text((u8"��ȣ�" + std::to_string(it2->transformation_ratio)).c_str());
					}
				}
			}
			ImGui::Separator();
		}
		ImGui::Unindent();
	}
}

void Energy::pre_button(bool choosed)
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

void Energy::post_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}
