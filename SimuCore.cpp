#include "SimuCore.h"

SimuCore::SimuCore()
{
	srand((unsigned int)std::time(0));
	this->time = glfwGetTime();
	this->simuTime= glfwGetTime();
	this->simu_deltaTime = 0.0;
	this->simu_lastFrame = 0.0;
	this->runtime_seconds = 0;
	this->runtime_minutes = 0;
	this->runtime_hours = 0;
	this->runtime_days = 0;
	this->run_rate = 1.0f;
	this->simu_pause = true;
	this->stepping = false;
	this->reset_zero = false;
	this->reset_rand = false;
	this->terminate_time = 86400.0;	//24Hour
	this->terminate_countdown = 86400.0;
	this->freshGapSwitch = 2;		//50ms
	this->freshGapTime = 0.05;
	this->curGapTime = 0.0;
	this->freshRequire = false;
}

void SimuCore::initGuiStyle()
{
	this->style = &ImGui::GetStyle();
}

void SimuCore::simulator_gui(Message& message)
{
	static float simu_rate = 1.0f;
	static double terminate = 86400.0;
	if (ImGui::CollapsingHeader(u8"仿真控制台"))
	{
		ImGui::Indent();
		this->pre_button(this->simu_pause);
		std::string pause_state = this->simu_pause ? u8"运行" : u8"停止";
		if (ImGui::Button(pause_state.c_str()))
		{
			if (this->simu_pause)
			{
				this->simu_lastFrame = this->time;
				message.push_sound("res/audio/start.ogg", false);
			}
			else
			{
				message.push_sound("res/audio/pause.ogg", false);
			}
			this->simu_pause = !this->simu_pause;
		}
		this->post_button();
		ImGui::SameLine();
		this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
		this->style->Colors[ImGuiCol_Button] = ImColor(200, 0, 0, 255);
		if (ImGui::Button(u8"零状态重置"))
		{
			this->simu_pause = true;
			this->simuTime = 0.0;
			this->run_rate = 1.0f;
			this->reset_zero = true;
			this->curGapTime = 0.0;
			this->freshRequire = false;
		}
		this->pre_button(false);
		ImGui::SameLine();
		this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
		this->style->Colors[ImGuiCol_Button] = ImColor(200, 0, 0, 255);
		if (ImGui::Button(u8"随机重置"))
		{
			this->simu_pause = true;
			this->simuTime = 0.0;
			this->run_rate = 1.0f;
			this->reset_rand = true;
			this->curGapTime = 0.0;
			this->freshRequire = false;
		}
		this->pre_button(false);
		ImGui::SameLine();
		//步进仅在仿真暂停的状态下有效
		if (ImGui::Button(u8"步进"))
		{
			if (this->simu_pause)
			{
				this->stepping = true;
				//时间前进，步进只执行一次时间前进
				//在仿真刚开始暂停状态下使用步进按钮，仿真时间不会增加
				this->simuTime += this->simu_deltaTime * this->run_rate;
				this->terminate_countdown -= this->simu_deltaTime * this->run_rate;
				if (this->terminate_countdown < 0.0)
				{
					this->simu_pause = true;
				}
			}
		}
		this->post_button();
		ImGui::Text(u8"选择仿真速率：");
		ImGui::SameLine();
		//ImGui::InputFloat(u8"倍", &simu_rate);
		ImGui::SliderFloat(u8"倍", &simu_rate, 0.1f, 200.0f);
		/*if (simu_rate <= 0 || simu_rate > 10000)
		{
			std::cout << "速率范围不合法！速率范围为0-10000" << std::endl;
		}
		else */if (this->run_rate != simu_rate)
		{
			this->run_rate = simu_rate;
		}
		ImGui::Text(u8"设定画面刷新间隔：");
		ImGui::SameLine();
		ImGui::RadioButton("20ms, 50FPS", &this->freshGapSwitch, 1);
		ImGui::SameLine();
		ImGui::RadioButton("50ms, 20FPS", &this->freshGapSwitch, 2);
		ImGui::SameLine();
		ImGui::RadioButton("100ms, 10FPS", &this->freshGapSwitch, 3);
		ImGui::SameLine();
		ImGui::RadioButton("200ms, 5FPS", &this->freshGapSwitch, 4);
		ImGui::SameLine();
		ImGui::RadioButton("500ms, 2FPS", &this->freshGapSwitch, 5);
		ImGui::SameLine();
		ImGui::RadioButton("1000ms, 1FPS", &this->freshGapSwitch, 6);
		switch (this->freshGapSwitch)
		{
		case 1:
			this->freshGapTime = 0.02;
			break;
		case 2:
			this->freshGapTime = 0.05;
			break;
		case 3:
			this->freshGapTime = 0.1;
			break;
		case 4:
			this->freshGapTime = 0.2;
			break;
		case 5:
			this->freshGapTime = 0.5;
			break;
		case 6:
			this->freshGapTime = 1.0;
			break;
		default:
			break;
		}
		ImGui::Text(u8"运行时间：");
		ImGui::SameLine();
		ImGui::InputDouble(u8"秒", &terminate);
		ImGui::SameLine();
		this->pre_button(false);
		if (ImGui::Button(u8"设定运行时间"))
		{
			this->terminate_time = terminate;
			this->terminate_countdown = terminate;
		}
		this->post_button();
		ImGui::Unindent();
	}
}

