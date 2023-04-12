#include "Berth.h"

#define CT_WIDTH 4100
#define CT_HEIGHT 2370
#define SCR_WIDTH 1440
#define SCR_HEIGHT 810
#define MAX_GENBERTH_CAPACITY 14	//ͨ�ò�λ����ܿ�������
#define MAX_START_LAOD 0.9f			//����װ���ĵ�ǰ������ʼ��������
#define FLUX_LAOD 0.0022f				//װ������
#define FLUX_BUCKET 0.001f			//ץ������

#define layer_unloader 0.0055f			//ж����ͼ��
#define layer_bucket 0.0058f			//ץ��ͼ��
#define layer_ship 0.0054f				//��ͼ��
#define unloader_pow 1915			//ж�������й���
#define bucket_speed_hor 0.01f		//ץ��ˮƽ�ƶ��ٶ�
#define bucket_speed_ver 0.01f		//ץ����ֱ�ƶ��ٶ�
#define bucket_speed_up 0.02f			//ץ��Ƥ�����������ٶ�
#define bucket_speed_reset 0.04f		//ץ��ˮƽ��������λ�ٶ�
constexpr auto speed_ship = 0.005f;	//�����ƶ��ٶ�;

Berth::Berth()
{
	this->berth_finished = -1;
	this->unload_mode_gen = false;
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
	//ж������ʼ��
	std::string info_unloader[8];
	info_unloader[0] = "1#,3870,390";
	info_unloader[1] = "2#,3750,390";
	info_unloader[2] = "3#,3630,390";
	info_unloader[3] = "4#,3510,390";
	info_unloader[4] = "11#,1840,1950";
	info_unloader[5] = "12#,1960,1950";
	info_unloader[6] = "13#,2080,1950";
	info_unloader[7] = "14#,2200,1950";
	//��λ����
	const float bias = 30.0f;
	this->berth_pos.x_4_1 = 1900.0f - bias;
	this->berth_pos.x_4_2 = 2140.0f - bias;
	this->berth_pos.x_21 = 3810.0f - bias;
	this->berth_pos.x_22 = 3570.0f - bias;
	this->berth_pos.x_23 = 2870.0f - bias;
	//�Ż�����
	this->unloader.crab_bucket_switch = false;
	this->unloader.crab_bucket_hor = -1.0f;
	this->unloader.crab_bucket_ver = 1.0f;
	this->unloader.unloader_state = 0;	
	this->unloader.crab_bucket_loaded = false;
	//ץ������
	this->bucket_up.ship = 290;
	this->bucket_up.conv_A = 535;
	this->bucket_up.conv_B = 465;
	this->bucket_up.idle = 600;
	this->bucket_down.ship = 2070;
	this->bucket_down.conv_A = 1880;
	this->bucket_down.conv_B = 1805;
	this->bucket_down.idle = 1730;
	//ж������ץ�����ݶ�ȡ
	for (int i = 0; i < 8; i++)
	{
		last_pos = 0;
		this->unloader.unloader_index = i;
		this->unloader.unloader_state = 0;
		this->unloader.unloader_name = info_unloader[i].substr(0, info_unloader[i].find(',', 0));
		last_pos = (int)(info_unloader[i].find(',', 0)) + 1;
		this->unloaders.push_back(this->unloader);
		for (int j = 0; j < 5; j++)
		{
			switch (j)
			{
			case 0:
				//x,x
				this->unloader_coords[5 * i] = stof(info_unloader[i].substr(last_pos, info_unloader[i].find(',', last_pos) - last_pos)) / (CT_WIDTH / 10.0f) - 5.0f;
				last_pos = (int)(info_unloader[i].find(',', last_pos)) + 1;
				this->bucket_coords[5 * i] = this->unloader_coords[5 * i];
				break;
			case 1:
				//y,y
				this->unloader_coords[5 * i + 1] = -stof(info_unloader[i].substr(last_pos, info_unloader[i].find(',', last_pos) - last_pos)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				last_pos = (int)(info_unloader[i].find(',', last_pos)) + 1;
				//��ʼ��������
				if (i < 4)
				{
					this->bucket_coords[5 * i + 1] = -(float)this->bucket_up.idle / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				}
				else
				{
					this->bucket_coords[5 * i + 1] = -(float)this->bucket_down.idle / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				}
				break;
			case 2:
				//z,z
				this->unloader_coords[5 * i + 2] = layer_unloader;
				this->bucket_coords[5 * i + 2] = layer_bucket;
				break;
			case 3:
				//state,ver
				this->unloader_coords[5 * i + 3] = (float)this->unloader.unloader_state;
				this->bucket_coords[5 * i + 3] = 1.0f;
				break;
			case 4:
				//focus,focus
				this->unloader_coords[5 * i + 4] = 0.0f;
				this->bucket_coords[5 * i + 4] = 0.0f;
				break;
			default:
				break;
			}
		}
	}
	/*buffers for ж���� & ץ��*/
	//ж����
	glGenVertexArrays(1, &unloaderVAO);
	glGenBuffers(1, &unloaderVBO);
	glBindVertexArray(unloaderVAO);
	glBindBuffer(GL_ARRAY_BUFFER, unloaderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unloader_coords), &unloader_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//ץ��
	glGenVertexArrays(1, &bucketVAO);
	glGenBuffers(1, &bucketVBO);
	glBindVertexArray(bucketVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bucketVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bucket_coords), &bucket_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//װ������ʼ��
	this->loader.loader_name = "SL4";
	this->loader.loader_state = 0;
	this->loader.loader_pow = 37;
	this->loader_coords[0] = 2870.0f / (CT_WIDTH / 10.0f) - 5.0f;
	this->loader_coords[1] = -390.0f / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
	this->loader_coords[2] = layer_unloader;
	this->loader_coords[3] = (float)this->loader.loader_state;
	this->loader_coords[4] = 0.0f;
	//buffers
	glGenVertexArrays(1, &loaderVAO);
	glGenBuffers(1, &loaderVBO);
	glBindVertexArray(loaderVAO);
	glBindBuffer(GL_ARRAY_BUFFER, loaderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(loader_coords), &loader_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//����ʼ��
	std::string info_ship_1 = u8"����11,����28,����ʢ,Ǯ��66,��ʢ161,����17,����ɽ,�����,��˳7,ʢ��8,����1,����7,����22,����11,����76,����6,����5,����59,����1,����20,����58,����11,����57,����ɽ,����21,Զ��֮��,����108,��ҵ2,����7,����5,��һ��3,����,���ݷ�չ2,����6";
	std::string info_ship_2 = "23500,18000,16100,16200,53671,19350,30000,24150,27300,43466,18593,19300,54052,19950,53656,51730,22440,46800,27000,64827,15739,47718,46100,23000,53669,24990,48922,17000,19300,75000,57000,24310,57300,42000";
	int last_pos_1 = 0;
	int last_pos_2 = 0;
	for (int i = 0; i < 34; i++)
	{
		this->info_ships_name[i] = info_ship_1.substr(last_pos_1, info_ship_1.find(',', last_pos_1) - last_pos_1);
		last_pos_1 = (int)(info_ship_1.find(',', last_pos_1)) + 1;
		this->info_ships_storage[i] = stof(info_ship_2.substr(last_pos_2, info_ship_2.find(',', last_pos_2) - last_pos_2));
		last_pos_2 = (int)(info_ship_2.find(',', last_pos_2)) + 1;
		this->info_ships_occupied[i] = 0;
		if (i < 5)
		{
			this->berth_occupied[i] = 0;
		}
	}
	//21��λ
	this->ship.ship_name = u8"����6";
	this->ship.ship_state = 0;
	this->ship.angle = 0.0;
	this->ship.ship_total_storage = 51730.0;
	this->ship.ship_current_storage = 51730.0;
	this->ship.berth_idx = 0;
	this->ship.ship_type = 1;
	this->ship.ship_index = 4;
	this->ship.ship_dist = 10.0;
	this->ship.wait_pos[0] = 3950.0f;
	this->ship.wait_pos[1] = 40.0f;
	this->ship.ship_coords[0] = 0.0f;
	this->ship.ship_coords[1] = 0.0f;
	this->ship.ship_coords[2] = layer_ship;
	this->ship.ship_coords[3] = 0.0f;
	this->ship.ship_coords[4] = 0.0f;
	this->ship.ship_coords[5] = 0.0f;
	this->ship.ship_coords[6] = 1.5f;
	this->ship.ship_coords[7] = 4.0f;
	this->ships.push_back(this->ship);
	//4��λ
	this->ship.ship_name = u8"����1";
	this->ship.ship_state = 2;
	this->ship.ship_total_storage = 18593.0;
	this->ship.ship_current_storage = 15593.0;
	this->ship.berth_idx = 3;
	this->ship.ship_type = 1;
	this->ship.ship_index = 2;
	this->ship.wait_pos[0] = 2450.0f;
	this->ship.wait_pos[1] = 2260.0f;
	this->ship.ship_coords[0] = this->ship.wait_pos[0] / (CT_WIDTH / 10.0f) - 5.0f;
	this->ship.ship_coords[1] = -this->ship.wait_pos[1] / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
	this->ship.angle = std::atan2((this->ship.wait_pos[1] - this->bucket_down.ship), -(this->ship.wait_pos[0] - this->berth_pos.x_4_1));
	this->ship.ship_coords[3] = 2.0f;
	this->ship.ship_coords[4] = 1.0f - this->ship.ship_current_storage / this->ship.ship_total_storage;
	this->ship.ship_coords[6] = 0.9f;
	this->ship.ship_coords[7] = 2.0f;
	this->ship.ship_dist = hypot((this->ship.ship_coords[0] - (this->berth_pos.x_4_1 / (CT_WIDTH / 10.0f) - 5.0f)), (this->ship.ship_coords[1] - (-this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH))));
	this->ships.push_back(this->ship);
	//22��λ
	this->ship.ship_name = u8"����57";
	this->ship.ship_state = 3;
	this->ship.ship_total_storage = 24700.0;
	this->ship.ship_current_storage = 18300.0;
	this->ship.berth_idx = 1;
	this->ship.angle = std::atan2((6.0f - this->bucket_up.ship), -(2.0f - this->berth_pos.x_22));
	this->ship.ship_type = 2;
	this->ship.ship_index = 5;
	this->ship.ship_dist = 10.0;
	this->ship.ship_coords[0] = this->berth_pos.x_22 / (CT_WIDTH / 10.0f) - 5.0f;
	this->ship.ship_coords[1] = -(float)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
	this->ship.ship_coords[3] = 3.0f;
	this->ship.ship_coords[4] = 1.0f - this->ship.ship_current_storage / this->ship.ship_total_storage;
	this->ship.ship_coords[6] = 1.3f;
	this->ship.ship_coords[7] = 7.0f;
	this->ships.push_back(this->ship);
	//23��λ
	this->ship.ship_name = u8"����5";
	this->ship.ship_state = 3;
	this->ship.ship_total_storage = 5000.0;
	this->ship.ship_current_storage = 0.0;
	this->ship.berth_idx = 4;
	this->ship.angle = std::atan2((1.5f - this->bucket_up.ship), -(1.3f - this->berth_pos.x_22));
	this->ship.ship_type = 3;
	this->ship.ship_index = 3;
	this->ship.ship_coords[0] = this->berth_pos.x_23 / (CT_WIDTH / 10.0f) - 5.0f;
	this->ship.ship_coords[1] = -(float)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
	this->ship.ship_coords[3] = 3.0f;
	this->ship.ship_coords[4] = 1.0f;
	this->ship.ship_coords[6] = 0.7f;
	this->ship.ship_coords[7] = 7.0f;
	this->ships.push_back(this->ship);
	//buffers
	glGenVertexArrays(1, &shipVAO);
	glGenBuffers(1, &shipVBO);
	glBindVertexArray(shipVAO);
	glBindBuffer(GL_ARRAY_BUFFER, shipVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ship.ship_coords), &ship.ship_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void Berth::reset(SimuCore& core, bool rand_init)
{
	//����ж����
	for (std::vector<ShipUnloader>::iterator it1 = this->unloaders.begin(); it1 != this->unloaders.end(); it1++)
	{
		it1->crab_bucket_switch = false;
		it1->crab_bucket_hor = -1.0f;
		it1->crab_bucket_ver = 1.0f;
		it1->unloader_state = 0;
		it1->crab_bucket_loaded = false;
		this->unloader_coords[5 * it1->unloader_index + 3] = 0.0f;
		this->bucket_coords[5 * it1->unloader_index + 3] = 1.0f;
		if (it1->unloader_index < 4)
		{
			this->bucket_coords[5 * it1->unloader_index + 1] = -(float)this->bucket_up.idle / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
		}
		else
		{
			this->bucket_coords[5 * it1->unloader_index + 1] = -(float)this->bucket_down.idle / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
		}
	}
	//����װ����
	this->loader.loader_state = 0;
	//���ô���
	this->ships.clear();
	if (rand_init)
	{
		this->ship_random_initiator(core);
	}
}

void Berth::ship_random_initiator(SimuCore& core)
{
	this->ships.clear();
	for (int i = 0; i < 34; i++)
	{
		this->info_ships_occupied[i] = 0;
		if (i < 5)
		{
			this->berth_occupied[i] = 0;
		}
	}
	//ͳ���������������
	int count_types[3] = { 0,0,0 };
	for (std::vector<Type_ship>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
	{
		count_types[it->type_type - 1]++;
	}
	int num_ships = core.genRandomInt(1, 6);
	int ship_idx = 0;
	while (num_ships > 0)
	{
		//�����ʼ������
		do
		{
			ship_idx = core.genRandomInt(1, 34) - 1;
		} while (this->info_ships_occupied[ship_idx] == 1);
		this->ship.ship_name = this->info_ships_name[ship_idx];
		this->ship.ship_total_storage = this->info_ships_storage[ship_idx];
		this->info_ships_occupied[ship_idx] = 1;
		bool enBerth = true;
		do
		{
			enBerth = true;
			//���ж�����Ĳ�λ�����ݲ�λ����ê��λ�á�����״̬
			this->ship.berth_idx = core.genRandomInt(0, 4);
			//�ж�����״̬����λ���д�����stateΪ2��3��4�߲����ظ���Ϊ2��3��4����ʼ�벴״̬�Ĵ�����Ȼ��Ϊռ�ò�λ����һ���ʵʱϵͳ�в�ͬ��
			do
			{
				this->ship.ship_state = core.genRandomInt(0, 4);
			} while (this->ship.ship_state > 1 && this->berth_occupied[this->ship.berth_idx] == 1);
			//�ڴ���״̬Ϊռ����״̬�£��ж���ǰռ���Ƿ����
			if (this->ship.ship_state > 1 && this->ship.berth_idx < 2)
			{
				//21��λ��22��λ�Ŀ�������
				int ship_type = this->get_ship_type(this->ship.ship_total_storage);
				int ship_21 = 0;
				int ship_22 = 0;
				int current_capacity = 0;
				for (std::vector<Ship>::const_iterator it3 = this->ships.begin(); it3 != this->ships.end(); it3++)
				{
					if (it3->ship_state > 1)
					{
						if (it3->berth_idx == 0)
						{
							//��ǰ���д�ռ��21��λ
							ship_21 = 1;
							current_capacity += this->get_ship_type(it3->ship_total_storage);
						}
						else if (it3->berth_idx == 1)
						{
							//��ǰ���д�ռ��22��λ
							ship_22 = 1;
							current_capacity += this->get_ship_type(it3->ship_total_storage);
						}
					}
				}
				if (ship_21 == 1 && ship_22 == 1)
				{
					//ͨ�ò�λ��ռ�ã����ؿ�������
					enBerth = false;
				}
				else if (ship_21 == 0 && ship_22 == 0)
				{
					//ͨ�ò�λΪ�գ�������
					enBerth = true;
				}
				else if (ship_21 + ship_22 == 1)
				{
					//ͨ�ò�λ��1����
					if (this->ship.berth_idx == 0 && ship_21 == 1)
					{
						//21��λ��ռ��
						enBerth = false;
					}
					else if (this->ship.berth_idx == 1 && ship_22 == 1)
					{
						//22��λ��ռ��
						enBerth = false;
					}
					else if (ship_type + current_capacity > MAX_GENBERTH_CAPACITY)
					{
						//������������
						enBerth = false;
					}
				}
			}
			else if (this->ship.ship_state > 1 && this->ship.berth_idx != 4)
			{
				//����4��λ������4��λͬʱֻ��ͣһ����
				if (this->get_ship_type(this->ship.ship_total_storage) > 1)
				{
					enBerth = false;
				}
				else
				{
					for (std::vector<Ship>::const_iterator it = this->ships.begin(); it != this->ships.end(); it++)
					{
						if (it->ship_state > 1 && (it->berth_idx == 2 || it->berth_idx == 3))
						{
							//�д�������ռ��4��λ
							enBerth = false;
							break;
						}
					}
				}
			}
			else if (this->ship.ship_state > 1 && this->ship.berth_idx == 4)
			{
				//����23��λ������������λ��ҪС��5000
				if (this->get_ship_type(this->ship.ship_total_storage) > 0)
				{
					enBerth = false;
				}
				else
				{
					for (std::vector<Ship>::const_iterator it = this->ships.begin(); it != this->ships.end(); it++)
					{
						if (it->ship_state > 1 && it->berth_idx == 4)
						{
							//�д�������ռ��23��λ
							enBerth = false;
							break;
						}
					}
				}
			}
		} while (!enBerth);
		if (this->ship.ship_state > 1)
		{
			this->berth_occupied[this->ship.berth_idx] = 1;
		}
		//����23��λ��˵������ǰ����Ϊ�մ��������д�������������벴ʱ������������80%�����������������λ��˵������ǰ��������80%������������д�������������벴ʱ����Ϊ�մ�
		if ((this->ship.ship_state < 3 && this->ship.berth_idx == 4) || (this->ship.ship_state == 4 && this->ship.berth_idx != 4))
		{
			this->ship.ship_current_storage = 0.0f;
			this->ship.ship_type = 0;
			this->ship.ship_index = 0;
		}
		else if (this->ship.ship_state == 3)
		{
			this->ship.ship_current_storage = core.genRandomFloat(0.0f, this->ship.ship_total_storage);
			this->ship.ship_type = core.genRandomInt(1, 3);
			this->ship.ship_index = core.genRandomInt(1, count_types[this->ship.ship_type - 1]);
		}
		else if ((this->ship.ship_state == 4 && this->ship.berth_idx == 4) || (this->ship.ship_state < 3 && this->ship.berth_idx != 4))
		{
			this->ship.ship_current_storage = core.genRandomFloat(0.8f * this->ship.ship_total_storage, this->ship.ship_total_storage);
			this->ship.ship_type = core.genRandomInt(1, 3);
			this->ship.ship_index = core.genRandomInt(1, count_types[this->ship.ship_type - 1]);
		}
		//����ê��
		this->ship.wait_pos[0] = core.genRandomFloat(0.0f, (float)CT_WIDTH);
		if (this->ship.berth_idx == 2 || this->ship.berth_idx == 3)
		{
			//4�Ų�λ2050
			this->ship.wait_pos[1] = core.genRandomFloat(2200.0f, (float)CT_HEIGHT);
		}
		else
		{
			//�ϲ�λ350
			this->ship.wait_pos[1] = core.genRandomFloat(0.0f, 200.0f);
		}
		//�������꣨���ڿ����Ĵ����ĳ�ʼλ����ê�أ������벴�Ĵ���λ���ڲ�λ��
		if (this->ship.ship_state == 1 || this->ship.ship_state == 2)
		{
			this->ship.ship_coords[0] = this->ship.wait_pos[0] / (CT_WIDTH / 10.0f) - 5.0f;
			this->ship.ship_coords[1] = -this->ship.wait_pos[1] / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
		}
		else if (this->ship.ship_state == 3 || this->ship.ship_state == 4)
		{
			switch (this->ship.berth_idx)
			{
			case 0:
				//21��λ
				this->ship.ship_coords[0] = this->berth_pos.x_21 / (CT_WIDTH / 10.0f) - 5.0f;
				this->ship.ship_coords[1] = -(float)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 1:
				//22��λ
				this->ship.ship_coords[0] = this->berth_pos.x_22 / (CT_WIDTH / 10.0f) - 5.0f;
				this->ship.ship_coords[1] = -(float)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 2:
				//4�Ҳ�λ
				this->ship.ship_coords[0] = this->berth_pos.x_4_2 / (CT_WIDTH / 10.0f) - 5.0f;
				this->ship.ship_coords[1] = -(float)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 3:
				//4��λ
				this->ship.ship_coords[0] = this->berth_pos.x_4_1 / (CT_WIDTH / 10.0f) - 5.0f;
				this->ship.ship_coords[1] = -(float)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 4:
				//23��λ
				this->ship.ship_coords[0] = this->berth_pos.x_23 / (CT_WIDTH / 10.0f) - 5.0f;
				this->ship.ship_coords[1] = -(float)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			default:
				break;
			}
		}
		//���ɽǶȣ�ѡ������
		long double berth_x = 0.0;
		long double berth_y = 0.0;
		switch (this->ship.berth_idx)
		{
		case 0:
			//21��λ
			this->ship.angle = atan2((this->ship.wait_pos[1] - this->bucket_up.ship), -(this->ship.wait_pos[0] - this->berth_pos.x_21));
			berth_x = (long double)this->berth_pos.x_21 / (CT_WIDTH / 10.0f) - 5.0f;
			berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
			break;
		case 1:
			//22��λ
			this->ship.angle = atan2((this->ship.wait_pos[1] - this->bucket_up.ship), -(this->ship.wait_pos[0] - this->berth_pos.x_22));
			berth_x = (long double)this->berth_pos.x_22 / (CT_WIDTH / 10.0f) - 5.0f;
			berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
			break;
		case 2:
			//4�Ҳ�λ
			this->ship.angle = atan2((this->ship.wait_pos[1] - this->bucket_down.ship), -(this->ship.wait_pos[0] - this->berth_pos.x_4_2));
			berth_x = (long double)this->berth_pos.x_4_2 / (CT_WIDTH / 10.0f) - 5.0f;
			berth_y = -(long double)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
			break;
		case 3:
			//4��λ
			this->ship.angle = atan2((this->ship.wait_pos[1] - this->bucket_down.ship), -(this->ship.wait_pos[0] - this->berth_pos.x_4_1));
			berth_x = (long double)this->berth_pos.x_4_1 / (CT_WIDTH / 10.0f) - 5.0f;
			berth_y = -(long double)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
			break;
		case 4:
			//23��λ
			berth_x = (long double)this->berth_pos.x_23 / (CT_WIDTH / 10.0f) - 5.0f;
			berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
			this->ship.angle = atan2((this->ship.wait_pos[1] - this->bucket_up.ship), -(this->ship.wait_pos[0] - this->berth_pos.x_23));
			break;
		default:
			break;
		}
		//��������
		this->ship.ship_dist = hypot((this->ship.ship_coords[0] - berth_x), (this->ship.ship_coords[1] - berth_y));
		//������������
		this->ship.ship_coords[3] = (float)this->ship.ship_state;
		this->ship.ship_coords[4] = 1.0f - this->ship.ship_current_storage / this->ship.ship_total_storage;
		this->ship.ship_coords[6] = this->get_ship_level(this->ship.ship_total_storage);
		if (this->ship.ship_type == 0)
		{
			this->ship.ship_coords[7] = 0.0f;
		}
		else if (this->ship.ship_type == 1)
		{
			if (this->ship.ship_index < 7)
			{
				this->ship.ship_coords[7] = (float)this->ship.ship_index;
			}
			else
			{
				this->ship.ship_coords[7] = 7.0f;
			}
		}
		else
		{
			this->ship.ship_coords[7] = 7.0f;
		}
		this->ships.push_back(this->ship);
		num_ships--;
	}
}

void Berth::drawUnloader(Camera& camera, Shader& unloaderShader, Shader& bucketShader)
{
	//ж����
	unloaderShader.use();
	//VBO��
	glBindBuffer(GL_ARRAY_BUFFER, unloaderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unloader_coords), &unloader_coords, GL_STATIC_DRAW);
	this->model = glm::mat4(1.0f);
	unloaderShader.setMat4("model", this->model);
	this->projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
	unloaderShader.setMat4("projection", this->projection);
	this->view = glm::mat4(1.0f);
	this->view = camera.GetViewMatrix();
	unloaderShader.setMat4("view", this->view);
	// draw points
	glBindVertexArray(unloaderVAO);
	glDrawArrays(GL_POINTS, 0, 8);
	//ץ��
	bucketShader.use();
	//VBO��
	glBindBuffer(GL_ARRAY_BUFFER, bucketVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bucket_coords), &bucket_coords, GL_STATIC_DRAW);
	bucketShader.setMat4("model", this->model);
	bucketShader.setMat4("projection", this->projection);
	bucketShader.setMat4("view", this->view);
	// draw points
	glBindVertexArray(bucketVAO);
	glDrawArrays(GL_POINTS, 0, 8);
}

void Berth::drawLoader(Camera& camera, Shader& loaderShader)
{
	loaderShader.use();
	//VBO��
	glBindBuffer(GL_ARRAY_BUFFER, loaderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(loader_coords), &loader_coords, GL_STATIC_DRAW);
	this->model = glm::mat4(1.0f);
	loaderShader.setMat4("model", this->model);
	this->projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
	loaderShader.setMat4("projection", this->projection);
	this->view = glm::mat4(1.0f);
	this->view = camera.GetViewMatrix();
	loaderShader.setMat4("view", this->view);
	// draw points
	glBindVertexArray(loaderVAO);
	glDrawArrays(GL_POINTS, 0, 1);
}

void Berth::drawShip(Camera& camera, Shader& shipShader)
{
	shipShader.use();
	glBindBuffer(GL_ARRAY_BUFFER, shipVBO);
	glBindVertexArray(shipVAO);
	this->model = glm::mat4(1.0f);
	shipShader.setMat4("model", this->model);
	this->projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
	shipShader.setMat4("projection", this->projection);
	this->view = glm::mat4(1.0f);
	this->view = camera.GetViewMatrix();
	shipShader.setMat4("view", this->view);
	for (std::vector<Ship>::iterator it = this->ships.begin(); it != this->ships.end(); it++)
	{
		if (it->ship_state != 0)
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(it->ship_coords), &it->ship_coords, GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, 1);
		}
	}
}

