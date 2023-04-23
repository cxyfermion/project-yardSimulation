#pragma once
#include <vector>
#include <string>
#include <imgui.h>
#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib")

class Message
{
public:
	Message();
	void initGuiStyle();					//样式初始化
	void reset();						//重置
	void push_message(std::string error_info);
	void push_sound(std::string sound_course, bool looped);
	void show();

private:
	ImGuiStyle* style;					//ImGui样式存储
	irrklang::ISoundEngine* SoundEngine;
	bool window_message;				//消息窗口的开关
	std::vector<std::string> messages;	//信息存储容器
	void pre_button();
	void post_button();

};
