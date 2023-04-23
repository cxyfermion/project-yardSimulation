#include "Message.h"

Message::Message()
{
	this->window_message = true;
}

void Message::initGuiStyle()
{
	this->style = &ImGui::GetStyle();
	this->SoundEngine = irrklang::createIrrKlangDevice();
}

void Message::reset()
{
	this->messages.clear();
	this->window_message = true;
}

void Message::push_message(std::string error_info)
{
	this->messages.push_back(error_info);
	this->window_message = true;
}

void Message::push_sound(std::string sound_course, bool looped)
{
	SoundEngine->play2D(sound_course.c_str(), looped);
}

void Message::show()
{
	if (this->messages.size() && this->window_message)
	{
		ImGui::Begin(u8"��Ϣ");
		this->pre_button();
		if (ImGui::Button(u8"�رմ���"))
		{
			this->window_message = false;
		}
		if (this->messages.size())
		{
			ImGui::SameLine();
			if (ImGui::Button(u8"���������Ϣ"))
			{
				this->messages.clear();
			}
		}
		this->post_button();
		bool temp = true;
		int idx = 0;
		for (std::vector<std::string>::iterator it = this->messages.begin(); it != this->messages.end();)
		{
			temp = true;
			idx++;
			ImGui::SetNextItemWidth(400.0f);
			ImGui::Text((*it).c_str());
			ImGui::SameLine();
			this->pre_button();
			if (ImGui::Button((u8"�����" + std::to_string(idx) + u8"����Ϣ").c_str()))
			{
				it = this->messages.erase(it);
				temp = false;
			}
			this->post_button();
			if (temp)
			{
				it++;
			}
		}
		ImGui::End();
	}
}

void Message::pre_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}

void Message::post_button()
{
	this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}