void Berth::initGuiStyle()
{
	this->style = &ImGui::GetStyle();
}

int Berth::ship_dispatch(Message& message)
{
	int ret = -1;
	static char name[50];
	static int wait_pos_1 = 0;
	static int wait_pos_2 = 0;
	static int berthIdx = 0;
	static int total_storage = 0;
	static int current_storage = 0;
	static bool mode_gen = false;
	//С��
	static int cargo_index = 1;
	//����
	static int cargo_type = 1;

	//��ֻ����̨
	if (ImGui::CollapsingHeader(u8"���������������"))
	{
		ImGui::Indent();
		ImGui::InputText(u8"�����Ϻ�����", name, 50);	//��ǰ�����Ϻ���ͬ
		ImGui::Separator();
		ImGui::Text(u8"ê��ѡ��");
		ImGui::SameLine();
		ImGui::RadioButton(u8"21�Ų�λ", &berthIdx, 0);
		ImGui::SameLine();
		ImGui::RadioButton(u8"22�Ų�λ", &berthIdx, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"4�Ų�λ��", &berthIdx, 2);
		ImGui::SameLine();
		ImGui::RadioButton(u8"4�Ų�λ��", &berthIdx, 3);
		ImGui::SameLine();
		ImGui::RadioButton(u8"23�Ų�λ", &berthIdx, 4);
		ImGui::SliderInt(u8"ê��x����", &wait_pos_1, 0, CT_WIDTH);
		ImGui::SliderInt(u8"ê��y����", &wait_pos_2, 0, CT_HEIGHT);
		ImGui::Separator();
		ImGui::InputInt(u8"�������ػ���", &total_storage, 1, 200);
		ImGui::InputInt(u8"������ǰ�ػ���", &current_storage, 1, 200);
		ImGui::Separator();
		ImGui::Text(u8"����������ࣺ");
		ImGui::SameLine();
		ImGui::RadioButton(u8"�����", &cargo_type, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"����Դ��", &cargo_type, 2);
		ImGui::SameLine();
		ImGui::RadioButton(u8"����ҵ��", &cargo_type, 3);
		ImGui::Text(u8"ѡ�����С�ࣺ");
		int temp = 1;
		for (std::vector<Type_ship>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
		{
			if (it->type_type == cargo_type)
			{
				ImGui::SameLine();
				ImGui::RadioButton(it->type_name.c_str(), &cargo_index, temp);
				temp++;
			}
		}
		this->pre_button();
		if (ImGui::Button(u8"�������"))
		{
			bool enable = true;		//�����������
			if (enable && name == "")
			{
				enable = false;
				//std::cout << "��������ʧ��::�Ϻ�Ϊ��" << std::endl;
				message.push_message(u8"��������ʧ��::�Ϻ�Ϊ��");
			}
			if (enable && berthIdx == 2 || berthIdx == 3)
			{
				//�²�λê��
				if (wait_pos_2 < 2050)
				{
					enable = false;
					//std::cout << "��������ʧ��::ê�طǷ�" << std::endl;
					message.push_message(u8"��������ʧ��::ê�طǷ�");
				}
			}
			else
			{
				//�ϲ�λê��
				if (wait_pos_2 > 350)
				{
					enable = false;
					//std::cout << "��������ʧ��::ê�طǷ�" << std::endl;
					message.push_message(u8"��������ʧ��::ê�طǷ�");
				}
			}
			if (enable && this->type_check(cargo_type, cargo_index) == false)
			{
				enable = false;
				//std::cout << "��������ʧ��::�����������" << std::endl;
				message.push_message(u8"��������ʧ��::�����������");
			}
			if (enable && total_storage <= 0 || current_storage <= 0)
			{
				enable = false;
				//std::cout << "��������ʧ��::�ػ���������Ϊ��" << std::endl;
				message.push_message(u8"��������ʧ��::�ػ���������Ϊ��");
			}
			if (enable && total_storage < current_storage)
			{
				enable = false;
				//std::cout << "��������ʧ��::��ֹ����" << std::endl;
				message.push_message(u8"��������ʧ��::��ֹ����");
			}
			if (enable && total_storage > 65000)
			{
				enable = false;
				//std::cout << "��������ʧ��::����������λ��������" << std::endl;
				message.push_message(u8"��������ʧ��::����������λ��������");
			}
			if (enable && (berthIdx == 2 || berthIdx == 3) && total_storage > 20000)
			{
				enable = false;
				//std::cout << "��������ʧ��::������λ����4��λ������" << std::endl;
				message.push_message(u8"��������ʧ��::������λ����4��λ������");
			}
			if (enable && berthIdx == 4 && total_storage > 5000)
			{
				enable = false;
				//std::cout << "��������ʧ��::������λ����23��λ������" << std::endl;
				message.push_message(u8"��������ʧ��::������λ����23��λ������");
			}
			if (enable)
			{
				//�������
				this->ship.ship_name = name;
				this->ship.ship_state = 0;
				this->ship.angle = 0.0;
				this->ship.ship_total_storage = (float)total_storage;
				this->ship.ship_current_storage = (float)current_storage;
				this->ship.berth_idx = berthIdx;
				this->ship.wait_pos[0] = (float)wait_pos_1;
				this->ship.wait_pos[1] = (float)wait_pos_2;
				this->ship.ship_coords[0] = 0.0f;
				this->ship.ship_coords[1] = 0.0f;
				this->ship.ship_coords[2] = layer_ship;
				this->ship.ship_coords[3] = 0.0f;
				this->ship.ship_coords[4] = 1.0f - (float)current_storage / (float)total_storage;
				this->ship.ship_coords[5] = 0.0f;
				this->ship.ship_coords[6] = this->get_ship_level((float)total_storage);
				//��������ж�
				if (cargo_type == 0)
				{
					this->ship.ship_coords[7] = 0.0f;
				}
				else if (cargo_type == 1)
				{
					if (cargo_index < 7)
					{
						this->ship.ship_coords[7] = (float)cargo_index;
					}
					else
					{
						this->ship.ship_coords[7] = 7.0f;
					}
				}
				else
				{
					this->ship.ship_coords[7] = 7.0f;
				}
				this->ships.push_back(this->ship);
				//std::cout << "������������ɹ���" << std::endl;
				message.push_message(u8"������������ɹ���");
			}
		}
		this->post_button();
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader(u8"��������̨"))
	{
		ImGui::Indent();
		std::string name_harbor = "";
		ImGui::Text(u8"ѡ��ͨ�ò�λ����ѡ��ģʽ��");
		ImGui::SameLine();
		if (this->unload_mode_gen)
		{
			ImGui::Checkbox(u8"����ȳ�ģʽ��LIFO��", &mode_gen);
			this->unload_mode_gen = mode_gen;
		}
		else
		{
			ImGui::Checkbox(u8"�Ƚ��ȳ�ģʽ��FIFO��", &mode_gen);
			this->unload_mode_gen = mode_gen;
		}
		ImGui::Separator();
		for (std::vector<Ship>::iterator it = this->ships.begin(); it != this->ships.end(); )
		{
			bool temp = true;
			switch(it->berth_idx)
			{
			case 0:
				name_harbor = "21#";
				break;
			case 1:
				name_harbor = "22#";
				break;
			case 2:
				name_harbor = "4#";
				break;
			case 3:
				name_harbor = "4#";
				break;
			case 4:
				name_harbor = "23#";
				break;
			default:
				break;
			}
			switch (it->ship_state)
			{
			case 0:
				ImGui::Text((u8"��������" + it->ship_name + u8" ����ǰδ���ۣ�").c_str());
				ImGui::Text((u8"���䲴λ��" + name_harbor).c_str());
				for (std::vector<Type_ship>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->ship_type && it2->type_index == it->ship_index)
					{
						ImGui::Text((u8"�����ػ����ࣺ" + it2->type_name).c_str());
						break;
					}
				}
				this->pre_button();
				if (ImGui::Button(u8"��ê�صȴ�"))
				{
					it->ship_state = 1;
					it->ship_coords[3] = 1.0f;
				}
				ImGui::SameLine();
				if (ImGui::Button((it->ship_name + u8"�������##1").c_str()))
				{
					it = this->ships.erase(it);
					temp = false;
				}
				this->post_button();
				break;
			case 1:
				ImGui::Text((u8"��������" + it->ship_name + u8" ����ǰ��ê�أ�").c_str());
				ImGui::Text((u8"���䲴λ��" + name_harbor).c_str());
				for (std::vector<Type_ship>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->ship_type && it2->type_index == it->ship_index)
					{
						ImGui::Text((u8"�����ػ����ࣺ" + it2->type_name).c_str());
						break;
					}
				}
				this->pre_button();
				if (ImGui::Button(u8"������"))
				{
					bool enBerth = true;
					if (it->berth_idx < 2)
					{
						//21��λ��22��λ�Ŀ�������
						int ship_type = this->get_ship_type(it->ship_total_storage);
						int ship_21 = 0;
						int ship_22 = 0;
						int current_capacity = 0;
						for (std::vector<Ship>::const_iterator it3 = this->ships.begin(); it3 != this->ships.end(); it3++)
						{
							if (it3->ship_state == 2 || it3->ship_state == 3)
							{
								if (it3->berth_idx == 0)
								{
									//��ǰ���д�ռ��21��λ
									ship_21 = 1;
									current_capacity += this->get_ship_type(it3->ship_total_storage);
								}
								else if (it3->berth_idx == 1)
								{
									//��ǰ���д�ռ��22��λ
									ship_22 = 1;
									current_capacity += this->get_ship_type(it3->ship_total_storage);
								}
							}
						}
						if (ship_21 == 1 && ship_22 == 1)
						{
							//ͨ�ò�λ��ռ�ã����ؿ�������
							enBerth = false;
							//std::cout << "��������ʧ��::ͨ�ò�λ��ȫ��ռ��" << std::endl;
							message.push_message(u8"��������ʧ��::ͨ�ò�λ��ȫ��ռ��");
						}
						else if (ship_21 == 0 && ship_22 == 0)
						{
							//ͨ�ò�λΪ�գ�������
							enBerth = true;
						}
						else if (ship_21 + ship_22 == 1)
						{
							//ͨ�ò�λ��1����
							if (berthIdx == 0 && ship_21 == 1)
							{
								//21��λ��ռ��
								enBerth = false;
								//std::cout << "��������ʧ��::21��λ��ռ��" << std::endl;
								message.push_message(u8"��������ʧ��::21��λ��ռ��");
							}
							else if (berthIdx == 1 && ship_22 == 1)
							{
								//22��λ��ռ��
								enBerth = false;
								//std::cout << "��������ʧ��::22��λ��ռ��" << std::endl;
								message.push_message(u8"��������ʧ��::22��λ��ռ��");
							}
							else if (ship_type + current_capacity > MAX_GENBERTH_CAPACITY)
							{
								//������������
								enBerth = false;
								//std::cout << "��������ʧ��::���볬����λ������������" << std::endl;
								message.push_message(u8"��������ʧ��::���볬����λ������������");
							}
						}
					}
					else if (it->berth_idx != 4)
					{
						//����4��λ������4��λͬʱֻ��ͣһ����
						for (std::vector<Ship>::const_iterator it = this->ships.begin(); it != this->ships.end(); it++)
						{
							if ((it->ship_state == 2 || it->ship_state == 3) && (it->berth_idx == 2 || it->berth_idx == 3))
							{
								//�д�������ռ��4��λ
								enBerth = false;
								//std::cout << "��������ʧ��::4��λ��ռ��" << std::endl;
								message.push_message(u8"��������ʧ��::4��λ��ռ��");
								break;
							}
						}
					}
					if (enBerth)
					{
						it->ship_state = 2;
						it->ship_coords[3] = 2.0f;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button((it->ship_name + u8"�������##2").c_str()))
				{
					it->ship_state = 4;
					it->ship_coords[3] = 4.0f;
				}
				this->post_button();
				break;
			case 2:
				ImGui::Text((u8"��������" + it->ship_name + u8" ����ǰ���ڿ�����").c_str());
				ImGui::Text((u8"ռ�ò�λ��" + name_harbor).c_str());
				for (std::vector<Type_ship>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->ship_type && it2->type_index == it->ship_index)
					{
						ImGui::Text((u8"�����ػ����ࣺ" + it2->type_name).c_str());
						break;
					}
				}
				this->pre_button();
				if (ImGui::Button((it->ship_name + u8"�������##3").c_str()))
				{
					it->ship_state = 4;
					it->ship_coords[3] = 4.0f;
				}
				this->post_button();
				break;
			case 3:
				ImGui::Text((u8"��������" + it->ship_name + u8" ����ǰλ�ڲ�λ��").c_str());
				ImGui::Text((u8"���䲴λ��" + name_harbor).c_str());
				for (std::vector<Type_ship>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->ship_type && it2->type_index == it->ship_index)
					{
						ImGui::Text((u8"�����ػ����ࣺ" + it2->type_name).c_str());
						break;
					}
				}
				this->pre_button();
				if (ImGui::Button((it->ship_name + u8"�������##4").c_str()))
				{
					it->ship_state = 4;
					it->ship_coords[3] = 4.0f;
					//ֹͣ��Ӧ��λ��ж������ץ��
					int num_gen_ships = 0;
					switch (it->berth_idx)
					{
					case 0:
						//21��λ
						for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
						{
							if (it2->unloader_index < 2 && it2->unloader_state == 2)
							{
								it2->unloader_state = 1;
								this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
							}
						}
						//���ͨ�ò�λ�����д������ͷ�������ָֹͣ��
						for (std::vector<Ship>::const_iterator it3 = this->ships.begin(); it3 != this->ships.end(); it3++)
						{
							if (it3->berth_idx < 2 && it3->ship_state == 3)
							{
								num_gen_ships++;
							}
						}
						if (num_gen_ships == 0)
						{
							ret = it->berth_idx;
						}
						break;
					case 1:
						//22��λ
						for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
						{
							if (it2->unloader_index > 1 && it2->unloader_index < 4 && it2->unloader_state == 2)
							{
								it2->unloader_state = 1;
								this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
							}
						}
						//���ͨ�ò�λ�����д������ͷ�������ָֹͣ��
						for (std::vector<Ship>::const_iterator it3 = this->ships.begin(); it3 != this->ships.end(); it3++)
						{
							if (it3->berth_idx < 2 && it3->ship_state == 3)
							{
								num_gen_ships++;
							}
						}
						if (num_gen_ships == 0)
						{
							ret = it->berth_idx;
						}
						break;
					case 2:
						//4�Ҳ�λ
						for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
						{
							if ((it2->unloader_index == 6 || it2->unloader_index == 7) && it2->unloader_state == 2)
							{
								it2->unloader_state = 1;
								this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
								ret = it->berth_idx;
							}
						}
						//4��λ����ͬʱ������������������ֱ���ж�����
						ret = it->berth_idx;
						break;
					case 3:
						//4��λ
						for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
						{
							if ((it2->unloader_index == 4 || it2->unloader_index == 5) && it2->unloader_state == 2)
							{
								it2->unloader_state = 1;
								this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
								ret = it->berth_idx;
							}
						}
						//4��λ����ͬʱ������������������ֱ���ж�����
						ret = it->berth_idx;
						break;
					case 4:
						//23��λ
						if (this->loader.loader_state == 2)
						{
							this->loader.loader_state = 1;
							this->loader_coords[3] = 1.0f;
							ret = it->berth_idx;
						}
						break;
					default:
						break;
					}
				}
				this->post_button();
				break;
			case 4:
				ImGui::Text((u8"��������" + it->ship_name + u8" ����ǰ������ۣ�").c_str());
				ImGui::Text((u8"ԭ��λ��" + name_harbor).c_str());
				for (std::vector<Type_ship>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->ship_type && it2->type_index == it->ship_index)
					{
						ImGui::Text((u8"�����ػ����ࣺ" + it2->type_name).c_str());
						break;
					}
				}
				break;
			default:
				break;
			}
			ImGui::Separator();
			if (temp)
			{
				it++;
			}
		}
		ImGui::Unindent();
	}
	return ret;
}

