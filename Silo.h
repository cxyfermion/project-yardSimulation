#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "res/GL_heads/Camera.hpp"
#include "res/GL_heads/Shader.h"
#include "SimuCore.h"

/*
����Ͳ��һ��Ͳ�ִ�ſ���װúһǧ�֡���14��Ͳ��
ȥ��������ú��2�֣����ͻ���ú
falseΪ���ú��trueΪ����ú
//14��Ͳ�����ڶ��ϵ��������Ϊ1��������ж�ϵ����Ϊ1��

*/

struct Silo_unit
{
	int silo_idx;			//Ͳ����ţ�1-14
	float max_storage;		//Ͳ����������
	float cur_storage;		//Ͳ�ֵ�ǰ����
	bool silo_type;			//�������falseΪ���úvec3(0.3f)��trueΪ����úvec3(0.1f)
	float silo_flux;			//Ͳ�ֱ䶯����
	bool silo_focus;			//Ͳ��ѡ��ע�����
	bool silo_state_up;		//Ͳ�ֶ�������ģʽ��0�޽�������ʾ������ʣ���1�����ϣ���ɫ��
	bool silo_state_down;	//Ͳ�ֵײ�����ģʽ��0�޽�������ʾ������ʣ���1��ж�ϣ���ɫ��
	float silo_coords[8];	//Ͳ�����꣺x,y,z,state_up,state_down,percent,type,focus
};

class Silo
{
public:
	Silo();
	void reset(SimuCore& core, bool rand_init);									//����
	void silo_random_initiator(SimuCore& core);								//Ͳ�������ʼ��
	bool data_send;																//�����Ƿ���
	bool select_window;															//�Ƿ���ʾͲ��ѡ�����
	float amount_apply;														//����ѷ���
	int selected_index;															//ѡ�е�Ͳ��
	void drawSilo(Camera& camera, Shader& siloShader);
	void initGuiStyle();
	void silo_dispatch(bool unreal);												//ImGuiͲ��״̬����̨����ȡͲ�ֵ�ǰ״̬��Ϣ������Ϊtrue��ʾ��ǿ��ָ��Ȩ
	void start_straight(float flux, bool type);									//ֱ�����ֵ����̿����ж�,����0��죬1����
	bool start_stack(int silo_index, bool type, float amount, float flux);		//��ʼ���ϣ�����ΪͲ�ֺţ������ࣨfalseΪ���ú��trueΪ����ú�����������������������falseΪ��������ʧ��
	bool updateSilos(float gapTime, float simurate);							//����0�����£�����1���ս��Ӧ����
	void updateStraight(float gapTime, float simurate);							//����ֱ��ϵͳ״̬
	void silo_end_out();														//��ֹͲ�����������
	bool out_check();															//����true��Ͳ�����ڳ�ú������false��Ͳ��û�г�ú
	void set_focus(std::vector<std::string>& equipments);
	void lose_focus();
	void show_select();															//��ʾͲ��ѡ����棬����0Ϊû��ѡ���ѡ��ʧ�ܣ�����1-14��ʾѡ�е�Ͳ�����
	void start_silo_out(float flux, bool type);									//��ʼͲ�ֳ�
	bool check_silo_out(bool type);												//Ͳ�ֳ����
	void end_silo(std::vector<std::string>& equipments);						//����Ͳ�ֳ�
	void send_reset();															//�����źŸ�λ

private:
	ImGuiStyle* style;
	Silo_unit silo_unit;
	std::vector<Silo_unit> silos;
	bool silo_occupied[14];
	unsigned int siloVBO, siloVAO;
	float storage_targets;				//Ŀ�����
	float storage_process;				//��ǰ�����仯
	int straight_state;					//ֱ��״̬��0Ϊ��ֱ����1Ϊֱ����죬2Ϊֱ������ú
	float straight_flux;				//ֱ������
	unsigned int out_total_0;			//�ܳ����
	unsigned int out_total_1;			//�ܳ�����ú
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	void pre_button(bool choosed);		//�ѳ�ѡ��ť��ʽ����
	void post_button();

};
