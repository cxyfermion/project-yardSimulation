#include "SlewingWheel.h"

#define CT_WIDTH 4100
#define CT_HEIGHT 2370
#define SCR_WIDTH 1440
#define SCR_HEIGHT 810
#define LAYER_wheel 0.0055f

SlewingWheel::SlewingWheel()
{
	std::string info_wheel[8];
	info_wheel[0] = "SR6,2250,1100";
	info_wheel[1] = "SR7,1930,1100";
	info_wheel[2] = "SR8,1385,1100";
	info_wheel[3] = "SR9,1000,1100";
	info_wheel[4] = "SR10,680,1100";
	info_wheel[5] = "SR11,3150,1030";
	info_wheel[6] = "SR12,680,1540";
	info_wheel[7] = "S12,3515,885";

	this->wheel_attr.wheel_state = 0;
	this->wheel_attr.wheel_mode = 0;
	//逗号索引
	int last_pos = 0;
	//斗轮机数据读取
	for (int i = 0; i < 8; i++)
	{
		last_pos = 0;
		this->wheel_attr.wheel_idx = i;
		this->wheel_attr.wheel_name = info_wheel[i].substr(0, info_wheel[i].find(',', 0));
		last_pos = (int)(info_wheel[i].find(',', 0)) + 1;
		for (int j = 0; j < 7; j++)
		{
			if (j == 2)
			{
				this->wheel_coords[7 * i + j] = LAYER_wheel;
				continue;
			}
			if (j == 6)
			{
				this->wheel_coords[7 * i + j] = 0.0f;
				continue;
			}
			if (j >= 3)
			{
				this->wheel_coords[7 * i + 3] = 1.0f;
				this->wheel_coords[7 * i + 4] = 0.0f;
				this->wheel_coords[7 * i + 5] = 0.0f;
				continue;
			}
			//横坐标除以400并减5，横坐标除以400并减3
			//总体范围横坐标-5到5，纵坐标-3到3
			if (j == 0)
			{
				this->wheel_coords[7 * i + j] = (float)(stoi(info_wheel[i].substr(last_pos, info_wheel[i].find(',', last_pos) - last_pos))) / (CT_WIDTH / 10.0f) - 5.0f;
			}
			else
			{
				this->wheel_coords[7 * i + j] = -(float)(stoi(info_wheel[i].substr(last_pos, info_wheel[i].find(',', last_pos) - last_pos))) / (CT_WIDTH / 10.0f) + (CT_HEIGHT * 5.0f / CT_WIDTH);
			}
			last_pos = (int)(info_wheel[i].find(',', last_pos)) + 1;
		}
		this->wheels.push_back(this->wheel_attr);
	}
	glGenVertexArrays(1, &wheelVAO);
	glGenBuffers(1, &wheelVBO);
	glBindVertexArray(wheelVAO);
	glBindBuffer(GL_ARRAY_BUFFER, wheelVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wheel_coords), &wheel_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void SlewingWheel::reset()
{
	for (std::vector<WheelAttrib>::iterator it1 = this->wheels.begin(); it1 != this->wheels.end(); it1++)
	{
		it1->wheel_state = 0;
		this->wheel_coords[7 * it1->wheel_idx + 3] = 1.0f;
		this->wheel_coords[7 * it1->wheel_idx + 4] = 0.0f;
		this->wheel_coords[7 * it1->wheel_idx + 5] = 0.0f;
	}
}

void SlewingWheel::draw(Camera& camera, Shader& wheelShader)
{
	wheelShader.use();
	glBindBuffer(GL_ARRAY_BUFFER, wheelVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wheel_coords), &wheel_coords, GL_STATIC_DRAW);
	this->model = glm::mat4(1.0f);
	wheelShader.setMat4("model", this->model);
	this->projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
	wheelShader.setMat4("projection", this->projection);
	this->view = glm::mat4(1.0f);
	this->view = camera.GetViewMatrix();
	wheelShader.setMat4("view", this->view);
	glBindVertexArray(wheelVAO);
	glDrawArrays(GL_POINTS, 0, 8);
}

bool SlewingWheel::startCheck(std::vector<std::string> equipments)
{
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<WheelAttrib>::const_iterator it2 = this->wheels.begin(); it2 != this->wheels.end(); it2++)
		{
			if (*it1 == it2->wheel_name)
			{
				if (it2->wheel_state == 1)
				{
					return false;
				}
				break;
			}
		}
	}
	return true;
}

void SlewingWheel::run(std::vector<std::string> equipments, bool mode)
{
	//设置相应斗轮机为绿色
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<WheelAttrib>::iterator it2 = this->wheels.begin(); it2 != this->wheels.end(); it2++)
		{
			if (*it1 == it2->wheel_name)
			{
				//设置运行状态
				it2->wheel_state = 1;
				it2->wheel_mode = mode;
				if (mode)
				{
					//堆料绿色
					this->wheel_coords[7 * it2->wheel_idx + 3] = 0.0f;
					this->wheel_coords[7 * it2->wheel_idx + 4] = 1.0f;
					this->wheel_coords[7 * it2->wheel_idx + 5] = 0.0f;
				}
				else
				{
					//取料蓝色
					this->wheel_coords[7 * it2->wheel_idx + 3] = 0.0f;
					this->wheel_coords[7 * it2->wheel_idx + 4] = 0.0f;
					this->wheel_coords[7 * it2->wheel_idx + 5] = 1.0f;
				}
				break;
			}
		}
	}
}

void SlewingWheel::shutDown(std::vector<std::string> equipments)
{
	//设置相应斗轮机为红色
	for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
	{
		for (std::vector<WheelAttrib>::iterator it2 = this->wheels.begin(); it2 != this->wheels.end(); it2++)
		{
			if (*it1 == it2->wheel_name)
			{
				//设置运行状态
				it2->wheel_state = 0;
				//设置对应颜色
				this->wheel_coords[7 * it2->wheel_idx + 3] = 1.0f;
				this->wheel_coords[7 * it2->wheel_idx + 4] = 0.0f;
				this->wheel_coords[7 * it2->wheel_idx + 5] = 0.0f;
				break;
			}
		}
	}
}

void SlewingWheel::set_focus(std::vector<std::string>& equipments)
{
	for (std::vector<WheelAttrib>::iterator it2 = this->wheels.begin(); it2 != this->wheels.end(); it2++)
	{
		this->wheel_coords[7 * it2->wheel_idx + 6] = 1.0f;
		for (std::vector<std::string>::const_iterator it1 = equipments.begin(); it1 != equipments.end(); it1++)
		{
			if (*it1 == it2->wheel_name)
			{
				this->wheel_coords[7 * it2->wheel_idx + 6] = 0.0f;
				break;
			}
		}
	}
}

void SlewingWheel::lose_focus()
{
	for (std::vector<WheelAttrib>::iterator it = this->wheels.begin(); it != this->wheels.end(); it++)
	{
		this->wheel_coords[7 * it->wheel_idx + 6] = 0.0f;
	}
}