void Berth::unloader_dispatch()
{
	static float set_hor = 0.0f;		//ǿ���趨ץ��ˮƽλ��
	static float set_ver = 0.0f;		//ǿ���趨ץ����ֱλ��
	static int set_state = 1;			//ǿ���趨ж��������״̬
	static bool belt_switch = false;	//ǿ���趨ж����ж�Ͽڷ���
	static int choosed_idx = -1;		//ѡ��Ҫָ����ж�������
	if (ImGui::CollapsingHeader(u8"ж��������̨"))
	{
		ImGui::Indent();
		for (std::vector<ShipUnloader>::iterator it1 = this->unloaders.begin(); it1 != this->unloaders.end(); it1++)
		{
			ImGui::Text((it1->unloader_name + u8"ж����").c_str());
			ImGui::Text(u8"��ǰ״̬��");
			ImGui::SameLine();
			if (it1->unloader_state == 0)
			{
				ImGui::Text(u8"δ����");
			}
			else if (it1->unloader_state == 1)
			{
				ImGui::Text(u8"�豸����");
			}
			else if (it1->unloader_state == 2)
			{
				ImGui::Text(u8"��������");
			}
			ImGui::Text((u8"ץ��ˮƽλ�ã�" + std::to_string(it1->crab_bucket_hor)).c_str());
			ImGui::Text((u8"ץ����ֱλ�ã�" + std::to_string(it1->crab_bucket_ver)).c_str());
			if (it1->crab_bucket_switch)
			{
				ImGui::Text(u8"��ǰ��BƤ��");
			}
			else
			{
				ImGui::Text(u8"��ǰ��AƤ��");
			}
			if (it1->crab_bucket_loaded)
			{
				ImGui::Text(u8"ץ����ȡ��");
			}
			else
			{
				ImGui::Text(u8"ץ��δȡ��");
			}
			this->pre_button();
			if (ImGui::Button((u8"ѡ��ָ��" + it1->unloader_name + u8"ж�����Ĳ���").c_str()))
			{
				if (choosed_idx == it1->unloader_index)
				{
					choosed_idx = -1;
				}
				else
				{
					choosed_idx = it1->unloader_index;
				}
			}
			this->post_button();
			if (choosed_idx == it1->unloader_index)
			{
				ImGui::Text((u8"ǿ��ָ��" + it1->unloader_name + u8"ж����������״̬��").c_str());
				if (set_state == 0)
				{
					ImGui::SliderInt((it1->unloader_name + u8"ͣ��").c_str(), &set_state, 0, 2);
				}
				else if (set_state == 1)
				{
					ImGui::SliderInt((it1->unloader_name + u8"����").c_str(), &set_state, 0, 2);
				}
				else if (set_state == 2)
				{
					ImGui::SliderInt((it1->unloader_name + u8"����").c_str(), &set_state, 0, 2);
				}
				this->pre_button();
				if (ImGui::Button((it1->unloader_name + u8"ָ������״̬").c_str()))
				{
					it1->unloader_state = set_state;
					this->unloader_coords[5 * it1->unloader_index + 3] = (float)set_state;
				}
				this->post_button();
				ImGui::Text((u8"ǿ��ָ��" + it1->unloader_name + u8"ж����ץ����ˮƽλ�ã�").c_str());
				ImGui::SliderFloat((it1->unloader_name + u8"ˮƽλ��").c_str(), &set_hor, -1.0f, 1.0f);
				this->pre_button();
				if (ImGui::Button((it1->unloader_name + u8"ָ��ץ����ˮƽλ��").c_str()))
				{
					it1->crab_bucket_hor = set_hor;
				}
				this->post_button();
				ImGui::Text((u8"ǿ��ָ��" + it1->unloader_name + u8"ж����ץ���Ĵ�ֱλ�ã�").c_str());
				ImGui::SliderFloat((it1->unloader_name + u8"��ֱλ��").c_str(), &set_ver, 0.0f, 1.0f);
				this->pre_button();
				if (ImGui::Button((it1->unloader_name + u8"ָ��ץ���Ĵ�ֱλ��").c_str()))
				{
					it1->crab_bucket_ver = set_ver;
				}
				this->post_button();
				ImGui::Text((u8"ǿ��ָ��" + it1->unloader_name + u8"ж����ץ���Ĵ�ֱλ�ã�").c_str());
				if (belt_switch)
				{
					ImGui::Checkbox((it1->unloader_name + u8"��BƤ��").c_str(), &belt_switch);
				}
				else
				{
					ImGui::Checkbox((it1->unloader_name + u8"��AƤ��").c_str(), &belt_switch);
				}
				this->pre_button();
				if (ImGui::Button((it1->unloader_name + u8"ָ���Ż�ץ��λ�ڵ�Ƥ��").c_str()))
				{
					it1->crab_bucket_switch = belt_switch;
				}
				this->post_button();
			}
			if (!(it1 + 1 == this->unloaders.end()))
			{
				ImGui::Separator();
			}
		}
		ImGui::Unindent();
	}
}

