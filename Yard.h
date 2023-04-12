#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "res/GL_heads/Camera.hpp"
#include "res/GL_heads/Shader.h"
#include "SimuCore.h"
#include "Message.h"

/*
child_type�Ӷѳ��ѻ����ࣺ
1��ʯ		2ˮ��		3��ҵ��		4ֽ��		5��ɰ		6ͭ��		7ʯ�ͽ�			8��ú		9����ú		10��ú		12�Ż�		13����ú	14����
1���		2�ɻ�		3��ͬ		4��		5��ú		6����˹ú	7����˹����	8��ú
1���и�	2�񻪾�		3��̩		4����		5�ص׻�		6���		7����			8��һ		9��		10����

type
0 vec3(1.0f)
1 vec3(0.3f)
2 vec3(0.4f)
3 vec3(0.85f)
4 vec3(0.6f)
5 vec3(0.7f, 0.7f, 0.0f)
6 vec3(0.9f, 0.9f, 0.4f)
7 vec3(0.0f)

�Ӷѳ����Ͻǵĵ㷴Ӧ�ұߵĶ��ֻ����佻��״̬��0�޽����������½�һ�£���1��װ������ɫ����2��ȡ������ɫ��
�Ӷѳ����Ͻǵĵ㷴Ӧ��ߵĶ��ֻ����佻��״̬��0�޽����������½�һ�£���1��װ������ɫ����2��ȡ������ɫ��
�����������������֣��Ӷѳ������㷴ӳ��ǰ�ѳ��Ļ������ࣺ�޻���Ϊ��ɫ�������ڵڶ�������ʾΪ0.0f�ĺڣ������ڵ���������ʾΪ0.0f�ĺڣ�
��ʯ��ʾΪ0.5f�Ļң�ˮ����ʾΪ0.7f�Ļң���ɰ��ʾΪ���ƣ���ҵ����ʾΪ0.85f�Ļ�ɫ��ͭ����ʾΪ���ƣ�����ú��ʾΪ0.2f��ɫ

��ȡ�϶ѵ�ǰ�������ڵ�λ������ֵ����ʾ���ϲ��ϣ����Ӷѳ��°벿�֣�����˸

��������ʧ�ܿ���ԭ��
1.�ó���������һ�ֻ���
2.�ó��صĻ�������������󳬹��ѷ���������

����ȥ������ú���֣�һ�Ǵ��1�ţ����ǻ���ú��

*/

struct Type_yard
{
	int type_index;			//����С���ţ����嵽��������ƣ���1��ʼ����
	int type_type;			//��������ţ�1Ϊ����������2Ϊ��Դ������3Ϊ��ҵ����
	std::string type_name;	//�ѻ�����
};

struct Yard_child
{
	//�Ӷѳ��ṹ��
	int child_name;				//�Ӷѳ���ţ���1��ʼ
	float child_max_amount;	//�Ӷѳ����ѻ���
	float child_cur_amount;	//�Ӷѳ���ǰ�ѻ���
	float child_flux_left;		//�Ӷѳ��䶯����
	float child_flux_right;		//�Ӷѳ��䶯����
	bool child_focus;			//�Ӷѳ�ѡ��ע�����
	int child_type;				//�Ӷѳ��ѻ����ࣨ0��ʾδ���壩
	int child_index;			//�Ӷѳ��ѻ�С�ࣨ0��ʾδ���壩
	int child_state_left;		//�Ӷѳ���������ģʽ��0�޽�������ʾ�ѳ�������ʣ���1�����ϣ���ɫ����2��ȡ�ϣ���ɫ��
	int child_state_right;		//�Ӷѳ���������ģʽ��0�޽�������ʾ�ѳ�������ʣ���1�����ϣ���ɫ����2��ȡ�ϣ���ɫ��
};

struct Yard_father
{
	//���ѳ��ṹ��
	std::string yard_name;						//�ѳ�����
	int yard_index;								//�ѳ���ţ���1��ʼ��
	std::vector<Yard_child> children;			//�Ӷѳ�
	std::vector<std::string> wheel_available;	//�ѳ������Ӷ��ֻ�
	float center_x;								//�ѳ�����x����
	float center_y;								//�ѳ�����y����
	bool yard_focus;							//�ѳ�ע��״̬����
	float yard_coords[7];						//�ѳ����꣺x,y,z,state_zuo,state_you,type,focus
};

