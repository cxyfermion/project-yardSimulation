#pragma once
#include <vector>
#include <string>
#include <imgui.h>

class Message
{
public:
	Message();
	void initGuiStyle();					//��ʽ��ʼ��
	void reset();						//����
	void push_message(std::string error_info);
	void show();

private:
	ImGuiStyle* style;					//ImGui��ʽ�洢
	bool window_message;				//��Ϣ���ڵĿ���
	std::vector<std::string> messages;	//��Ϣ�洢����
	void pre_button();
	void post_button();

};