void Berth::updateBuckets(float simurate)
{
	//ץ��״̬���´���
	//float rate_ceiling = simurate > 20.0f ? 20.0f : simurate;
	float rate_ceiling = simurate;
	for (std::vector<ShipUnloader>::iterator it = this->unloaders.begin(); it != this->unloaders.end(); it++)
	{
		//����ж��������״̬ȷ���߼���֧
		if (it->unloader_state == 1)
		{
			//�Ż�״̬Ϊ1ʱ��ץ���ƶ�������λ�ã��������ȵ�Ƥ����ж�ϣ�Ȼ���ƶ�������λ��
			if (this->bucket_coords[5 * it->unloader_index + 3] < 1.0f)
			{
				//ץ��δж�ִ꣬��ж��
				if (it->crab_bucket_hor == 0.0f)
				{
					//ץ��λ�ڴ�
					if (it->crab_bucket_ver < 1.0f)
					{
						//ץ��δ����λ��y���䣬hor���ϣ�ץ����ú
						it->crab_bucket_ver += 0.1f * bucket_speed_ver * rate_ceiling;
						//this->bucket_coords[5 * it->unloader_index + 3] = 0.0f;
					}
					else
					{
						//ץ������λ����Ƥ������
						it->crab_bucket_hor += 0.1f * bucket_speed_hor * rate_ceiling;
						if (it->crab_bucket_switch == false)
						{
							//AƤ��
							if (it->unloader_index < 4)
							{
								//1-4#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_A - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
							else
							{
								//11-14#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_A)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
						}
						else
						{
							//BƤ��
							if (it->unloader_index < 4)
							{
								//1-4#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_B - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
							else
							{
								//11-14#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_B)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
						}
					}
				}
				else if (it->crab_bucket_hor < 1.0f)
				{
					//ץ������λ����Ƥ������
					it->crab_bucket_hor += 0.1f * bucket_speed_hor * rate_ceiling;
					if (it->crab_bucket_switch == false)
					{
						//AƤ��
						if (it->unloader_index < 4)
						{
							//1-4#
							this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_A - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
						}
						else
						{
							//11-14#
							this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_A)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
						}
					}
					else
					{
						//BƤ��
						if (it->unloader_index < 4)
						{
							//1-4#
							this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_B - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
						}
						else
						{
							//11-14#
							this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_B)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
						}
					}
				}
				else
				{
					//ץ��λ��Ƥ��
					if (it->crab_bucket_ver > 0.0f)
					{
						//ץ��δ����λ��ץ���½���ץ��жú����ɫ��ǳ
						it->crab_bucket_ver -= 0.1f * bucket_speed_ver * rate_ceiling;
						this->bucket_coords[5 * it->unloader_index + 3] += 0.1f * bucket_speed_ver * rate_ceiling;
						//��ֵУ��
						if (this->bucket_coords[5 * it->unloader_index + 3] > 1.0f || it->crab_bucket_ver < 0.0f)
						{
							it->crab_bucket_ver = 0.0f;
							this->bucket_coords[5 * it->unloader_index + 3] = 1.0f;
							it->crab_bucket_loaded = false;
						}
					}
					else
					{
						//ץ������λ���������
						it->crab_bucket_ver = 0.0f;
						this->bucket_coords[5 * it->unloader_index + 3] = 1.0f;
						it->crab_bucket_loaded = false;
					}
				}
			}
			else
			{
				//ץ��ж�꣬�ƶ�������λ��
				if (it->crab_bucket_ver < 1.0f)
				{
					//ץ��δ����λ��������
					it->crab_bucket_ver += 0.1f * bucket_speed_ver * rate_ceiling;
				}
				else
				{
					//ץ������λ��������λ���ƶ�
					if (it->crab_bucket_hor > -1.0f)
					{
						//ץ��δ��λ��������λ���ƶ�
						it->crab_bucket_hor -= 0.1f * bucket_speed_reset * rate_ceiling;
						if (it->crab_bucket_switch == false)
						{
							//AƤ��
							if (it->unloader_index < 4)
							{
								//1-4#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.conv_A + (1 - it->crab_bucket_hor) * (this->bucket_up.idle - this->bucket_up.conv_A) / 2) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
							else
							{
								//11-14#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.conv_A - (1 - it->crab_bucket_hor) * (this->bucket_down.conv_A - this->bucket_down.idle) / 2) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
						}
						else
						{
							//BƤ��
							if (it->unloader_index < 4)
							{
								//1-4#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.conv_B + (1 - it->crab_bucket_hor) * (this->bucket_up.idle - this->bucket_up.conv_B) / 2) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
							else
							{
								//11-14#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.conv_B - (1 - it->crab_bucket_hor) * (this->bucket_down.conv_B - this->bucket_down.idle) / 2) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
						}
					}
				}
			}
		}
		else if (it->unloader_state == 2)
		{
			//�Ż�״̬Ϊ2ʱץ�����ڹ���ѭ�����ƶ�����������ץ����ȡ�ϣ�������ץ�����ƶ���Ƥ��������ץ����ж�ϣ�������ץ��
			if (it->crab_bucket_hor < 0.0f)
			{
				//ץ������������½�ץ���ƶ�������λ�ã����Ƥ�������迼��crab_bucket_switch��
				it->crab_bucket_hor += 0.1f * bucket_speed_hor * rate_ceiling;
				//��ֵУ��
				if (it->crab_bucket_hor > 0.0f)
					it->crab_bucket_hor = 0.0f;
				if (it->unloader_index < 4)
				{
					//1-4#
					this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship - it->crab_bucket_hor * (this->bucket_up.idle - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				}
				else
				{
					//11-14#
					this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.idle + (1 + it->crab_bucket_hor) * (this->bucket_down.ship - this->bucket_down.idle)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				}
			}
			else
			{
				//ץ�����ڹ���λ�ã���������
				if (it->crab_bucket_loaded == false)
				{
					//ץ��δȡ�ϣ������У�������Ƥ���������������У����µĹ���
					if (it->crab_bucket_hor == 1.0f)
					{
						//ץ��λ��Ƥ��
						if (it->crab_bucket_ver < 1.0f)
						{
							//ץ��δ����λ��y���䣬hor���ϣ�ץ��Ϊ�գ��ǰ�ɫ
							it->crab_bucket_ver += 0.1f * bucket_speed_up * rate_ceiling;
							//��ֵУ��
							if (it->crab_bucket_ver > 1.0f)
								it->crab_bucket_ver = 1.0f;
							//this->bucket_coords[5 * it->unloader_index + 3] = 1.0f;
						}
						else
						{
							//ץ������λ��������
							it->crab_bucket_hor -= 0.1f * bucket_speed_hor * rate_ceiling;
							if (it->crab_bucket_switch == false)
							{
								//AƤ��
								if (it->unloader_index < 4)
								{
									//1-4#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_A - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
								else
								{
									//11-14#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_A)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
							}
							else
							{
								//BƤ��
								if (it->unloader_index < 4)
								{
									//1-4#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_B - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
								else
								{
									//11-14#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_B)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
							}
						}
					}
					else if (it->crab_bucket_hor > 0.0f)
					{
						//ץ��������
						if (it->crab_bucket_ver < 1.0f)
						{
							//ץ��δ����λ��y���䣬hor����
							it->crab_bucket_ver += 0.1f * bucket_speed_up * rate_ceiling;
							//��ֵУ��
							if (it->crab_bucket_ver > 1.0f)
								it->crab_bucket_ver = 1.0f;
						}
						else
						{
							//ץ������λ��������
							it->crab_bucket_hor -= 0.1f * bucket_speed_hor * rate_ceiling;
							//��ֵУ��
							if (it->crab_bucket_hor < 0.0f)
								it->crab_bucket_hor = 0.0f;
							if (it->crab_bucket_switch == false)
							{
								//AƤ��
								if (it->unloader_index < 4)
								{
									//1-4#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_A - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
								else
								{
									//11-14#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_A)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
							}
							else
							{
								//BƤ��
								if (it->unloader_index < 4)
								{
									//1-4#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_B - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
								else
								{
									//11-14#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_B)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
							}
						}
					}
					else
					{
						//ץ���ڴ�
						if (it->crab_bucket_ver > 0.0f)
						{
							//ץ��δ����λ��ץ���½���ץ��װú����ɫ����
							it->crab_bucket_ver -= 0.1f * bucket_speed_ver * rate_ceiling;
							this->bucket_coords[5 * it->unloader_index + 3] -= 0.1f * bucket_speed_ver * rate_ceiling;
							//��ֵУ��
							if (this->bucket_coords[5 * it->unloader_index + 3] < 0.0f || it->crab_bucket_ver < 0.0f)
							{
								it->crab_bucket_ver = 0.0f;
								this->bucket_coords[5 * it->unloader_index + 3] = 0.0f;
								it->crab_bucket_loaded = true;
							}
							//��жú
							this->bucket_unload(simurate, *it);
						}
						else
						{
							//ץ������λ��ȥ�����
							it->crab_bucket_ver = 0.0f;
							this->bucket_coords[5 * it->unloader_index + 3] = 0.0f;
							it->crab_bucket_loaded = true;
						}
					}
				}
				else
				{
					//ץ����ȡ�ϣ��Ӵ��������ƶ���Ƥ��������
					if (it->crab_bucket_hor == 0.0f)
					{
						//ץ��λ�ڴ�
						if (it->crab_bucket_ver < 1.0f)
						{
							//ץ��δ����λ��y���䣬hor���ϣ�ץ��Ϊ�����Ǻ�ɫ
							it->crab_bucket_ver += 0.1f * bucket_speed_ver * simurate;
							//��ֵУ��
							if (it->crab_bucket_ver > 1.0f)
								it->crab_bucket_ver = 1.0f;
							//this->bucket_coords[5 * it->unloader_index + 3] = 0.0f;
						}
						else
						{
							//ץ������λ����Ƥ������
							it->crab_bucket_hor += 0.1f * bucket_speed_hor * rate_ceiling;
							if (it->crab_bucket_switch == false)
							{
								//AƤ��
								if (it->unloader_index < 4)
								{
									//1-4#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_A - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
								else
								{
									//11-14#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_A)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
							}
							else
							{
								//BƤ��
								if (it->unloader_index < 4)
								{
									//1-4#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_B - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
								else
								{
									//11-14#
									this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_B)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
								}
							}
						}
					}
					else if (it->crab_bucket_hor < 1.0f)
					{
						//ץ������λ����Ƥ������
						it->crab_bucket_hor += 0.1f * bucket_speed_hor * rate_ceiling;
						//��ֵУ��
						if (it->crab_bucket_hor > 1.0f)
							it->crab_bucket_hor = 1.0f;
						if (it->crab_bucket_switch == false)
						{
							//AƤ��
							if (it->unloader_index < 4)
							{
								//1-4#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_A - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
							else
							{
								//11-14#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_A)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
						}
						else
						{
							//BƤ��
							if (it->unloader_index < 4)
							{
								//1-4#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_up.ship + it->crab_bucket_hor * (this->bucket_up.conv_B - this->bucket_up.ship)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
							else
							{
								//11-14#
								this->bucket_coords[5 * it->unloader_index + 1] = -(this->bucket_down.ship - it->crab_bucket_hor * (this->bucket_down.ship - this->bucket_down.conv_B)) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
							}
						}
					}
					else
					{
						//ץ��λ��Ƥ��
						if (it->crab_bucket_ver > 0.0f)
						{
							//ץ��δ����λ��ץ���½���ץ��жú����ɫ��ǳ
							it->crab_bucket_ver -= 0.1f * bucket_speed_ver * rate_ceiling;
							this->bucket_coords[5 * it->unloader_index + 3] += 0.1f * bucket_speed_ver * rate_ceiling;
							//��ֵУ��
							if (this->bucket_coords[5 * it->unloader_index + 3] > 1.0f || it->crab_bucket_ver < 0.0f)
							{
								it->crab_bucket_ver = 0.0f;
								this->bucket_coords[5 * it->unloader_index + 3] = 1.0f;
								it->crab_bucket_loaded = false;
							}
						}
						else
						{
							//ץ������λ���������
							it->crab_bucket_ver = 0.0f;
							this->bucket_coords[5 * it->unloader_index + 3] = 1.0f;
							it->crab_bucket_loaded = false;
						}
					}
				}
			}
		}
	}
}

int Berth::updateShips(float simurate)
{
	int ship_ret = 0;
	//��ֻ״̬���´���
	for (std::vector<Ship>::iterator it = this->ships.begin(); it != this->ships.end(); )
	{
		bool temp = true;
		if (it->ship_state == 0)
		{
			//δ���ۣ�����������
			it++;
			continue;
		}
		else if (it->ship_state == 1)
		{
			//λ��ê�أ�λ�ò���
			if (it->ship_coords[0] == 0.0f)
			{
				//����λ�úͽǶ�
				it->ship_coords[0] = it->wait_pos[0] / (CT_WIDTH / 10.0f) - 5.0f;
				it->ship_coords[1] = -it->wait_pos[1] / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				switch (it->berth_idx)
				{
				case 0:
					//21��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_21));
					break;
				case 1:
					//22��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_22));
					break;
				case 2:
					//4�Ҳ�λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_2));
					break;
				case 3:
					//4��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_1));
					break;
				case 4:
					//23��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_23));
					break;
				default:
					break;
				}
				//���ô���ɫλ
				it->ship_coords[3] = 1.0f;
			}
		}
		else if (it->ship_state == 2)
		{
			//���ڿ�������λ�ƶ�
			if (it->ship_coords[0] == 0.0f)
			{
				//����λ�úͽǶ�
				it->ship_coords[0] = it->wait_pos[0] / (CT_WIDTH / 10.0f) - 5.0f;
				it->ship_coords[1] = -it->wait_pos[1] / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				switch (it->berth_idx)
				{
				case 0:
					//21��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_21));
					break;
				case 1:
					//22��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_22));
					break;
				case 2:
					//4�Ҳ�λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_2));
					break;
				case 3:
					//4��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_1));
					break;
				case 4:
					//23��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_23));
					break;
				default:
					break;
				}
				//���ô���ɫλ
				it->ship_coords[3] = 2.0f;
			}
			long double berth_x = 0.0;
			long double berth_y = 0.0;
			switch (it->berth_idx)
			{
			case 0:
				//21��λ
				berth_x = (long double)this->berth_pos.x_21 / (CT_WIDTH / 10.0f) - 5.0f;
				berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 1:
				//22��λ
				berth_x = (long double)this->berth_pos.x_22 / (CT_WIDTH / 10.0f) - 5.0f;
				berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 2:
				//4�Ҳ�λ
				berth_x = (long double)this->berth_pos.x_4_2 / (CT_WIDTH / 10.0f) - 5.0f;
				berth_y = -(long double)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 3:
				//4��λ
				berth_x = (long double)this->berth_pos.x_4_1 / (CT_WIDTH / 10.0f) - 5.0f;
				berth_y = -(long double)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 4:
				//23��λ
				berth_x = (long double)this->berth_pos.x_23 / (CT_WIDTH / 10.0f) - 5.0f;
				berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			default:
				break;
			}
			long double temp_dist = hypot((it->ship_coords[0] - berth_x), (it->ship_coords[1] - berth_y));
			if (temp_dist > it->ship_dist)
			{
				//���ھ������֮ǰһ�̵ľ��룬��ʾ����ʻ�벴λ����Ϊ����״̬
				it->ship_coords[0] = (float)berth_x;
				it->ship_coords[1] = (float)berth_y;
				it->ship_coords[3] = 3.0f;
				it->ship_coords[4] = 1.0f - it->ship_current_storage / it->ship_total_storage;
				it->ship_state = 3;
			}
			else
			{
				//��ֻδ��λ���ƶ�
				it->ship_coords[0] += (float)(speed_ship * cos(it->angle)) * simurate;
				it->ship_coords[1] += (float)(speed_ship * sin(it->angle)) * simurate;
			}
			it->ship_dist = temp_dist;
		}
		else if (it->ship_state == 3)
		{
			//�ڲ�λ�������ϲְٷֱ�
			if (it->ship_current_storage >= 0.0f)
			{
				it->ship_coords[4] = 1.0f - it->ship_current_storage / it->ship_total_storage;
			}
			//ж���Ĵ������䶯
			if (it->berth_idx == 4 && this->loader.loader_state == 2)
			{
				it->ship_current_storage += FLUX_LAOD * simurate;
				if (it->ship_current_storage > it->ship_total_storage)
				{
					it->ship_current_storage = it->ship_total_storage;
					ship_ret = 1;
				}
			}
		}
		else if (it->ship_state == 4)
		{
			//�����
			if (it->ship_coords[0] == 0.0f)
			{
				//����λ�úͽǶ�
				it->ship_coords[0] = it->wait_pos[0] / (CT_WIDTH / 10.0f) - 5.0f;
				it->ship_coords[1] = -it->wait_pos[1] / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				switch (it->berth_idx)
				{
				case 0:
					//21��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_21));
					break;
				case 1:
					//22��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_22));
					break;
				case 2:
					//4�Ҳ�λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_2));
					break;
				case 3:
					//4��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_1));
					break;
				case 4:
					//23��λ
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_23));
					break;
				default:
					break;
				}
				//���ô���ɫλ
				it->ship_coords[3] = 4.0f;
			}
			if (it->ship_coords[0] > 5.0f || it->ship_coords[0] < -5.0f || it->ship_coords[1]>3.0f || it->ship_coords[1] < -3.0f)
			{
				//�����뿪���Ʒ�Χ����ʧ
				it = this->ships.erase(it);
				temp = false;
			}
			else
			{
				//��������ԭ·����
				it->ship_coords[0] -= (float)(speed_ship * cos(it->angle)) * simurate;
				it->ship_coords[1] -= (float)(speed_ship * sin(it->angle)) * simurate;
			}
		}
		if (temp)
		{
			it++;
		}
	}
	return ship_ret;
}

void Berth::add_type(std::string str_name, int type_type)
{
	int lst_idx = 1;		//ԭ��Ӧ������С����������
	for (std::vector<Type_ship>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
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

bool Berth::set_unloading_ship(Message& message, std::vector<std::string>& equipments)
{
	bool ret = false;
	std::vector<std::string>::const_iterator it1 = equipments.begin();
	for (std::vector<Ship>::const_iterator it2 = this->ships.begin(); it2 != this->ships.end(); it2++)
	{
		if (it2->berth_idx < 2 && it2->ship_state == 3 && (*it1 == "BC20A" || *it1 == "BC20B"))
		{
			ret = true;
			break;
		}
		else if ((it2->berth_idx == 2 || it2->berth_idx == 3) && it2->ship_state == 3 && (*it1 == "BC1A" || *it1 == "BC1B"))
		{
			ret = true;
			break;
		}
	}
	if (!ret)
	{
		//std::cout << "��������ʧ��::��λû�д���" << std::endl;
		message.push_message(u8"��������ʧ��::��λû�д���");
	}
	return ret;
}

bool Berth::set_loading_ship(Message& message, int load_type, int load_index)
{
	bool ret = true;
	for (std::vector<Ship>::iterator it1 = this->ships.begin(); it1 != this->ships.end(); it1++)
	{
		if (it1->ship_state == 3 && it1->berth_idx == 4)
		{
			if ((it1->ship_current_storage < it1->ship_total_storage * MAX_START_LAOD) && ((it1->ship_type == 0 && it1->ship_index == 0) || (it1->ship_type == load_type && it1->ship_index == load_index)))
			{
				//����װ��
				it1->ship_type = load_type;
				it1->ship_index = load_index;
				ret = true;
			}
			else
			{
				//����ʧ��
				//std::cout << "װ������ʧ��::�������Ͳ�ƥ��" << std::endl;
				message.push_message(u8"װ������ʧ��::�������Ͳ�ƥ��");
				ret = false;
			}
			break;
		}
	}
	return ret;
}

void Berth::run_unloader_unloaded(Message& message, std::vector<std::string>& equipments)
{
	//������Ӧж������װ����Ϊ��ɫ
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		if (*it1 == "BC20A" || *it1 == "BC20B")
		{
			bool docked = false;				//Ϊfalse���޴���������������״̬
			bool dock_21 = false;				//�Ƿ�21�Ҳ�λ�п���
			bool dock_22 = false;				//�Ƿ�22��λ�п���
			//�ж��Ƿ���ڿ�����
			for (std::vector<Ship>::const_iterator it3 = this->ships.begin(); it3 != this->ships.end(); it3++)
			{
				if (it3->ship_state == 3 && it3->berth_idx == 0)
				{
					//21��λ
					docked = true;
					dock_21 = true;
				}
				else if (it3->ship_state == 3 && it3->berth_idx == 1)
				{
					//22��λ
					docked = true;
					dock_22 = true;
				}
			}
			if (docked)
			{
				//ͨ�ò�λ����׼��
				for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end() && it2->unloader_index < 4; it2++)
				{
					if (dock_21 && it2->unloader_index < 2)
					{
						//1#��2#�Ż�����
						it2->unloader_state = 1;
						this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
						if (*it1 == "BC20A")
						{
							//AƤ��
							it2->crab_bucket_switch = false;
						}
						else
						{
							//BƤ��
							it2->crab_bucket_switch = true;
						}
					}
					else if (dock_22 && it2->unloader_index > 1)
					{
						//3#��4#�Ż�����
						it2->unloader_state = 1;
						this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
						if (*it1 == "BC20A")
						{
							//AƤ��
							it2->crab_bucket_switch = false;
						}
						else
						{
							//BƤ��
							it2->crab_bucket_switch = true;
						}
					}
				}
			}
			else
			{
				//�޴�����������������
				//std::cout << "ж����׼��ʧ��::ͨ�ò�λ�޴�ֻ����" << std::endl;
				message.push_message(u8"ж����׼��ʧ��::ͨ�ò�λ�޴�ֻ����");
			}
		}
		else if (*it1 == "BC1A" || *it1 == "BC1B")
		{
			//4��λ����׼��
			bool docked = false;				//Ϊfalse���޴���������������״̬+
			bool dock_4_1 = false;				//�Ƿ�4��λ�п���
			bool dock_4_2 = false;				//�Ƿ�4�Ҳ�λ�п���
			//�ж��Ƿ���ڿ�����
			for (std::vector<Ship>::const_iterator it3 = this->ships.begin(); it3 != this->ships.end(); it3++)
			{
				if (it3->ship_state == 3 && it3->berth_idx == 2)
				{
					//4�Ҳ�λ
					docked = true;
					dock_4_2 = true;
				}
				else if (it3->ship_state == 3 && it3->berth_idx == 3)
				{
					//4��λ
					docked = true;
					dock_4_1 = true;
				}
			}
			if (docked)
			{
				//4��λ����׼��
				for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
				{
					if (dock_4_1 && (it2->unloader_index == 4 || it2->unloader_index == 5))
					{
						//11#��12#�Ż�������
						it2->unloader_state = 1;
						this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
						if (*it1 == "BC1A")
						{
							//AƤ��
							it2->crab_bucket_switch = false;
						}
						else
						{
							//BƤ��
							it2->crab_bucket_switch = true;
						}
					}
					else if (dock_4_2 && (it2->unloader_index == 6 || it2->unloader_index == 7))
					{
						//13#��14#�Ż�������
						it2->unloader_state = 1;
						this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
						if (*it1 == "BC1A")
						{
							//AƤ��
							it2->crab_bucket_switch = false;
						}
						else
						{
							//BƤ��
							it2->crab_bucket_switch = true;
						}
					}
				}
			}
			else
			{
				//�޴�����������������
				//std::cout << "ж����׼��ʧ��::4��λ�޴�ֻ����" << std::endl;
				message.push_message(u8"ж����׼��ʧ��::4��λ�޴�ֻ����");
			}
		}
		else if (*it1 == "SL4")
		{
			//װ����׼��
			this->loader.loader_state = 1;
			this->loader_coords[3] = 1.0f;
		}
	}
}

void Berth::run_unloader_loaded(std::vector<std::string>& equipments)
{
	//������Ӧж����Ϊ��ɫ
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		if (*it1 == "BC20A" || *it1 == "BC20B")
		{
			//ͨ�ò�λж��������
			for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end() && it2->unloader_index < 4; it2++)
			{
				//��ǰ�������ھ���״̬��ж����������ֱ�ӽ��빤��״̬
				if (it2->unloader_state == 1)
				{
					it2->unloader_state = 2;
					this->unloader_coords[5 * it2->unloader_index + 3] = 2.0f;
				}
			}
		}
		else if (*it1 == "BC1A" || *it1 == "BC1B")
		{
			//4��λж��������
			for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
			{
				if (it2->unloader_index > 3)
				{
					if (it2->unloader_state == 1)
					{
						it2->unloader_state = 2;
						this->unloader_coords[5 * it2->unloader_index + 3] = 2.0f;
					}
				}
			}
		}
		else if (*it1 == "SL4" && this->loader.loader_state == 1)
		{
			//װ��������
			this->loader.loader_state = 2;
			this->loader_coords[3] = 2.0f;
		}
	}
}

void Berth::unloader_shutDown(std::vector<std::string>& equipments)
{
	//������Ӧж����Ϊ��ɫ
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		if (*it1 == "BC20A" || *it1 == "BC20B")
		{
			//ͨ�ò�λж����ͣ��
			for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end() && it2->unloader_index < 4; it2++)
			{
				if (it2->unloader_state > 0)
				{
					it2->unloader_state = 0;
					this->unloader_coords[5 * it2->unloader_index + 3] = 0.0f;
				}
			}
		}
		else if (*it1 == "BC1A" || *it1 == "BC1B")
		{
			//4��λж����ͣ��
			for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
			{
				if (it2->unloader_state > 0 && it2->unloader_index > 3)
				{
					it2->unloader_state = 0;
					this->unloader_coords[5 * it2->unloader_index + 3] = 0.0f;
				}
			}
			break;
		}
		if (*it1 == "SL4" && this->loader.loader_state > 0)
		{
			//װ����ͣ��
			this->loader.loader_state = 0;
			this->loader_coords[3] = 0.0f;
		}
	}
}

int Berth::get_up_type()
{
	int ret = -1;
	for (std::vector<Ship>::const_iterator it1 = this->ships.begin(); it1 != this->ships.end(); it1++)
	{
		if (it1->berth_idx < 2 && it1->ship_state == 3)
		{
			ret = it1->ship_type;
			if (!this->unload_mode_gen)
			{
				break;
			}
		}
	}
	return ret;
}

int Berth::get_up_index()
{
	int ret = -1;
	for (std::vector<Ship>::const_iterator it1 = this->ships.begin(); it1 != this->ships.end(); it1++)
	{
		if (it1->berth_idx < 2 && it1->ship_state == 3)
		{
			ret = it1->ship_index;
			if (!this->unload_mode_gen)
			{
				break;
			}
		}
	}
	return ret;
}

float Berth::get_up_amount()
{
	float ret = 0.0f;
	for (std::vector<Ship>::const_iterator it1 = this->ships.begin(); it1 != this->ships.end(); it1++)
	{
		if (it1->berth_idx < 2 && it1->ship_state == 3)
		{
			ret = (float)it1->ship_current_storage;
			if (!this->unload_mode_gen)
			{
				break;
			}
		}
	}
	return ret;
}

int Berth::get_down_type()
{
	int ret = -1;
	for (std::vector<Ship>::const_iterator it1 = this->ships.begin(); it1 != this->ships.end(); it1++)
	{
		if ((it1->berth_idx == 2 || it1->berth_idx == 3) && it1->ship_state == 3)
		{
			ret = it1->ship_type;
			break;
		}
	}
	return ret;
}

int Berth::get_down_index()
{
	int ret = -1;
	for (std::vector<Ship>::const_iterator it1 = this->ships.begin(); it1 != this->ships.end(); it1++)
	{
		if ((it1->berth_idx == 2 || it1->berth_idx == 3) && it1->ship_state == 3)
		{
			ret = it1->ship_index;
			break;
		}
	}
	return ret;
}

float Berth::get_down_amount()
{
	float ret = -1;
	for (std::vector<Ship>::const_iterator it1 = this->ships.begin(); it1 != this->ships.end(); it1++)
	{
		if ((it1->berth_idx == 2 || it1->berth_idx == 3) && it1->ship_state == 3)
		{
			ret = (float)it1->ship_current_storage;
			break;
		}
	}
	return ret;
}

void Berth::set_focus(std::vector<std::string>& equipments)
{
	this->loader_coords[4] = 1.0f;
	std::vector<std::string>::const_iterator it2 = equipments.begin();
	//���ô����Ľ���״̬
	for (std::vector<Ship>::iterator it3 = this->ships.begin(); it3 != this->ships.end(); it3++)
	{
		it3->ship_coords[5] = 1.0f;
		if ((*it2 == "BC20A" || *it2 == "BC20B") && it3->berth_idx < 2)
		{
			it3->ship_coords[5] = 0.0f;
		}
		else if ((*it2 == "BC1A" || *it2 == "BC1B") && (it3->berth_idx == 2 || it3->berth_idx == 3))
		{
			it3->ship_coords[5] = 0.0f;
		}
		if ((*(--equipments.end()) == "SL4") && (it3->berth_idx == 4))
		{
			it3->ship_coords[5] = 0.0f;
		}
	}
	//ж������ץ��focus����
	for (std::vector<ShipUnloader>::iterator it1 = this->unloaders.begin(); it1 != this->unloaders.end(); it1++)
	{
		this->unloader_coords[5 * it1->unloader_index + 4] = 1.0f;
		this->bucket_coords[5 * it1->unloader_index + 4] = 1.0f;
		if ((*it2 == "BC20A" || *it2 == "BC20B") && it1->unloader_index < 4)
		{
			this->unloader_coords[5 * it1->unloader_index + 4] = 0.0f;
			this->bucket_coords[5 * it1->unloader_index + 4] = 0.0f;
		}
		else if ((*it2 == "BC1A" || *it2 == "BC1B") && it1->unloader_index > 3)
		{
			this->unloader_coords[5 * it1->unloader_index + 4] = 0.0f;
			this->bucket_coords[5 * it1->unloader_index + 4] = 0.0f;
		}
		if (*(--equipments.end()) == "SL4")
		{
			this->loader_coords[4] = 0.0f;
		}
	}
}

void Berth::lose_focus()
{
	for (std::vector<Ship>::iterator it1 = this->ships.begin(); it1 != this->ships.end(); it1++)
	{
		it1->ship_coords[5] = 0.0f;
	}
	this->loader_coords[4] = 0.0f;
	for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
	{
		this->unloader_coords[5 * it2->unloader_index + 4] = 0.0f;
		this->bucket_coords[5 * it2->unloader_index + 4] = 0.0f;
	}
}

int Berth::get_ship_type(float total_storage)
{
	//���ش�����λ����
	if (total_storage < 5000.0f)
	{
		return 1;
	}
	else if (total_storage < 20000.0f)
	{
		return 4;
	}
	else if (total_storage < 35000.0f)
	{
		return 7;
	}
	else if (total_storage < 50000.0f)
	{
		return 10;
	}
	else if (total_storage < 65000.0f)
	{
		return 14;
	}
	return -1;
}

float Berth::get_ship_level(float total_storage)
{
	if (total_storage < 5000.0f)
	{
		return 0.7f;
	}
	else if (total_storage < 20000.0f)
	{
		return 0.9f;
	}
	else if (total_storage < 35000.0f)
	{
		return 1.1f;
	}
	else if (total_storage < 50000.0f)
	{
		return 1.3f;
	}
	else if (total_storage < 65000.0f)
	{
		return 1.5f;
	}
	return 0.0f;
}

bool Berth::type_check(int type, int index)
{
	bool ret = false;
	for (std::vector<Type_ship>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
	{
		if (it->type_type == type && it->type_index == index)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

void Berth::bucket_unload(float simurate, ShipUnloader& unloader)
{
	int berth_index = 0;	//0Ϊ21�Ų�λ��1Ϊ22�Ų�λ��2Ϊ4�Ų�λ�ң�3Ϊ4�Ų�λ��
	switch (unloader.unloader_index)
	{
	case 0:
		berth_index = 0;
		break;
	case 1:
		berth_index = 0;
		break;
	case 2:
		berth_index = 1;
		break;
	case 3:
		berth_index = 1;
		break;
	case 4:
		berth_index = 3;
		break;
	case 5:
		berth_index = 3;
		break;
	case 6:
		berth_index = 2;
		break;
	case 7:
		berth_index = 2;
		break;
	default:
		break;
	}
	this->berth_finished = -1;
	for (std::vector<Ship>::iterator it2 = this->ships.begin(); it2 != this->ships.end(); it2++)
	{
		if (it2->berth_idx == berth_index)
		{
			it2->ship_current_storage -= FLUX_BUCKET * simurate;
			if (it2->ship_current_storage < 0.0)
			{
				//ж��
				it2->ship_current_storage = 0.0;
				this->berth_finished = berth_index + 4 * unloader.crab_bucket_switch;
			}
			break;
		}
	}
}

void Berth::pre_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}

void Berth::post_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}