void SimuCore::base_info()
{
	float FPS = ImGui::GetIO().Framerate;
	ImGui::Text("FPS = %f", FPS);
	ImGui::SameLine();
	ImGui::Text((u8"	程序运行时间：" + std::to_string(this->time) + u8"秒").c_str());
	ImGui::SameLine();
	ImGui::Text(u8"	仿真运行时间：");
	ImGui::SameLine();
	if (this->runtime_days > 0)
	{
		ImGui::Text((std::to_string(this->runtime_days) + u8" 日").c_str());
		ImGui::SameLine();
	}
	if (this->runtime_days > 0 || this->runtime_hours > 0)
	{
		ImGui::Text((std::to_string(this->runtime_hours) + u8" 时").c_str());
		ImGui::SameLine();
	}
	if (this->runtime_days > 0 || this->runtime_hours > 0 || this->runtime_minutes > 0)
	{
		ImGui::Text((std::to_string(this->runtime_minutes) + u8" 分").c_str());
		ImGui::SameLine();
	}
	ImGui::Text((std::to_string(this->runtime_seconds) + u8" 秒").c_str());
	ImGui::Text(u8"移动：W A S D R F；视角：MOUSE2（按住）；视野：UP DOWN；退出：ESC");
	ImGui::Text(u8"快速移动：LEFT SHIFT（按住）；缓慢移动：LEFT CONTROL（按住）；控制台（未实装）：~");
}

void SimuCore::updateTime()
{
	this->time = glfwGetTime();
	if (!this->simu_pause)
	{
		this->simu_deltaTime = this->time - this->simu_lastFrame;
		this->simu_lastFrame = this->time;
		this->simuTime += this->simu_deltaTime * this->run_rate;
		this->curGapTime += this->simu_deltaTime;
		if (this->curGapTime > this->freshGapTime)
		{
			this->freshRequire = true;
			this->curGapTime -= this->freshGapTime;
		}
		this->terminate_countdown -= this->simu_deltaTime * this->run_rate;
		if (this->terminate_countdown < 0.0)
		{
			this->simu_pause = true;
		}
	}
	this->fresh_time();
}

float SimuCore::genRandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> randomFloat(min, max);
	return randomFloat(this->generator);
}

int SimuCore::genRandomInt(int min, int max)
{
	//包含下界，不包含上界
	std::uniform_real_distribution<float> randomInt((float)min, (float)(max + 1));
	return (int)(randomInt(this->generator));
}

bool SimuCore::dice(float probability)
{
	std::uniform_real_distribution<float> randomFloat(0.0f, 1.0f);
	return randomFloat(this->generator) < probability;
}

void SimuCore::fresh_time()
{
	this->runtime_days = (int)this->simuTime / 86400;
	this->runtime_hours = ((int)this->simuTime - this->runtime_days * 86400) / 3600;
	this->runtime_minutes= ((int)this->simuTime - this->runtime_days * 86400 - this->runtime_hours * 3600) / 60;
	this->runtime_seconds = (int)this->simuTime - this->runtime_days * 86400 - this->runtime_hours * 3600 - this->runtime_minutes * 60;
}

void SimuCore::pre_button(bool choosed)
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

void SimuCore::post_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}