class FindYardG
{
public:
	bool operator ()(Yard_father father)
	{
		return father.yard_name == "G";
	}
};

class FindYardH
{
public:
	bool operator ()(Yard_father father)
	{
		return father.yard_name == "H";
	}
};

class FindYardK
{
public:
	bool operator ()(Yard_father father)
	{
		return father.yard_name == "K";
	}
};

class FindYardWEST2
{
public:
	bool operator ()(Yard_father father)
	{
		return father.yard_name == "WEST2";
	}
};

//�ѳ�
class Yard
{
public:
	Yard();
	void reset(SimuCore& core, bool rand_init);		//���ã�rand_initΪtrue��ʾ���������ʼ��
	void yard_random_initiator(SimuCore& core);	//�ѳ������ʼ��
	std::string terminate_wheel;					//�����ȡ�������رյĶ��ֻ�����
	int yard_choosed;								//ѡ�еĶѳ�
	int child_choosed;								//ѡ�е��Ӷѳ�
	int type_choosed;								//ѡ���Ӷѳ��Ļ������
	int index_choosed;								//ѡ���Ӷѳ��Ļ���С��
	float amount_choosed;							//ѡ���Ӷѳ��Ļ������
	bool data_send;									//�����Ƿ���
	bool yard_choosing;								//�Ƿ����ѳ�ѡ�񴰿�
	void drawYard(Camera& camera, Shader& yardShader);
	void initGuiStyle();								//��ʽ��ʼ��
	void yard_dispatch(Message& message, bool unreal);				//ImGui�ѳ�״̬����̨����������µĻ���Ʒ�֣���ȡ�ѳ���ǰ״̬��Ϣ������Ϊtrue��ʾ��ǿ��ָ��Ȩ
	void add_type(std::string str_name, int type_type);		//��ӻ�������
	bool start_stack(Message& message, std::vector<std::string>& equipments, int yard, int child, int type, int index, float amount, float flux);	//��ʼ���ϣ�����Ϊ�ѳ��ţ��ӳ��ţ������࣬�������������������falseΪ��������ʧ��
	bool start_reclaim(Message& message, std::vector<std::string>& equipments, int yard, int child, int type, int index, float flux);				//��ʼȡ�ϣ�����falseΪȡ������ʧ��
	void yard_end(std::vector<std::string>& equipments);		//��ֹ�ѳ��ж���ɨ�������豸�еĶ��ֻ�����������Ӧ�ѳ�Ϊ0״̬��
	bool type_check(int type, int index);			//����Ƿ������Ӧ������Ķ��壬����true��ʾ���ڶ���
	bool updateYards(float simurate);				//����0�����£�����1����ȡ�رն��ֻ����Ų��ս��Ӧ����
	void yard_choose(Message& message);								//ѡ��ѳ�����
	void set_focus(std::vector<std::string>& equipments);
	void lose_focus();
	void send_reset();								//�����źŸ�λ

private:
	int flow_yard_choosed_1;				//������ѡ�еĶѳ�1��
	int flow_yard_choosed_2;				//������ѡ�еĶѳ�2��
	int flow_num_choose;					//������ѡ�еĶѳ�������
	ImGuiStyle* style;						//ImGui��ʽ
	Type_yard type_unit;					//���ﵥԪ����������������ż�������
	std::vector<Type_yard> names;			//���������������ƴ洢����
	Yard_child child;						//�Ӷѳ�
	Yard_father father;						//�ѳ�
	std::vector<Yard_father> yards;			//�ѳ�����
	unsigned int yardVBO, yardVAO;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	void showYardInfo(int yard_area, bool unreal);		//�ѳ���Ϣ��ʾ
	void chooseYardInfo(std::vector<Yard_father>::iterator& it_begin, std::vector<Yard_father>::iterator& it_end, int schoose);	//�ѳ�ѡ����Ϣ��ʾ
	void pre_button(bool choosed);						//�ѳ�ѡ��ť��ʽ����
	void post_button();

};
