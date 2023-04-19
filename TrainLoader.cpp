#include "TrainLoader.h"

#define CT_WIDTH 4100
#define CT_HEIGHT 2370
#define SCR_WIDTH 1440
#define SCR_HEIGHT 810
#define THRESH_NORTH 660				//列车显示北限
#define THRESH_SOUTH 2000			//列车显示南限

#define SPEED_TRAIN_LOAD 0.008f		//火车装车速度
#define SPEED_TRAIN_LEAVE 0.02f		//火车离港速度
#define BIAS_X 100						//装车楼x坐标右偏移值
#define TRAIN_BIAS_X 100				//火车向装车楼中心x坐标偏移值
#define WAGON_LENGTH 400				//单节车皮长度
#define LAYER_building 0.0050f		//装车楼图层
#define LAYER_train 0.0040f			//火车图层
#define LOAD_SPEED 3.0f				//装车楼装车速度

#define PROB_TRAIN 0.5f				//车道有火车的概率

TrainLoader::TrainLoader()
{
	this->north_thresh = -THRESH_NORTH / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
	this->south_thresh = -THRESH_SOUTH / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
	this->train_x_bias = 2.0f * TRAIN_BIAS_X / CT_WIDTH;
	this->length_wagon = 2.0f * WAGON_LENGTH / CT_WIDTH;
	//初始化种类数据
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
	//装车楼初始化
	this->building_A.building_pow = 45;
	this->building_A.building_state = 0;
	this->building_A.building_mode = 2;
	this->building_B.building_pow = 45;
	this->building_B.building_state = 0;
	this->building_B.building_mode = 2;
	//A楼
	this->building_coords_A[0] = (3630.0f + BIAS_X) / (CT_WIDTH / 10.0f) - 5.0f;
	this->building_coords_A[1] = -1325.0f / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
	this->building_coords_A[2] = LAYER_building;
	this->building_coords_A[3] = 0.0f;
	this->building_coords_A[4] = 0.0f;
	this->building_coords_A[5] = 2.0f;
	//B楼
	this->building_coords_B[0] = (3900.0f + BIAS_X) / (CT_WIDTH / 10.0f) - 5.0f;
	this->building_coords_B[1] = -1175.0f / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
	this->building_coords_B[2] = LAYER_building;
	this->building_coords_B[3] = 0.0f;
	this->building_coords_B[4] = 0.0f;
	this->building_coords_B[5] = 2.0f;
	glGenVertexArrays(1, &buildingVAO);
	glGenBuffers(1, &buildingVBO);
	glBindVertexArray(buildingVAO);
	glBindBuffer(GL_ARRAY_BUFFER, buildingVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(building_coords_A), &building_coords_A, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//火车初始化
	this->wagon.wagon_idx = 1;
	this->wagon.amount = 0;
	this->train.train_idx = 82001;
	this->train.train_lane = 1;
	this->train.train_state = 1;
	this->train.total_num = 22;
	this->train.current_num = 1;
	this->train.max_per_wagon = 65.0f;
	//this->train.first_pos = this->north_thresh + 0.1f;
	this->train.first_pos = 0.1f;
	this->train.speed_train = SPEED_TRAIN_LEAVE;
	this->train.train_type = 1;
	this->train.train_index = 4;
	for (int i = 0; i < this->train.total_num; i++)
	{
		//车厢和VBO初始化
		this->wagon.wagon_idx = i + 1;
		this->train.wagons.push_back(this->wagon);
	}
	this->train.train_coords[0] = this->building_coords_B[0] - this->train_x_bias;
	this->train.train_coords[1] = this->train.first_pos;
	this->train.train_coords[2] = LAYER_train;
	this->train.train_coords[3] = 1.0f;
	this->train.train_coords[4] = 1.0f;
	this->train.train_coords[5] = 0.0f;
	this->train.train_coords[6] = 6.0f;
	this->trains.push_back(this->train);
	glGenVertexArrays(1, &trainVAO);
	glGenBuffers(1, &trainVBO);
	glBindVertexArray(trainVAO);
	glBindBuffer(GL_ARRAY_BUFFER, trainVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->train.train_coords), &this->train.train_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void TrainLoader::reset(SimuCore& core, bool rand_init)
{
	this->building_A.building_state = 0;
	this->building_B.building_state = 0;
	this->building_coords_A[3] = 0.0f;
	this->building_coords_B[3] = 0.0f;
	this->trains.clear();
	if (rand_init)
	{
		this->train_random_initiator(core);
	}
}

void TrainLoader::train_random_initiator(SimuCore& core)
{
	this->trains.clear();
	for (int i = 0; i < 4; i++)
	{
		this->lane_occupied[i] = 0;
		this->index_occupied[i] = 0;
	}
	int count_types[3] = { 0,0,0 };
	for (std::vector<Type_train>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
	{
		count_types[it->type_type - 1]++;
	}
	for (int i = 0; i < 4; i++)
	{
		if (core.dice(PROB_TRAIN))
		{
			this->train.train_lane = 0;
			do
			{
				this->train.train_lane = core.genRandomInt(0, 3);
			} while (this->lane_occupied[this->train.train_lane] == 1);
			this->lane_occupied[this->train.train_lane] = 1;
			do
			{
				this->train.train_idx = core.genRandomInt(82001, 84998);
			} while (this->index_occupied[this->train.train_lane] == this->train.train_idx);
			this->index_occupied[this->train.train_lane] = this->train.train_idx;
			this->train.total_num = core.genRandomInt(1, 28);
			this->train.max_per_wagon = core.genRandomFloat(62.0f, 67.0f);
			//重置后不允许出现正在装货的火车
			this->train.train_state = core.genRandomInt(0, 1);
			if (this->train.train_state == 1)
			{
				this->train.train_state++;
			}
			if (this->train.train_state == 0)
			{
				this->train.first_pos = this->north_thresh + 0.1f;
				this->train.speed_train = 0.0f;
				this->train.train_type = 0;
				this->train.train_index = 0;
			}
			else
			{
				this->train.first_pos = core.genRandomFloat(this->south_thresh, this->north_thresh);
				this->train.speed_train = SPEED_TRAIN_LEAVE;
				this->train.train_type = core.genRandomInt(1, 3);
				this->train.train_index = core.genRandomInt(1, count_types[this->train.train_type - 1]);
			}
			if (this->train.train_lane < 2)
			{
				this->train.current_num = (int)((this->building_coords_B[1] - this->train.first_pos) / length_wagon) + 1;
			}
			else
			{
				this->train.current_num = (int)((this->building_coords_A[1] - this->train.first_pos) / length_wagon) + 1;
			}
			this->train.wagons.clear();
			for (int j = 0; j < this->train.total_num; j++)
			{
				this->wagon.wagon_idx = j + 1;
				if (this->train.train_state == 0)
				{
					this->wagon.amount = 0;
				}
				else if (this->train.train_state == 2)
				{
					this->wagon.amount = core.genRandomFloat(0.8f * this->train.max_per_wagon, this->train.max_per_wagon);
				}
				//重置后不会出现正在装货的火车，这一分支可以省略
				else if (this->train.train_state == 1)
				{
					if (this->wagon.wagon_idx < this->train.current_num)
					{
						this->wagon.amount = core.genRandomFloat(0.8f * this->train.max_per_wagon, this->train.max_per_wagon);
					}
					else if (this->wagon.wagon_idx > this->train.current_num)
					{
						this->wagon.amount = 0;
					}
					else
					{
						this->wagon.amount = core.genRandomFloat(0.0f, this->train.max_per_wagon);
					}
				}
				this->train.wagons.push_back(this->wagon);
			}
			switch (this->train.train_lane)
			{
			case 0:
				this->train.train_coords[0] = this->building_coords_B[0] + this->train_x_bias;
				break;
			case 1:
				this->train.train_coords[0] = this->building_coords_B[0] - this->train_x_bias;
				break;
			case 2:
				this->train.train_coords[0] = this->building_coords_A[0] + this->train_x_bias;
				break;
			case 3:
				this->train.train_coords[0] = this->building_coords_A[0] - this->train_x_bias;
				break;
			default:
				break;
			}
			this->train.train_coords[1] = this->train.first_pos;
			this->train.train_coords[3] = (float)this->train.train_state;
			if (this->train.train_type == 0)
			{
				this->train.train_coords[6] = 0.0f;
			}
			else if (this->train.train_type == 1)
			{
				if (this->train.train_index < 7)
				{
					this->train.train_coords[6] = (float)this->train.train_index;
				}
				else
				{
					this->train.train_coords[6] = 7.0f;
				}
			}
			else
			{
				this->train.train_coords[6] = 7.0f;
			}
			this->trains.push_back(this->train);
		}
	}
}

void TrainLoader::drawBuilding(Camera& camera, Shader& buildingShader)
{
	buildingShader.use();
	glBindBuffer(GL_ARRAY_BUFFER, buildingVBO);
	float time = (float)glfwGetTime();
	float xValue = (float)(0.5 * std::sin(time) + 0.5);
	float yValue = (float)(0.5 * std::cos(time) + 0.5);
	glm::vec3 myVec;
	myVec.z = 0.31f;
	this->model = glm::mat4(1.0f);
	buildingShader.setMat4("model", this->model);
	this->projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
	buildingShader.setMat4("projection", this->projection);
	this->view = glm::mat4(1.0f);
	this->view = camera.GetViewMatrix();
	buildingShader.setMat4("view", this->view);
	glBindVertexArray(buildingVAO);
	if (this->building_A.building_mode == 0)
	{
		myVec.x = xValue;
		myVec.y = yValue;
	}
	else
	{
		myVec.x = yValue;
		myVec.y = xValue;
	}
	buildingShader.setVec3("colors", myVec);
	glBufferData(GL_ARRAY_BUFFER, sizeof(building_coords_A), &building_coords_A, GL_STATIC_DRAW);
	glDrawArrays(GL_POINTS, 0, 1);
	if (this->building_B.building_mode == 0)
	{
		myVec.x = xValue;
		myVec.y = yValue;
	}
	else
	{
		myVec.x = yValue;
		myVec.y = xValue;
	}
	buildingShader.setVec3("colors", myVec);
	glBufferData(GL_ARRAY_BUFFER, sizeof(building_coords_B), &building_coords_B, GL_STATIC_DRAW);
	glDrawArrays(GL_POINTS, 0, 1);
}

void TrainLoader::drawTrain(Camera& camera, Shader& trainShader)
{
	trainShader.use();
	glBindBuffer(GL_ARRAY_BUFFER, trainVBO);
	glBindVertexArray(trainVAO);
	this->model = glm::mat4(1.0f);
	trainShader.setMat4("model", this->model);
	this->projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
	trainShader.setMat4("projection", this->projection);
	this->view = glm::mat4(1.0f);
	this->view = camera.GetViewMatrix();
	trainShader.setMat4("view", this->view);
	for (std::vector<TrainAttrib>::iterator it1 = this->trains.begin(); it1 != this->trains.end(); it1++)
	{
		if (it1->train_state != -1)
		{
			for (std::vector<One_wagon>::iterator it2 = it1->wagons.begin(); it2 != it1->wagons.end(); it2++)
			{
				//车皮y坐标计算
				it1->train_coords[1] = it1->first_pos + (it2->wagon_idx - 1) * this->length_wagon;
				if (it1->train_coords[1] < this->north_thresh && it1->train_coords[1] > this->south_thresh)
				{
					//不超过范围的车皮绘制
					it1->train_coords[4] = (1.0f - it2->amount / it1->max_per_wagon);
					glBufferData(GL_ARRAY_BUFFER, sizeof(it1->train_coords), &it1->train_coords, GL_STATIC_DRAW);
					glDrawArrays(GL_POINTS, 0, 1);
				}
			}
		}
	}
}

void TrainLoader::initGuiStyle()
{
	this->style = &ImGui::GetStyle();
}

int TrainLoader::train_dispatch(Message& message)
{
	int end = 0;		//流程结束信号
	static int idx = 82001;
	static int lane = 0;
	static int max_wagon = 62;
	static int wagon_num = 24;
	if (ImGui::CollapsingHeader(u8"列车入港申请流程"))
	{
		ImGui::Indent();
		ImGui::Text(u8"编号选择：");
		ImGui::SameLine();
		ImGui::SliderInt(u8"输入列车编号", &idx, 82001, 84998);
		ImGui::Separator();
		ImGui::Text(u8"列车每节最大载货量选择：");
		ImGui::SameLine();
		ImGui::SliderInt(u8"输入最大每节载货量", &max_wagon, 62, 67);
		ImGui::Text(u8"列车车道选择：");
		ImGui::RadioButton(u8"15道", &lane, 0);
		ImGui::SameLine();
		ImGui::RadioButton(u8"16道", &lane, 1);
		ImGui::SameLine();
		ImGui::RadioButton(u8"17道", &lane, 2);
		ImGui::SameLine();
		ImGui::RadioButton(u8"18道", &lane, 3);
		ImGui::Text(u8"列车车皮数选择：");
		ImGui::SameLine();
		ImGui::SliderInt(u8"输入车皮数", &wagon_num, 1, 28);
		this->pre_button();
		if (ImGui::Button(u8"申请入港"))
		{
			bool enable = true;		//进港允许变量
			for (std::vector<TrainAttrib>::const_iterator it = this->trains.begin(); it != this->trains.end(); it++)
			{
				if (it->train_idx == idx)
				{
					enable = false;
					//std::cout << "进港申请失败::已存在相同编号的列车" << std::endl;
					message.push_message(u8"进港申请失败::已存在相同编号的列车");
				}
				if (it->train_lane == lane)
				{
					enable = false;
					//std::cout << "进港申请失败::车道被占用" << std::endl;
					message.push_message(u8"进港申请失败::车道被占用");
				}
			}
			if (enable && lane < 2 && wagon_num>24)
			{
				enable = false;
				//std::cout << "进港申请失败::车皮数超过车道承载上限" << std::endl;
				message.push_message(u8"进港申请失败::车皮数超过车道承载上限");
			}
			if (enable)
			{
				//允许进港
				this->train.train_idx = idx;
				this->train.train_lane = lane;
				this->train.max_per_wagon = (float)max_wagon;
				this->train.total_num = wagon_num;
				this->train.current_num = 1;
				this->train.first_pos = this->north_thresh + 0.1f;
				this->train.speed_train = SPEED_TRAIN_LEAVE;
				this->train.train_state = 0;
				this->train.train_type = 0;
				this->train.train_index = 0;
				//车皮初始化
				this->train.wagons.clear();
				this->wagon.amount = 0.0f;
				for (int i = 0; i < this->train.total_num; i++)
				{
					//车厢和VBO初始化
					this->wagon.wagon_idx = i + 1;
					this->train.wagons.push_back(this->wagon);
				}
				switch (lane)
				{
				case 0:
					this->train.train_coords[0] = this->building_coords_B[0] + this->train_x_bias;
					break;
				case 1:
					this->train.train_coords[0] = this->building_coords_B[0] - this->train_x_bias;
					break;
				case 2:
					this->train.train_coords[0] = this->building_coords_A[0] + this->train_x_bias;
					break;
				case 3:
					this->train.train_coords[0] = this->building_coords_A[0] - this->train_x_bias;
					break;
				default:
					break;
				}
				this->train.train_coords[1] = this->train.first_pos;
				this->train.train_coords[2] = LAYER_train;
				this->train.train_coords[3] = 0.0f;
				this->train.train_coords[4] = 1.0f;
				this->train.train_coords[5] = 0.0f;
				this->train.train_coords[6] = 0.0f;
				this->trains.push_back(this->train);
				//std::cout << "火车进港申请成功！" << std::endl;
				message.push_message(u8"火车进港申请成功！");
			}
		}
		this->post_button();
		ImGui::Unindent();
	}
	if (ImGui::CollapsingHeader(u8"列车调度台"))
	{
		ImGui::Indent();
		//切换装车楼装模式
		ImGui::Text(u8"15、16道装车模式选择：");
		ImGui::SameLine();
		this->set_button(0, false);
		if (ImGui::Button(u8"先进先出(FIFO)模式（15、16道）"))
		{
			this->building_B.building_mode = 0;
			this->building_coords_B[5] = 0.0f;
		}
		ImGui::SameLine();
		this->set_button(1, false);
		if (ImGui::Button(u8"后进先出(LIFO)模式（15、16道）"))
		{
			this->building_B.building_mode = 1;
			this->building_coords_B[5] = 1.0f;
		}
		ImGui::SameLine();
		this->set_button(2, false);
		if (ImGui::Button(u8"向西车道(16道)卸煤模式"))
		{
			this->building_B.building_mode = 2;
			this->building_coords_B[5] = 2.0f;
		}
		ImGui::SameLine();
		this->set_button(3, false);
		if (ImGui::Button(u8"向东车道(15道)卸煤模式"))
		{
			this->building_B.building_mode = 3;
			this->building_coords_B[5] = 3.0f;
		}
		this->post_button();
		ImGui::Text(u8"17、18道装车模式选择：");
		ImGui::SameLine();
		this->set_button(0, true);
		if (ImGui::Button(u8"先进先出(FIFO)模式（17、18道）"))
		{
			this->building_A.building_mode = 0;
			this->building_coords_A[5] = 0.0f;
		}
		ImGui::SameLine();
		this->set_button(1, true);
		if (ImGui::Button(u8"后进先出(LIFO)模式（17、18道）"))
		{
			this->building_A.building_mode = 1;
			this->building_coords_A[5] = 1.0f;
		}
		ImGui::SameLine();
		this->set_button(2, true);
		if (ImGui::Button(u8"向西车道(18道)卸煤模式"))
		{
			this->building_A.building_mode = 2;
			this->building_coords_A[5] = 2.0f;
		}
		ImGui::SameLine();
		this->set_button(3, true);
		if (ImGui::Button(u8"向东车道(17道)卸煤模式"))
		{
			this->building_A.building_mode = 3;
			this->building_coords_A[5] = 3.0f;
		}
		this->post_button();
		ImGui::Separator();
		//当前火车状态显示
		std::string name_lane = "";
		for (std::vector<TrainAttrib>::iterator it = this->trains.begin(); it != this->trains.end(); it++)
		{
			std::string name_idx = std::to_string(it->train_idx);
			switch (it->train_lane)
			{
			case 0:
				name_lane = u8"15道";
				break;
			case 1:
				name_lane = u8"16道";
				break;
			case 2:
				name_lane = u8"17道";
				break;
			case 3:
				name_lane = u8"18道";
				break;
			default:
				break;
			}
			switch (it->train_state)
			{
			case 0:
				ImGui::Text((u8"列车编号：" + name_idx + u8" （当前等待装货）").c_str());
				ImGui::Text((u8"分配车道：" + name_lane).c_str());
				this->pre_button();
				if (ImGui::Button((name_idx + u8"立即离港").c_str()))
				{
					it->speed_train = SPEED_TRAIN_LEAVE;
					it->train_state = 2;
					it->train_coords[3] = 2.0f;
					//装车楼变红，返回装车结束信号
					if (it->train_lane < 2)
					{
						//B楼BC15停止
						end = 1;
					}
					else
					{
						//A楼BC33停止
						end = 2;
					}
				}
				this->post_button();
				break;
			case 1:
				ImGui::Text((u8"列车编号：" + name_idx + u8" （当前正在装货）").c_str());
				ImGui::Text((u8"分配车道：" + name_lane).c_str());
				for (std::vector<Type_train>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->train_type && it2->type_index == it->train_index)
					{
						ImGui::Text((u8"火车载货种类：" + it2->type_name).c_str());
						break;
					}
				}
				this->pre_button();
				if (ImGui::Button((name_idx + u8"立即离港").c_str()))
				{
					it->speed_train = SPEED_TRAIN_LEAVE;
					it->train_state = 2;
					it->train_coords[3] = 2.0f;
					//装车楼变红，返回装车结束信号
					if (it->train_lane < 2)
					{
						//B楼BC15停止
						end = 1;
					}
					else
					{
						//A楼BC33停止
						end = 2;
					}
				}
				this->post_button();
				break;
			case 2:
				ImGui::Text((u8"列车编号：" + name_idx + u8" （当前正在离港）").c_str());
				ImGui::Text((u8"原车道：" + name_lane).c_str());
				for (std::vector<Type_train>::const_iterator it2 = this->names.begin(); it2 != this->names.end(); it2++)
				{
					if (it2->type_type == it->train_type && it2->type_index == it->train_index)
					{
						ImGui::Text((u8"火车载货种类：" + it2->type_name).c_str());
						break;
					}
				}
				break;
			default:
				break;
			}
			ImGui::Separator();
		}
		ImGui::Unindent();
	}
	return end;
}

int TrainLoader::updateTrains(float gapTime, float simurate)
{
	int end = 0;
	for (std::vector<TrainAttrib>::iterator it1 = this->trains.begin(); it1 != this->trains.end(); )
	{
		bool temp = true;
		if (it1->train_state == 0)
		{
			//等待装货，不更新
			it1++;
			continue;
		}
		else if (it1->train_state == 1)
		{
			//正在装货
			if (it1->train_lane < 2)
			{
				//B楼
				if (it1->first_pos - this->length_wagon / 2 > this->building_coords_B[1])
				{
					//未到达装车楼下方，减速前进
					it1->speed_train = SPEED_TRAIN_LOAD + (it1->first_pos - this->length_wagon / 2 - this->building_coords_B[1]) * (SPEED_TRAIN_LEAVE - SPEED_TRAIN_LOAD) / 2.0f;
					it1->first_pos -= it1->speed_train * gapTime *simurate;
				}
				else
				{
					//火车到达装车楼下方，装车
					it1->speed_train = SPEED_TRAIN_LOAD;
					it1->current_num = (int)((this->building_coords_B[1] - it1->first_pos) / length_wagon) + 1;
					for (std::vector<One_wagon>::iterator it2 = it1->wagons.begin(); it2 != it1->wagons.end(); it2++)
					{
						if (it2->wagon_idx == it1->current_num)
						{
							//当前正在装货编号
							if (it2->amount < it1->max_per_wagon)
							{
								it2->amount += LOAD_SPEED * gapTime * simurate;
								it1->train_coords[4] = (1.0f - it2->amount / it1->max_per_wagon);
							}
							break;
						}
					}
					it1->first_pos -= it1->speed_train * gapTime * simurate;
					if (it1->first_pos + this->length_wagon * ((float)it1->total_num + 0.5f) < this->building_coords_B[1])
					{
						//火车驶过装车楼，装车结束
						it1->speed_train = SPEED_TRAIN_LEAVE;
						it1->train_state = 2;
						it1->train_coords[3] = 2.0f;
						//装车楼变红，返回B楼BC15装车结束信号
						end = 1;
					}
				}
			}
			else
			{
				//A楼
				if (it1->first_pos - this->length_wagon / 2 > this->building_coords_A[1])
				{
					//未到达装车楼下方，减速前进
					it1->speed_train = SPEED_TRAIN_LOAD + (it1->first_pos - this->length_wagon / 2 - this->building_coords_A[1]) * (SPEED_TRAIN_LEAVE - SPEED_TRAIN_LOAD) / 2.0f;
					it1->first_pos -= it1->speed_train * gapTime * simurate;
				}
				else
				{
					//火车到达装车楼下方，装车
					it1->speed_train = SPEED_TRAIN_LOAD;
					it1->current_num = (int)((this->building_coords_A[1] - it1->first_pos) / length_wagon) + 1;
					for (std::vector<One_wagon>::iterator it2 = it1->wagons.begin(); it2 != it1->wagons.end(); it2++)
					{
						if (it2->wagon_idx == it1->current_num)
						{
							//当前正在装货编号
							if (it2->amount < it1->max_per_wagon)
							{
								it2->amount += LOAD_SPEED * gapTime * simurate;
								it1->train_coords[4] = (1.0f - it2->amount / it1->max_per_wagon);
							}
							break;
						}
					}
					it1->first_pos -= it1->speed_train * gapTime * simurate;
					if (it1->first_pos + this->length_wagon * ((float)it1->total_num + 0.5f) < this->building_coords_A[1])
					{
						//火车驶过装车楼，装车结束
						it1->speed_train = SPEED_TRAIN_LEAVE;
						it1->train_state = 2;
						it1->train_coords[3] = 2.0f;
						//装车楼变红，返回A楼BC33装车结束信号
						end = 2;
					}
				}
			}
		}
		else if (it1->train_state == 2)
		{
			//正在离港
			it1->speed_train += 0.00005f;
			it1->first_pos -= it1->speed_train * gapTime * simurate;
			if (it1->first_pos + this->length_wagon * ((float)it1->total_num + 0.5f) < this->south_thresh)
			{
				//火车离开视线，消失
				temp = false;
				it1 = this->trains.erase(it1);
			}
		}
		if (temp)
		{
			it1++;
		}
	}
	return end;
}

void TrainLoader::add_type(std::string str_name, int type_type)
{
	int lst_idx = 1;		//原对应大类中小类序号最大者
	for (std::vector<Type_train>::const_iterator it = this->names.begin(); it != this->names.end(); it++)
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

bool TrainLoader::run(std::vector<std::string>& equipments, int type, int index)
{
	float type_coord = 0.0f;
	//type类别判断
	if (type == 0)
	{
		type_coord = 0.0f;
	}
	else if (type == 1)
	{
		if (index < 7)
		{
			type_coord = (float)index;
		}
		else
		{
			type_coord = 7.0f;
		}
	}
	else
	{
		type_coord = 7.0f;
	}
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		if (*it1 == "BC33")
		{
			bool passed = false;
			for (std::vector<TrainAttrib>::iterator it2 = this->trains.begin(); it2 != this->trains.end(); it2++)
			{
				if(this->building_A.building_mode == 0)
				{
					//先进先出：遍历火车，先启动早进港且状态为正等待的车
					if (it2->train_lane > 1 && it2->train_state == 0)
					{
						passed = true;
						it2->train_state = 1;
						it2->train_coords[3] = 1.0f;
						it2->train_type = type;
						it2->train_index = index;
						it2->train_coords[6] = type_coord;
						break;
					}
				}
				else if (this->building_A.building_mode == 1)
				{
					//后进先出：遍历火车，先启动晚进港且状态为正等待的车
					if (it2->train_lane > 1 && it2->train_state == 0)
					{
						passed = true;
						it2->train_state = 1;
						it2->train_coords[3] = 1.0f;
						it2->train_type = type;
						it2->train_index = index;
						it2->train_coords[6] = type_coord;
					}
				}
				else if (this->building_A.building_mode == 2)
				{
					//左车道装车
					if (it2->train_state == 0 && it2->train_lane == 3)
					{
						passed = true;
						it2->train_state = 1;
						it2->train_coords[3] = 1.0f;
						it2->train_type = type;
						it2->train_index = index;
						it2->train_coords[6] = type_coord;
						break;
					}
				}
				else if (this->building_A.building_mode == 3)
				{
					//右车道装车
					if (it2->train_state == 0 && it2->train_lane == 2)
					{
						passed = true;
						it2->train_state = 1;
						it2->train_coords[3] = 1.0f;
						it2->train_type = type;
						it2->train_index = index;
						it2->train_coords[6] = type_coord;
						break;
					}
				}
			}
			if (!passed)
			{
				return false;
			}
			//启动A楼
			this->building_A.building_state = 1;
			this->building_coords_A[3] = 1.0f;
			break;
		}
		else if (*it1 == "BC15")
		{
			bool passed = false;
			for (std::vector<TrainAttrib>::iterator it2 = this->trains.begin(); it2 != this->trains.end(); it2++)
			{
				if (this->building_B.building_mode == 0)
				{
					//先进先出：遍历火车，先启动早进港且状态为正等待的车
					if (it2->train_lane < 2 && it2->train_state == 0)
					{
						passed = true;
						it2->train_state = 1;
						it2->train_coords[3] = 1.0f;
						it2->train_type = type;
						it2->train_index = index;
						it2->train_coords[6] = type_coord;
						break;
					}
				}
				else if (this->building_B.building_mode == 1)
				{
					//后进先出：遍历火车，先启动晚进港且状态为正等待的车
					if (it2->train_lane < 2 && it2->train_state == 0)
					{
						passed = true;
						it2->train_state = 1;
						it2->train_coords[3] = 1.0f;
						it2->train_type = type;
						it2->train_index = index;
						it2->train_coords[6] = type_coord;
					}
				}
				else if (this->building_B.building_mode == 2)
				{
					//左车道装车
					if (it2->train_state == 0 && it2->train_lane == 1)
					{
						passed = true;
						it2->train_state = 1;
						it2->train_coords[3] = 1.0f;
						it2->train_type = type;
						it2->train_index = index;
						it2->train_coords[6] = type_coord;
						break;
					}
				}
				else if (this->building_B.building_mode == 3)
				{
					//右车道装车
					if (it2->train_state == 0 && it2->train_lane == 0)
					{
						passed = true;
						it2->train_state = 1;
						it2->train_coords[3] = 1.0f;
						it2->train_type = type;
						it2->train_index = index;
						it2->train_coords[6] = type_coord;
						break;
					}
				}
			}
			if (!passed)
			{
				return false;
			}
			//启动B楼
			this->building_B.building_state = 1;
			this->building_coords_B[3] = 1.0f;
			break;
		}
	}
	return true;
}

void TrainLoader::shutDown(std::vector<std::string>& equipments)
{
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		if (*it1 == "BC33")
		{
			//关闭A楼
			this->building_A.building_state = 0;
			this->building_coords_A[3] = 0.0f;
			for (std::vector<TrainAttrib>::iterator it2 = this->trains.begin(); it2 != this->trains.end(); it2++)
			{
				//火车变等待（红色）
				if (it2->train_lane > 1 && it2->train_state == 1)
				{
					it2->train_state = 0;
					it2->train_coords[3] = 0.0f;
					break;
				}
			}
			break;
		}
		else if (*it1 == "BC15")
		{
			//关闭B楼
			this->building_B.building_state = 0;
			this->building_coords_B[3] = 0.0f;
			for (std::vector<TrainAttrib>::iterator it2 = this->trains.begin(); it2 != this->trains.end(); it2++)
			{
				//火车变等待（红色）
				if (it2->train_lane < 2 && it2->train_state == 1)
				{
					it2->train_state = 0;
					it2->train_coords[3] = 0.0f;
					break;
				}
			}
			break;
		}
	}
}

void TrainLoader::set_focus(std::vector<std::string>& equipments)
{
	this->building_coords_A[4] = 1.0f;
	this->building_coords_B[4] = 1.0f;
	for (std::vector<TrainAttrib>::iterator it1 = this->trains.begin(); it1 != this->trains.end(); it1++)
	{
		it1->train_coords[5] = 1.0f;
	}
	for (std::vector<std::string>::const_iterator it2 = equipments.begin(); it2 != equipments.end(); it2++)
	{
		if (*it2 == "BC33")
		{
			//A楼
			this->building_coords_A[4] = 0.0f;
			for (std::vector<TrainAttrib>::iterator it3 = this->trains.begin(); it3 != this->trains.end(); it3++)
			{
				if (it3->train_lane > 1)
				{
					it3->train_coords[5] = 0.0f;
				}
			}
			break;
		}
		else if (*it2 == "BC15")
		{
			//B楼
			this->building_coords_B[4] = 0.0f;
			for (std::vector<TrainAttrib>::iterator it3 = this->trains.begin(); it3 != this->trains.end(); it3++)
			{
				if (it3->train_lane < 2)
				{
					it3->train_coords[5] = 0.0f;
				}
			}
			break;
		}
	}
}

void TrainLoader::lose_focus()
{
	this->building_coords_A[4] = 0.0f;
	this->building_coords_B[4] = 0.0f;
	for (std::vector<TrainAttrib>::iterator it = this->trains.begin(); it != this->trains.end(); it++)
	{
		it->train_coords[5] = 0.0f;
	}
}

void TrainLoader::pre_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}

void TrainLoader::post_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}

void TrainLoader::set_button(int index, bool building)
{
	if (building)
	{
		//A楼
		if (this->building_A.building_mode == index)
		{
			this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
			this->style->Colors[ImGuiCol_Button] = ImColor(0, 255, 0, 255);
		}
		else
		{
			this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
			this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
		}
	}
	else
	{
		//B楼
		if (this->building_B.building_mode == index)
		{
			this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
			this->style->Colors[ImGuiCol_Button] = ImColor(0, 255, 0, 255);
		}
		else
		{
			this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
			this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
		}
	}
}
