#include "Berth.h"

#define CT_WIDTH 4100
#define CT_HEIGHT 2370
#define SCR_WIDTH 1440
#define SCR_HEIGHT 810
#define MAX_GENBERTH_CAPACITY 14	//通用泊位最高总靠泊能力
#define MAX_START_LAOD 0.9f			//允许装船的当前船最大初始容量比例
#define FLUX_LAOD 0.0022f				//装船速率
#define FLUX_BUCKET 0.001f			//抓斗速率

#define layer_unloader 0.0055f			//卸船机图层
#define layer_bucket 0.0058f			//抓斗图层
#define layer_ship 0.0054f				//船图层
#define unloader_pow 1915			//卸船机运行功率
#define bucket_speed_hor 0.01f		//抓斗水平移动速度
#define bucket_speed_ver 0.01f		//抓斗垂直移动速度
#define bucket_speed_up 0.02f			//抓斗皮带空载起升速度
#define bucket_speed_reset 0.04f		//抓斗水平移向闲置位速度
constexpr auto speed_ship = 0.005f;	//船舶移动速度;

Berth::Berth()
{
	this->berth_finished = -1;
	this->unload_mode_gen = false;
	this->webUnloaders[0] = 0;
	this->webUnloaders[1] = 0;
	this->webUnloaders[2] = 0;
	this->webUnloaders[3] = 0;
	//初始化名称容器
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
	//卸船机初始化
	std::string info_unloader[8];
	info_unloader[0] = "1#,3870,390";
	info_unloader[1] = "2#,3750,390";
	info_unloader[2] = "3#,3630,390";
	info_unloader[3] = "4#,3510,390";
	info_unloader[4] = "11#,1840,1950";
	info_unloader[5] = "12#,1960,1950";
	info_unloader[6] = "13#,2080,1950";
	info_unloader[7] = "14#,2200,1950";
	//泊位数据
	const float bias = 30.0f;
	this->berth_pos.x_4_1 = 1900.0f - bias;
	this->berth_pos.x_4_2 = 2140.0f - bias;
	this->berth_pos.x_21 = 3810.0f - bias;
	this->berth_pos.x_22 = 3570.0f - bias;
	this->berth_pos.x_23 = 2870.0f - bias;
	//门机数据
	this->unloader.crab_bucket_switch = false;
	this->unloader.crab_bucket_hor = -1.0f;
	this->unloader.crab_bucket_ver = 1.0f;
	this->unloader.unloader_state = 0;	
	this->unloader.crab_bucket_loaded = false;
	//抓斗数据
	this->bucket_up.ship = 290;
	this->bucket_up.conv_A = 535;
	this->bucket_up.conv_B = 465;
	this->bucket_up.idle = 600;
	this->bucket_down.ship = 2070;
	this->bucket_down.conv_A = 1880;
	this->bucket_down.conv_B = 1805;
	this->bucket_down.idle = 1730;
	//卸船机与抓斗数据读取
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
				//初始化，闲置
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
	/*buffers for 卸船机 & 抓斗*/
	//卸船机
	glGenVertexArrays(1, &unloaderVAO);
	glGenBuffers(1, &unloaderVBO);
	glBindVertexArray(unloaderVAO);
	glBindBuffer(GL_ARRAY_BUFFER, unloaderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unloader_coords), &unloader_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//抓斗
	glGenVertexArrays(1, &bucketVAO);
	glGenBuffers(1, &bucketVBO);
	glBindVertexArray(bucketVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bucketVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bucket_coords), &bucket_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//装船机初始化
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

	//船初始化
	std::string info_ship_1 = u8"华江11,金亿28,安达盛,钱鸿66,华盛161,联合17,嘉宁山,戈尔德,长顺7,盛达8,宁丰1,联合7,富兴22,联合11,明州76,瑞宁6,联合5,明州59,绿能1,明州20,明州58,富兴11,明州57,东成山,富兴21,远大之星,德勤108,敬业2,联合7,浙能5,新一海3,国铨,广州发展2,利电6";
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
	//21泊位
	this->ship.ship_name = u8"瑞宁6";
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
	//4泊位
	this->ship.ship_name = u8"宁丰1";
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
	//22泊位
	this->ship.ship_name = u8"明州57";
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
	//23泊位
	this->ship.ship_name = u8"联合5";
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
	//重置卸船机
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
	//重置装船机
	this->loader.loader_state = 0;
	//重置船舶
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
	//统计三大类货物总数
	int count_types[3] = { 0,0,0 };
	for (std::vector<Type_ship>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
	{
		count_types[it->type_type - 1]++;
	}
	int num_ships = core.genRandomInt(1, 6);
	int ship_idx = 0;
	while (num_ships > 0)
	{
		//随机初始化船舶
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
			//先判定分配的泊位，根据泊位生成锚地位置、船舶状态
			this->ship.berth_idx = core.genRandomInt(0, 4);
			//判定船舶状态，泊位已有船舶且state为2或3或4者不能重复设为2或3或4（初始离泊状态的船舶仍然视为占用泊位，这一点和实时系统中不同）
			do
			{
				this->ship.ship_state = core.genRandomInt(0, 4);
			} while (this->ship.ship_state > 1 && this->berth_occupied[this->ship.berth_idx] == 1);
			//在船舶状态为占泊的状态下，判定当前占泊是否合理
			if (this->ship.ship_state > 1 && this->ship.berth_idx < 2)
			{
				//21泊位和22泊位的靠泊申请
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
							//当前已有船占用21泊位
							ship_21 = 1;
							current_capacity += this->get_ship_type(it3->ship_total_storage);
						}
						else if (it3->berth_idx == 1)
						{
							//当前已有船占用22泊位
							ship_22 = 1;
							current_capacity += this->get_ship_type(it3->ship_total_storage);
						}
					}
				}
				if (ship_21 == 1 && ship_22 == 1)
				{
					//通用泊位被占用，驳回靠泊请求
					enBerth = false;
				}
				else if (ship_21 == 0 && ship_22 == 0)
				{
					//通用泊位为空，允许靠泊
					enBerth = true;
				}
				else if (ship_21 + ship_22 == 1)
				{
					//通用泊位有1船舶
					if (this->ship.berth_idx == 0 && ship_21 == 1)
					{
						//21泊位被占用
						enBerth = false;
					}
					else if (this->ship.berth_idx == 1 && ship_22 == 1)
					{
						//22泊位被占用
						enBerth = false;
					}
					else if (ship_type + current_capacity > MAX_GENBERTH_CAPACITY)
					{
						//超过承载上限
						enBerth = false;
					}
				}
			}
			else if (this->ship.ship_state > 1 && this->ship.berth_idx != 4)
			{
				//申请4泊位靠泊，4泊位同时只能停一条船
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
							//有船舶正在占用4泊位
							enBerth = false;
							break;
						}
					}
				}
			}
			else if (this->ship.ship_state > 1 && this->ship.berth_idx == 4)
			{
				//申请23泊位靠泊，船舶吨位需要小于5000
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
							//有船舶正在占用23泊位
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
		//对于23泊位来说，靠泊前船舶为空船，靠泊中船舶容量随机，离泊时船舶容量大于80%且随机；对于其它泊位来说，靠泊前容量大于80%且随机，靠泊中船舶容量随机，离泊时船舶为空船
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
		//生成锚地
		this->ship.wait_pos[0] = core.genRandomFloat(0.0f, (float)CT_WIDTH);
		if (this->ship.berth_idx == 2 || this->ship.berth_idx == 3)
		{
			//4号泊位2050
			this->ship.wait_pos[1] = core.genRandomFloat(2200.0f, (float)CT_HEIGHT);
		}
		else
		{
			//上泊位350
			this->ship.wait_pos[1] = core.genRandomFloat(0.0f, 200.0f);
		}
		//生成坐标（正在靠泊的船舶的初始位置在锚地，正在离泊的船舶位置在泊位）
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
				//21泊位
				this->ship.ship_coords[0] = this->berth_pos.x_21 / (CT_WIDTH / 10.0f) - 5.0f;
				this->ship.ship_coords[1] = -(float)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 1:
				//22泊位
				this->ship.ship_coords[0] = this->berth_pos.x_22 / (CT_WIDTH / 10.0f) - 5.0f;
				this->ship.ship_coords[1] = -(float)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 2:
				//4右泊位
				this->ship.ship_coords[0] = this->berth_pos.x_4_2 / (CT_WIDTH / 10.0f) - 5.0f;
				this->ship.ship_coords[1] = -(float)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 3:
				//4左泊位
				this->ship.ship_coords[0] = this->berth_pos.x_4_1 / (CT_WIDTH / 10.0f) - 5.0f;
				this->ship.ship_coords[1] = -(float)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 4:
				//23泊位
				this->ship.ship_coords[0] = this->berth_pos.x_23 / (CT_WIDTH / 10.0f) - 5.0f;
				this->ship.ship_coords[1] = -(float)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			default:
				break;
			}
		}
		//生成角度，选定距离
		long double berth_x = 0.0;
		long double berth_y = 0.0;
		switch (this->ship.berth_idx)
		{
		case 0:
			//21泊位
			this->ship.angle = atan2((this->ship.wait_pos[1] - this->bucket_up.ship), -(this->ship.wait_pos[0] - this->berth_pos.x_21));
			berth_x = (long double)this->berth_pos.x_21 / (CT_WIDTH / 10.0f) - 5.0f;
			berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
			break;
		case 1:
			//22泊位
			this->ship.angle = atan2((this->ship.wait_pos[1] - this->bucket_up.ship), -(this->ship.wait_pos[0] - this->berth_pos.x_22));
			berth_x = (long double)this->berth_pos.x_22 / (CT_WIDTH / 10.0f) - 5.0f;
			berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
			break;
		case 2:
			//4右泊位
			this->ship.angle = atan2((this->ship.wait_pos[1] - this->bucket_down.ship), -(this->ship.wait_pos[0] - this->berth_pos.x_4_2));
			berth_x = (long double)this->berth_pos.x_4_2 / (CT_WIDTH / 10.0f) - 5.0f;
			berth_y = -(long double)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
			break;
		case 3:
			//4左泊位
			this->ship.angle = atan2((this->ship.wait_pos[1] - this->bucket_down.ship), -(this->ship.wait_pos[0] - this->berth_pos.x_4_1));
			berth_x = (long double)this->berth_pos.x_4_1 / (CT_WIDTH / 10.0f) - 5.0f;
			berth_y = -(long double)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
			break;
		case 4:
			//23泊位
			berth_x = (long double)this->berth_pos.x_23 / (CT_WIDTH / 10.0f) - 5.0f;
			berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
			this->ship.angle = atan2((this->ship.wait_pos[1] - this->bucket_up.ship), -(this->ship.wait_pos[0] - this->berth_pos.x_23));
			break;
		default:
			break;
		}
		//距离生成
		this->ship.ship_dist = hypot((this->ship.ship_coords[0] - berth_x), (this->ship.ship_coords[1] - berth_y));
		//其它属性生成
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
	//卸船机
	unloaderShader.use();
	//VBO绑定
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
	//抓斗
	bucketShader.use();
	//VBO绑定
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
	//VBO绑定
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
	//小类
	static int cargo_index = 1;
	//大类
	static int cargo_type = 1;

	//船只调度台
	if (ImGui::CollapsingHeader(u8"船舶入港申请流程"))
	{
		ImGui::Indent();
		ImGui::InputText(u8"船舶舷号输入", name, 50);	//当前允许舷号相同
		ImGui::Separator();
		ImGui::Text(u8"锚地选择：");
		ImGui::SameLine();
		ImGui::RadioButton(u8"21号泊位", &berthIdx, 0);
		ImGui::SameLine();
		ImGui::RadioButton(u8"22号泊位", &berthIdx, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"4号泊位右", &berthIdx, 2);
		ImGui::SameLine();
		ImGui::RadioButton(u8"4号泊位左", &berthIdx, 3);
		ImGui::SameLine();
		ImGui::RadioButton(u8"23号泊位", &berthIdx, 4);
		ImGui::SliderInt(u8"锚地x坐标", &wait_pos_1, 0, CT_WIDTH);
		ImGui::SliderInt(u8"锚地y坐标", &wait_pos_2, 0, CT_HEIGHT);
		ImGui::Separator();
		ImGui::InputInt(u8"船舶总载货量", &total_storage, 1, 200);
		ImGui::InputInt(u8"船舶当前载货量", &current_storage, 1, 200);
		ImGui::Separator();
		ImGui::Text(u8"船舶货物大类：");
		ImGui::SameLine();
		ImGui::RadioButton(u8"按类分", &cargo_type, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"按来源分", &cargo_type, 2);
		ImGui::SameLine();
		ImGui::RadioButton(u8"按企业分", &cargo_type, 3);
		ImGui::Text(u8"选择货物小类：");
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
		if (ImGui::Button(u8"申请入港"))
		{
			bool enable = true;		//进港允许变量
			if (enable && name == "")
			{
				enable = false;
				//std::cout << "进港申请失败::舷号为空" << std::endl;
				message.push_message(u8"进港申请失败::舷号为空");
			}
			if (enable && berthIdx == 2 || berthIdx == 3)
			{
				//下泊位锚地
				if (wait_pos_2 < 2050)
				{
					enable = false;
					//std::cout << "进港申请失败::锚地非法" << std::endl;
					message.push_message(u8"进港申请失败::锚地非法");
				}
			}
			else
			{
				//上泊位锚地
				if (wait_pos_2 > 350)
				{
					enable = false;
					//std::cout << "进港申请失败::锚地非法" << std::endl;
					message.push_message(u8"进港申请失败::锚地非法");
				}
			}
			if (enable && this->type_check(cargo_type, cargo_index) == false)
			{
				enable = false;
				//std::cout << "进港申请失败::货物种类错误" << std::endl;
				message.push_message(u8"进港申请失败::货物种类错误");
			}
			if (enable && total_storage <= 0 || current_storage <= 0)
			{
				enable = false;
				//std::cout << "进港申请失败::载货量必须需为正" << std::endl;
				message.push_message(u8"进港申请失败::载货量必须需为正");
			}
			if (enable && total_storage < current_storage)
			{
				enable = false;
				//std::cout << "进港申请失败::禁止超载" << std::endl;
				message.push_message(u8"进港申请失败::禁止超载");
			}
			if (enable && total_storage > 65000)
			{
				enable = false;
				//std::cout << "进港申请失败::船舶超过泊位能力上限" << std::endl;
				message.push_message(u8"进港申请失败::船舶超过泊位能力上限");
			}
			if (enable && (berthIdx == 2 || berthIdx == 3) && total_storage > 20000)
			{
				enable = false;
				//std::cout << "进港申请失败::船舶吨位超过4泊位承载力" << std::endl;
				message.push_message(u8"进港申请失败::船舶吨位超过4泊位承载力");
			}
			if (enable && berthIdx == 4 && total_storage > 5000)
			{
				enable = false;
				//std::cout << "进港申请失败::船舶吨位超过23泊位承载力" << std::endl;
				message.push_message(u8"进港申请失败::船舶吨位超过23泊位承载力");
			}
			if (enable)
			{
				//允许进港
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
				//货物类别判定
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
				//std::cout << "船舶进港申请成功！" << std::endl;
				message.push_message(u8"船舶进港申请成功！");
			}
		}
		this->post_button();
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader(u8"船舶调度台"))
	{
		ImGui::Indent();
		std::string name_harbor = "";
		ImGui::Text(u8"选择通用泊位靠泊选择模式：");
		ImGui::SameLine();
		if (this->unload_mode_gen)
		{
			ImGui::Checkbox(u8"后进先出模式（LIFO）", &mode_gen);
			this->unload_mode_gen = mode_gen;
		}
		else
		{
			ImGui::Checkbox(u8"先进先出模式（FIFO）", &mode_gen);
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
				ImGui::Text((u8"船舶名：" + it->ship_name + u8" （当前未到港）").c_str());
				ImGui::Text((u8"分配泊位：" + name_harbor).c_str());
				for (std::vector<Type_ship>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->ship_type && it2->type_index == it->ship_index)
					{
						ImGui::Text((u8"船舶载货种类：" + it2->type_name).c_str());
						break;
					}
				}
				this->pre_button();
				if (ImGui::Button(u8"至锚地等待"))
				{
					it->ship_state = 1;
					it->ship_coords[3] = 1.0f;
				}
				ImGui::SameLine();
				if (ImGui::Button((it->ship_name + u8"立即离港##1").c_str()))
				{
					it = this->ships.erase(it);
					temp = false;
				}
				this->post_button();
				break;
			case 1:
				ImGui::Text((u8"船舶名：" + it->ship_name + u8" （当前在锚地）").c_str());
				ImGui::Text((u8"分配泊位：" + name_harbor).c_str());
				for (std::vector<Type_ship>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->ship_type && it2->type_index == it->ship_index)
					{
						ImGui::Text((u8"船舶载货种类：" + it2->type_name).c_str());
						break;
					}
				}
				this->pre_button();
				if (ImGui::Button(u8"允许靠泊"))
				{
					bool enBerth = true;
					if (it->berth_idx < 2)
					{
						//21泊位和22泊位的靠泊申请
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
									//当前已有船占用21泊位
									ship_21 = 1;
									current_capacity += this->get_ship_type(it3->ship_total_storage);
								}
								else if (it3->berth_idx == 1)
								{
									//当前已有船占用22泊位
									ship_22 = 1;
									current_capacity += this->get_ship_type(it3->ship_total_storage);
								}
							}
						}
						if (ship_21 == 1 && ship_22 == 1)
						{
							//通用泊位被占用，驳回靠泊请求
							enBerth = false;
							//std::cout << "靠泊申请失败::通用泊位被全部占用" << std::endl;
							message.push_message(u8"靠泊申请失败::通用泊位被全部占用");
						}
						else if (ship_21 == 0 && ship_22 == 0)
						{
							//通用泊位为空，允许靠泊
							enBerth = true;
						}
						else if (ship_21 + ship_22 == 1)
						{
							//通用泊位有1船舶
							if (berthIdx == 0 && ship_21 == 1)
							{
								//21泊位被占用
								enBerth = false;
								//std::cout << "靠泊申请失败::21泊位被占用" << std::endl;
								message.push_message(u8"靠泊申请失败::21泊位被占用");
							}
							else if (berthIdx == 1 && ship_22 == 1)
							{
								//22泊位被占用
								enBerth = false;
								//std::cout << "靠泊申请失败::22泊位被占用" << std::endl;
								message.push_message(u8"靠泊申请失败::22泊位被占用");
							}
							else if (ship_type + current_capacity > MAX_GENBERTH_CAPACITY)
							{
								//超过承载上限
								enBerth = false;
								//std::cout << "靠泊申请失败::申请超过泊位靠泊能力上限" << std::endl;
								message.push_message(u8"靠泊申请失败::申请超过泊位靠泊能力上限");
							}
						}
					}
					else if (it->berth_idx != 4)
					{
						//申请4泊位靠泊，4泊位同时只能停一条船
						for (std::vector<Ship>::const_iterator it = this->ships.begin(); it != this->ships.end(); it++)
						{
							if ((it->ship_state == 2 || it->ship_state == 3) && (it->berth_idx == 2 || it->berth_idx == 3))
							{
								//有船舶正在占用4泊位
								enBerth = false;
								//std::cout << "靠泊申请失败::4泊位被占用" << std::endl;
								message.push_message(u8"靠泊申请失败::4泊位被占用");
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
				if (ImGui::Button((it->ship_name + u8"立即离港##2").c_str()))
				{
					it->ship_state = 4;
					it->ship_coords[3] = 4.0f;
				}
				this->post_button();
				break;
			case 2:
				ImGui::Text((u8"船舶名：" + it->ship_name + u8" （当前正在靠泊）").c_str());
				ImGui::Text((u8"占用泊位：" + name_harbor).c_str());
				for (std::vector<Type_ship>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->ship_type && it2->type_index == it->ship_index)
					{
						ImGui::Text((u8"船舶载货种类：" + it2->type_name).c_str());
						break;
					}
				}
				this->pre_button();
				if (ImGui::Button((it->ship_name + u8"立即离港##3").c_str()))
				{
					it->ship_state = 4;
					it->ship_coords[3] = 4.0f;
				}
				this->post_button();
				break;
			case 3:
				ImGui::Text((u8"船舶名：" + it->ship_name + u8" （当前位于泊位）").c_str());
				ImGui::Text((u8"分配泊位：" + name_harbor).c_str());
				for (std::vector<Type_ship>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->ship_type && it2->type_index == it->ship_index)
					{
						ImGui::Text((u8"船舶载货种类：" + it2->type_name).c_str());
						break;
					}
				}
				this->pre_button();
				if (ImGui::Button((it->ship_name + u8"立即离港##4").c_str()))
				{
					it->ship_state = 4;
					it->ship_coords[3] = 4.0f;
					//停止对应泊位的卸船机和抓斗
					int num_gen_ships = 0;
					switch (it->berth_idx)
					{
					case 0:
						//21泊位
						for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
						{
							if (it2->unloader_index < 2 && it2->unloader_state == 2)
							{
								it2->unloader_state = 1;
								this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
							}
						}
						//如果通用泊位不再有船舶，就发送流程停止指令
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
						//22泊位
						for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
						{
							if (it2->unloader_index > 1 && it2->unloader_index < 4 && it2->unloader_state == 2)
							{
								it2->unloader_state = 1;
								this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
							}
						}
						//如果通用泊位不再有船舶，就发送流程停止指令
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
						//4右泊位
						for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
						{
							if ((it2->unloader_index == 6 || it2->unloader_index == 7) && it2->unloader_state == 2)
							{
								it2->unloader_state = 1;
								this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
								ret = it->berth_idx;
							}
						}
						//4泊位不能同时有两条船靠泊，所以直接中断流程
						ret = it->berth_idx;
						break;
					case 3:
						//4左泊位
						for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
						{
							if ((it2->unloader_index == 4 || it2->unloader_index == 5) && it2->unloader_state == 2)
							{
								it2->unloader_state = 1;
								this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
								ret = it->berth_idx;
							}
						}
						//4泊位不能同时有两条船靠泊，所以直接中断流程
						ret = it->berth_idx;
						break;
					case 4:
						//23泊位
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
				ImGui::Text((u8"船舶名：" + it->ship_name + u8" （当前正在离港）").c_str());
				ImGui::Text((u8"原泊位：" + name_harbor).c_str());
				for (std::vector<Type_ship>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->ship_type && it2->type_index == it->ship_index)
					{
						ImGui::Text((u8"船舶载货种类：" + it2->type_name).c_str());
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
	static float set_hor = 0.0f;		//强制设定抓斗水平位置
	static float set_ver = 0.0f;		//强制设定抓斗垂直位置
	static int set_state = 1;			//强制设定卸船机运行状态
	static bool belt_switch = false;	//强制设定卸船机卸料口方向
	static int choosed_idx = -1;		//选择要指定的卸船机序号
	if (ImGui::CollapsingHeader(u8"卸船机控制台"))
	{
		ImGui::Indent();
		for (std::vector<ShipUnloader>::iterator it1 = this->unloaders.begin(); it1 != this->unloaders.end(); it1++)
		{
			ImGui::Text((it1->unloader_name + u8"卸船机").c_str());
			ImGui::Text(u8"当前状态：");
			ImGui::SameLine();
			if (it1->unloader_state == 0)
			{
				ImGui::Text(u8"未运行");
			}
			else if (it1->unloader_state == 1)
			{
				ImGui::Text(u8"设备就绪");
			}
			else if (it1->unloader_state == 2)
			{
				ImGui::Text(u8"负载运行");
			}
			ImGui::Text((u8"抓斗水平位置：" + std::to_string(it1->crab_bucket_hor)).c_str());
			ImGui::Text((u8"抓斗垂直位置：" + std::to_string(it1->crab_bucket_ver)).c_str());
			if (it1->crab_bucket_switch)
			{
				ImGui::Text(u8"当前打到B皮带");
			}
			else
			{
				ImGui::Text(u8"当前打到A皮带");
			}
			if (it1->crab_bucket_loaded)
			{
				ImGui::Text(u8"抓斗已取料");
			}
			else
			{
				ImGui::Text(u8"抓斗未取料");
			}
			this->pre_button();
			if (ImGui::Button((u8"选择指定" + it1->unloader_name + u8"卸船机的参数").c_str()))
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
				ImGui::Text((u8"强制指定" + it1->unloader_name + u8"卸船机的运行状态：").c_str());
				if (set_state == 0)
				{
					ImGui::SliderInt((it1->unloader_name + u8"停机").c_str(), &set_state, 0, 2);
				}
				else if (set_state == 1)
				{
					ImGui::SliderInt((it1->unloader_name + u8"就绪").c_str(), &set_state, 0, 2);
				}
				else if (set_state == 2)
				{
					ImGui::SliderInt((it1->unloader_name + u8"运行").c_str(), &set_state, 0, 2);
				}
				this->pre_button();
				if (ImGui::Button((it1->unloader_name + u8"指定运行状态").c_str()))
				{
					it1->unloader_state = set_state;
					this->unloader_coords[5 * it1->unloader_index + 3] = (float)set_state;
				}
				this->post_button();
				ImGui::Text((u8"强制指定" + it1->unloader_name + u8"卸船机抓斗的水平位置：").c_str());
				ImGui::SliderFloat((it1->unloader_name + u8"水平位置").c_str(), &set_hor, -1.0f, 1.0f);
				this->pre_button();
				if (ImGui::Button((it1->unloader_name + u8"指定抓斗的水平位置").c_str()))
				{
					it1->crab_bucket_hor = set_hor;
				}
				this->post_button();
				ImGui::Text((u8"强制指定" + it1->unloader_name + u8"卸船机抓斗的垂直位置：").c_str());
				ImGui::SliderFloat((it1->unloader_name + u8"垂直位置").c_str(), &set_ver, 0.0f, 1.0f);
				this->pre_button();
				if (ImGui::Button((it1->unloader_name + u8"指定抓斗的垂直位置").c_str()))
				{
					it1->crab_bucket_ver = set_ver;
				}
				this->post_button();
				ImGui::Text((u8"强制指定" + it1->unloader_name + u8"卸船机抓斗的垂直位置：").c_str());
				if (belt_switch)
				{
					ImGui::Checkbox((it1->unloader_name + u8"打到B皮带").c_str(), &belt_switch);
				}
				else
				{
					ImGui::Checkbox((it1->unloader_name + u8"打到A皮带").c_str(), &belt_switch);
				}
				this->pre_button();
				if (ImGui::Button((it1->unloader_name + u8"指定门机抓斗位于的皮带").c_str()))
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
	//抓斗状态更新代码
	//float rate_ceiling = simurate > 20.0f ? 20.0f : simurate;
	float rate_ceiling = simurate;
	for (std::vector<ShipUnloader>::iterator it = this->unloaders.begin(); it != this->unloaders.end(); it++)
	{
		//遍历卸船机，由状态确定逻辑分支
		if (it->unloader_state == 1)
		{
			//门机状态为1时将抓斗移动到闲置位置，若有料先到皮带上卸料，然后移动到闲置位置
			if (this->bucket_coords[5 * it->unloader_index + 3] < 1.0f)
			{
				//抓斗未卸完，执行卸料
				if (it->crab_bucket_hor == 0.0f)
				{
					//抓斗位于船
					if (it->crab_bucket_ver < 1.0f)
					{
						//抓斗未升到位，y不变，hor向上，抓斗有煤
						it->crab_bucket_ver += 0.1f * bucket_speed_ver * rate_ceiling;
						//this->bucket_coords[5 * it->unloader_index + 3] = 0.0f;
					}
					else
					{
						//抓斗升到位，向皮带运行
						it->crab_bucket_hor += 0.1f * bucket_speed_hor * rate_ceiling;
						if (it->crab_bucket_switch == false)
						{
							//A皮带
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
							//B皮带
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
					//抓斗升到位，向皮带运行
					it->crab_bucket_hor += 0.1f * bucket_speed_hor * rate_ceiling;
					if (it->crab_bucket_switch == false)
					{
						//A皮带
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
						//B皮带
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
					//抓斗位于皮带
					if (it->crab_bucket_ver > 0.0f)
					{
						//抓斗未降到位，抓斗下降，抓斗卸煤，颜色变浅
						it->crab_bucket_ver -= 0.1f * bucket_speed_ver * rate_ceiling;
						this->bucket_coords[5 * it->unloader_index + 3] += 0.1f * bucket_speed_ver * rate_ceiling;
						//数值校正
						if (this->bucket_coords[5 * it->unloader_index + 3] > 1.0f || it->crab_bucket_ver < 0.0f)
						{
							it->crab_bucket_ver = 0.0f;
							this->bucket_coords[5 * it->unloader_index + 3] = 1.0f;
							it->crab_bucket_loaded = false;
						}
					}
					else
					{
						//抓斗降到位，返程完成
						it->crab_bucket_ver = 0.0f;
						this->bucket_coords[5 * it->unloader_index + 3] = 1.0f;
						it->crab_bucket_loaded = false;
					}
				}
			}
			else
			{
				//抓斗卸完，移动到闲置位置
				if (it->crab_bucket_ver < 1.0f)
				{
					//抓斗未升到位，向上升
					it->crab_bucket_ver += 0.1f * bucket_speed_ver * rate_ceiling;
				}
				else
				{
					//抓斗升到位，向闲置位置移动
					if (it->crab_bucket_hor > -1.0f)
					{
						//抓斗未就位，向闲置位置移动
						it->crab_bucket_hor -= 0.1f * bucket_speed_reset * rate_ceiling;
						if (it->crab_bucket_switch == false)
						{
							//A皮带
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
							//B皮带
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
			//门机状态为2时抓斗处于工作循环：移动到船，放下抓斗（取料），提升抓斗，移动到皮带，放下抓斗（卸料），提升抓斗
			if (it->crab_bucket_hor < 0.0f)
			{
				//抓斗正闲置情况下将抓斗移动到工作位置（跨过皮带，无需考虑crab_bucket_switch）
				it->crab_bucket_hor += 0.1f * bucket_speed_hor * rate_ceiling;
				//数值校正
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
				//抓斗处于工作位置，正常运作
				if (it->crab_bucket_loaded == false)
				{
					//抓斗未取料，向船运行，包含从皮带上提升，向船运行，放下的过程
					if (it->crab_bucket_hor == 1.0f)
					{
						//抓斗位于皮带
						if (it->crab_bucket_ver < 1.0f)
						{
							//抓斗未升到位，y不变，hor向上，抓斗为空，是白色
							it->crab_bucket_ver += 0.1f * bucket_speed_up * rate_ceiling;
							//数值校正
							if (it->crab_bucket_ver > 1.0f)
								it->crab_bucket_ver = 1.0f;
							//this->bucket_coords[5 * it->unloader_index + 3] = 1.0f;
						}
						else
						{
							//抓斗升到位，向船运行
							it->crab_bucket_hor -= 0.1f * bucket_speed_hor * rate_ceiling;
							if (it->crab_bucket_switch == false)
							{
								//A皮带
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
								//B皮带
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
						//抓斗向船运行
						if (it->crab_bucket_ver < 1.0f)
						{
							//抓斗未升到位，y不变，hor向上
							it->crab_bucket_ver += 0.1f * bucket_speed_up * rate_ceiling;
							//数值校正
							if (it->crab_bucket_ver > 1.0f)
								it->crab_bucket_ver = 1.0f;
						}
						else
						{
							//抓斗升到位，向船运行
							it->crab_bucket_hor -= 0.1f * bucket_speed_hor * rate_ceiling;
							//数值校正
							if (it->crab_bucket_hor < 0.0f)
								it->crab_bucket_hor = 0.0f;
							if (it->crab_bucket_switch == false)
							{
								//A皮带
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
								//B皮带
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
						//抓斗在船
						if (it->crab_bucket_ver > 0.0f)
						{
							//抓斗未降到位，抓斗下降，抓斗装煤，颜色变深
							it->crab_bucket_ver -= 0.1f * bucket_speed_ver * rate_ceiling;
							this->bucket_coords[5 * it->unloader_index + 3] -= 0.1f * bucket_speed_ver * rate_ceiling;
							//数值校正
							if (this->bucket_coords[5 * it->unloader_index + 3] < 0.0f || it->crab_bucket_ver < 0.0f)
							{
								it->crab_bucket_ver = 0.0f;
								this->bucket_coords[5 * it->unloader_index + 3] = 0.0f;
								it->crab_bucket_loaded = true;
							}
							//船卸煤
							this->bucket_unload(simurate, *it);
						}
						else
						{
							//抓斗降到位，去程完成
							it->crab_bucket_ver = 0.0f;
							this->bucket_coords[5 * it->unloader_index + 3] = 0.0f;
							it->crab_bucket_loaded = true;
						}
					}
				}
				else
				{
					//抓斗已取料，从船上升起，移动到皮带，放下
					if (it->crab_bucket_hor == 0.0f)
					{
						//抓斗位于船
						if (it->crab_bucket_ver < 1.0f)
						{
							//抓斗未升到位，y不变，hor向上，抓斗为满，是黑色
							it->crab_bucket_ver += 0.1f * bucket_speed_ver * simurate;
							//数值校正
							if (it->crab_bucket_ver > 1.0f)
								it->crab_bucket_ver = 1.0f;
							//this->bucket_coords[5 * it->unloader_index + 3] = 0.0f;
						}
						else
						{
							//抓斗升到位，向皮带运行
							it->crab_bucket_hor += 0.1f * bucket_speed_hor * rate_ceiling;
							if (it->crab_bucket_switch == false)
							{
								//A皮带
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
								//B皮带
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
						//抓斗升到位，向皮带运行
						it->crab_bucket_hor += 0.1f * bucket_speed_hor * rate_ceiling;
						//数值校正
						if (it->crab_bucket_hor > 1.0f)
							it->crab_bucket_hor = 1.0f;
						if (it->crab_bucket_switch == false)
						{
							//A皮带
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
							//B皮带
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
						//抓斗位于皮带
						if (it->crab_bucket_ver > 0.0f)
						{
							//抓斗未降到位，抓斗下降，抓斗卸煤，颜色变浅
							it->crab_bucket_ver -= 0.1f * bucket_speed_ver * rate_ceiling;
							this->bucket_coords[5 * it->unloader_index + 3] += 0.1f * bucket_speed_ver * rate_ceiling;
							//数值校正
							if (this->bucket_coords[5 * it->unloader_index + 3] > 1.0f || it->crab_bucket_ver < 0.0f)
							{
								it->crab_bucket_ver = 0.0f;
								this->bucket_coords[5 * it->unloader_index + 3] = 1.0f;
								it->crab_bucket_loaded = false;
							}
						}
						else
						{
							//抓斗降到位，返程完成
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
	//船只状态更新代码
	for (std::vector<Ship>::iterator it = this->ships.begin(); it != this->ships.end(); )
	{
		bool temp = true;
		if (it->ship_state == 0)
		{
			//未到港，跳过不更新
			it++;
			continue;
		}
		else if (it->ship_state == 1)
		{
			//位于锚地，位置不变
			if (it->ship_coords[0] == 0.0f)
			{
				//更新位置和角度
				it->ship_coords[0] = it->wait_pos[0] / (CT_WIDTH / 10.0f) - 5.0f;
				it->ship_coords[1] = -it->wait_pos[1] / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				switch (it->berth_idx)
				{
				case 0:
					//21泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_21));
					break;
				case 1:
					//22泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_22));
					break;
				case 2:
					//4右泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_2));
					break;
				case 3:
					//4左泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_1));
					break;
				case 4:
					//23泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_23));
					break;
				default:
					break;
				}
				//设置船颜色位
				it->ship_coords[3] = 1.0f;
			}
		}
		else if (it->ship_state == 2)
		{
			//正在靠泊，向泊位移动
			if (it->ship_coords[0] == 0.0f)
			{
				//更新位置和角度
				it->ship_coords[0] = it->wait_pos[0] / (CT_WIDTH / 10.0f) - 5.0f;
				it->ship_coords[1] = -it->wait_pos[1] / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				switch (it->berth_idx)
				{
				case 0:
					//21泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_21));
					break;
				case 1:
					//22泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_22));
					break;
				case 2:
					//4右泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_2));
					break;
				case 3:
					//4左泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_1));
					break;
				case 4:
					//23泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_23));
					break;
				default:
					break;
				}
				//设置船颜色位
				it->ship_coords[3] = 2.0f;
			}
			long double berth_x = 0.0;
			long double berth_y = 0.0;
			switch (it->berth_idx)
			{
			case 0:
				//21泊位
				berth_x = (long double)this->berth_pos.x_21 / (CT_WIDTH / 10.0f) - 5.0f;
				berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 1:
				//22泊位
				berth_x = (long double)this->berth_pos.x_22 / (CT_WIDTH / 10.0f) - 5.0f;
				berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 2:
				//4右泊位
				berth_x = (long double)this->berth_pos.x_4_2 / (CT_WIDTH / 10.0f) - 5.0f;
				berth_y = -(long double)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 3:
				//4左泊位
				berth_x = (long double)this->berth_pos.x_4_1 / (CT_WIDTH / 10.0f) - 5.0f;
				berth_y = -(long double)this->bucket_down.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			case 4:
				//23泊位
				berth_x = (long double)this->berth_pos.x_23 / (CT_WIDTH / 10.0f) - 5.0f;
				berth_y = -(long double)this->bucket_up.ship / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				break;
			default:
				break;
			}
			long double temp_dist = hypot((it->ship_coords[0] - berth_x), (it->ship_coords[1] - berth_y));
			if (temp_dist > it->ship_dist)
			{
				//现在距离大于之前一刻的距离，表示船在驶离泊位，改为靠泊状态
				it->ship_coords[0] = (float)berth_x;
				it->ship_coords[1] = (float)berth_y;
				it->ship_coords[3] = 3.0f;
				it->ship_coords[4] = 1.0f - it->ship_current_storage / it->ship_total_storage;
				it->ship_state = 3;
			}
			else
			{
				//船只未到位，移动
				it->ship_coords[0] += (float)(speed_ship * cos(it->angle)) * simurate;
				it->ship_coords[1] += (float)(speed_ship * sin(it->angle)) * simurate;
			}
			it->ship_dist = temp_dist;
		}
		else if (it->ship_state == 3)
		{
			//在泊位，更新料仓百分比
			if (it->ship_current_storage >= 0.0f)
			{
				it->ship_coords[4] = 1.0f - it->ship_current_storage / it->ship_total_storage;
			}
			//卸船的船储量变动
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
			//正离港
			if (it->ship_coords[0] == 0.0f)
			{
				//更新位置和角度
				it->ship_coords[0] = it->wait_pos[0] / (CT_WIDTH / 10.0f) - 5.0f;
				it->ship_coords[1] = -it->wait_pos[1] / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
				switch (it->berth_idx)
				{
				case 0:
					//21泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_21));
					break;
				case 1:
					//22泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_22));
					break;
				case 2:
					//4右泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_2));
					break;
				case 3:
					//4左泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_down.ship), -(it->wait_pos[0] - this->berth_pos.x_4_1));
					break;
				case 4:
					//23泊位
					it->angle = atan2((it->wait_pos[1] - this->bucket_up.ship), -(it->wait_pos[0] - this->berth_pos.x_23));
					break;
				default:
					break;
				}
				//设置船颜色位
				it->ship_coords[3] = 4.0f;
			}
			if (it->ship_coords[0] > 5.0f || it->ship_coords[0] < -5.0f || it->ship_coords[1]>3.0f || it->ship_coords[1] < -3.0f)
			{
				//船舶离开控制范围，消失
				it = this->ships.erase(it);
				temp = false;
			}
			else
			{
				//船舶正在原路返回
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
	int lst_idx = 1;		//原对应大类中小类序号最大者
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
		//std::cout << "启动流程失败::泊位没有船舶" << std::endl;
		message.push_message(u8"启动流程失败::泊位没有船舶");
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
				//允许装船
				it1->ship_type = load_type;
				it1->ship_index = load_index;
				ret = true;
			}
			else
			{
				//请求失败
				//std::cout << "装船请求失败::货物类型不匹配" << std::endl;
				message.push_message(u8"装船请求失败::货物类型不匹配");
				ret = false;
			}
			break;
		}
	}
	return ret;
}

void Berth::run_unloader_unloaded(Message& message, std::vector<std::string>& equipments)
{
	//设置相应卸船机或装船机为蓝色
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		if (*it1 == "BC20A" || *it1 == "BC20B")
		{
			bool docked = false;				//为false，无船靠泊，不允许变更状态
			bool dock_21 = false;				//是否21右泊位有靠泊
			bool dock_22 = false;				//是否22左泊位有靠泊
			//判断是否存在靠泊船
			for (std::vector<Ship>::const_iterator it3 = this->ships.begin(); it3 != this->ships.end(); it3++)
			{
				if (it3->ship_state == 3 && it3->berth_idx == 0)
				{
					//21泊位
					docked = true;
					dock_21 = true;
				}
				else if (it3->ship_state == 3 && it3->berth_idx == 1)
				{
					//22泊位
					docked = true;
					dock_22 = true;
				}
			}
			if (docked)
			{
				//通用泊位流程准备
				for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end() && it2->unloader_index < 4; it2++)
				{
					if (dock_21 && it2->unloader_index < 2)
					{
						//1#，2#门机就绪
						it2->unloader_state = 1;
						this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
						if (*it1 == "BC20A")
						{
							//A皮带
							it2->crab_bucket_switch = false;
						}
						else
						{
							//B皮带
							it2->crab_bucket_switch = true;
						}
					}
					else if (dock_22 && it2->unloader_index > 1)
					{
						//3#，4#门机就绪
						it2->unloader_state = 1;
						this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
						if (*it1 == "BC20A")
						{
							//A皮带
							it2->crab_bucket_switch = false;
						}
						else
						{
							//B皮带
							it2->crab_bucket_switch = true;
						}
					}
				}
			}
			else
			{
				//无船靠泊，不允许启动
				//std::cout << "卸船机准备失败::通用泊位无船只靠泊" << std::endl;
				message.push_message(u8"卸船机准备失败::通用泊位无船只靠泊");
			}
		}
		else if (*it1 == "BC1A" || *it1 == "BC1B")
		{
			//4泊位流程准备
			bool docked = false;				//为false，无船靠泊，不允许变更状态+
			bool dock_4_1 = false;				//是否4左泊位有靠泊
			bool dock_4_2 = false;				//是否4右泊位有靠泊
			//判断是否存在靠泊船
			for (std::vector<Ship>::const_iterator it3 = this->ships.begin(); it3 != this->ships.end(); it3++)
			{
				if (it3->ship_state == 3 && it3->berth_idx == 2)
				{
					//4右泊位
					docked = true;
					dock_4_2 = true;
				}
				else if (it3->ship_state == 3 && it3->berth_idx == 3)
				{
					//4左泊位
					docked = true;
					dock_4_1 = true;
				}
			}
			if (docked)
			{
				//4泊位流程准备
				for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
				{
					if (dock_4_1 && (it2->unloader_index == 4 || it2->unloader_index == 5))
					{
						//11#和12#门机部就绪
						it2->unloader_state = 1;
						this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
						if (*it1 == "BC1A")
						{
							//A皮带
							it2->crab_bucket_switch = false;
						}
						else
						{
							//B皮带
							it2->crab_bucket_switch = true;
						}
					}
					else if (dock_4_2 && (it2->unloader_index == 6 || it2->unloader_index == 7))
					{
						//13#和14#门机部就绪
						it2->unloader_state = 1;
						this->unloader_coords[5 * it2->unloader_index + 3] = 1.0f;
						if (*it1 == "BC1A")
						{
							//A皮带
							it2->crab_bucket_switch = false;
						}
						else
						{
							//B皮带
							it2->crab_bucket_switch = true;
						}
					}
				}
			}
			else
			{
				//无船靠泊，不允许启动
				//std::cout << "卸船机准备失败::4泊位无船只靠泊" << std::endl;
				message.push_message(u8"卸船机准备失败::4泊位无船只靠泊");
			}
		}
		else if (*it1 == "SL4")
		{
			//装船机准备
			this->loader.loader_state = 1;
			this->loader_coords[3] = 1.0f;
		}
	}
}

void Berth::run_unloader_loaded(std::vector<std::string>& equipments)
{
	//设置相应卸船机为绿色
	this->webUnloaders[0] = 0;
	this->webUnloaders[1] = 0;
	this->webUnloaders[2] = 0;
	this->webUnloaders[3] = 0;
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		if (*it1 == "BC20A" || *it1 == "BC20B")
		{
			//通用泊位卸船机运行
			for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end() && it2->unloader_index < 4; it2++)
			{
				//当前：不处于就绪状态的卸船机不可以直接进入工作状态
				if (it2->unloader_state == 1)
				{
					it2->unloader_state = 2;
					this->unloader_coords[5 * it2->unloader_index + 3] = 2.0f;
					this->webUnloaders[it2->unloader_index] = 1;
				}
			}
		}
		else if (*it1 == "BC1A" || *it1 == "BC1B")
		{
			//4泊位卸船机运行
			for (std::vector<ShipUnloader>::iterator it2 = this->unloaders.begin(); it2 != this->unloaders.end(); it2++)
			{
				if (it2->unloader_index > 3)
				{
					if (it2->unloader_state == 1)
					{
						it2->unloader_state = 2;
						this->unloader_coords[5 * it2->unloader_index + 3] = 2.0f;
						this->webUnloaders[it2->unloader_index] = 1;
					}
				}
			}
		}
		else if (*it1 == "SL4" && this->loader.loader_state == 1)
		{
			//装船机运行
			this->loader.loader_state = 2;
			this->loader_coords[3] = 2.0f;
		}
	}
}

void Berth::unloader_shutDown(std::vector<std::string>& equipments)
{
	//设置相应卸船机为红色
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		if (*it1 == "BC20A" || *it1 == "BC20B")
		{
			//通用泊位卸船机停机
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
			//4泊位卸船机停机
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
			//装船机停机
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
	//设置船舶的焦点状态
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
	//卸船机与抓斗focus设置
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
	//返回船舶吨位级别
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
	int berth_index = 0;	//0为21号泊位，1为22号泊位，2为4号泊位右，3为4号泊位左
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
				//卸空
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
