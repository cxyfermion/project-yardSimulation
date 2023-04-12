#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "res/GL_heads/Camera.hpp"
#include "res/GL_heads/Shader.h"
#include "SimuCore.h"
#include "Message.h"

/*
* ImGuiҪ��ע�⣺��������button��������Ʋ���Ҫ��һ�������������ڰ�ť������һ����ֻ�ܴ�����һ����ť
* ֻ������ ImGui::Button("Press") �����еĵ�һ����ע���ԭ���� - Ĭ������� - ImGui ʹ�ð�ť���ı���Ϊ���ʶ����
* ����ͨ���ڰ�ť��Χʹ�� PushID() �� PopID() �������µ� ID ��ջ������ͨ���ڰ�ť�ı�����ӡ�##������ť���������Ϣ����ܴ����⡣
* ���磺 ImGui::Button("Press##1") �� ImGui::Button("Press##2") ����ע�ᣬ�����߶�ֻ�С����¡���ť��
* ��##��֮��������ı������������øð�ť�ı�ʶ����������ʾ��
* 
* ImGui�ؼ��ģ�һ�㣩��һ��const�ı���������ҪΪ" "������Ϊ""�գ���Ϊ�����ݻ���ΪID����Ϊ��""�����Ұָ��

��ɫ��ʾ�߼�������Ϊ��ʱ��ʾ��ɫ������Ϊ��ʱ��ʾ������ɫ

BC33��Ӧ17��18��װ��¥A¥��λ����ߣ�18��������� x = 3630 y = 1325
BC15��Ӧ15��16��װ��¥B¥��λ���ұߣ�15�������ұ� x = 3900 y = 1175
18, 17, 16, 15��

ֱ�Ӽ��㱱�˺��϶˵ĸ���ֵ����vertShader�У���yֵ���ڷ�Χ�ڣ���stateֵ��Ϊ-1����fragShader����state<-0.9�Ͷ���Ƭ��
train_coords[]��28�ڳ�Ƥ���޼��㣬������ǰ�г����ȵĳ�Ƥ��Ӧstate��Ϊ-1

Type_train
1��ʯ		2ˮ��		3��ҵ��		4ֽ��		5��ɰ		6ͭ��		7ʯ�ͽ�			8��ú		9����ú		10��ú		12�Ż�		13����ú	14����
1���		2�ɻ�		3��ͬ		4��		5��ú		6����˹ú	7����˹����	8��ú
1���и�	2�񻪾�		3��̩		4����		5�ص׻�		6���		7����			8��һ		9��		10����

*/

struct Type_train
{
	int type_index;			//����С���ţ����嵽��������ƣ���1��ʼ����
	int type_type;			//��������ţ�1Ϊ����������2Ϊ��Դ������3Ϊ��ҵ����
	std::string type_name;	//�ѻ�����
};

struct BuildingAttrib
{
	int building_state;				//װ��¥����״̬��-1��ɫϵͳ���ߣ�ͣ��״̬����0��ɫδ�ϵ����ϣ�1��ɫ����
	int building_pow;				//װ��¥���й���
	int building_mode;				//װ��ģʽ�л���0�Ƚ��ȳ�FIFO��1����ȳ�LIFO��2��16��18������3�ң�15��17����
};

struct One_wagon
{
	int wagon_idx;		//��Ƥ��ţ���1��ʼ��
	float amount;		//��Ƥ�ػ���
};

struct TrainAttrib
{
	int train_idx;					//�𳵱��
	float speed_train;				//������
	int train_type;					//���˻����ࣨ0��ʾδ���壩
	int train_index;				//���˻�С�ࣨ0��ʾδ���壩
	int train_lane;					//�𳵳�����0Ϊ15����1Ϊ16����2Ϊ17����3Ϊ18��
	int train_state;				//��״̬��-1��������ɫ���ı��ر���������Χ��discard����0�ȴ�װ������ɫ����1����װ����ÿ�ڳ�Ƥ�����ذ�ɫ�����غ�ɫ����2������ۣ���ɫ��
	int total_num;					//�𳵳�Ƥ����A¥���28�ڣ�B¥���24��
	int current_num;				//��ǰװ����Ƥ��ţ���1�ӵ�total_num����ʼֵΪ1
	float first_pos;					//��һ�ڳ�Ƥ��yλ��
	float max_per_wagon;			//ÿ��Ƥ����װú����62-67��
	std::vector<One_wagon> wagons;	//����úֵ
	float train_coords[7];			//�����꣺x,y,z,state��ǰ״̬,percent�洢���ٷֱȣ�focus��ý��㣨δ����ʱx,y��Ϊ0����type��������
};

class TrainLoader
{
public:
	TrainLoader();
	void reset(SimuCore& core, bool rand_init);								//����
	void train_random_initiator(SimuCore& core);							//�������ʼ��
	void drawBuilding(Camera& camera, Shader& buildingShader);
	void drawTrain(Camera& camera, Shader& trainShader);
	void initGuiStyle();														//��ʽ��ʼ��
	int train_dispatch(Message& message);									//ImGui�𳵵��ȿ���̨
	int updateTrains(float simurate);										//��״̬
	void add_type(std::string str_name, int type_type);					//��ӻ�������
	bool run(std::vector<std::string>& equipments, int type, int index);	//�������У�����true�����ɹ���˫�����г�����װ����۵ȴ��ĳ���������false������û�г����޷�����
	void shutDown(std::vector<std::string>& equipments);					//����ֹͣ
	void set_focus(std::vector<std::string>& equipments);					//ѡ�����̵���ʾ
	void lose_focus();														//ʧ�����������ʾ

private:
	ImGuiStyle* style;						//ImGui��ʽ�洢
	Type_train type_unit;					//���ﵥԪ����������������ż�������
	std::vector<Type_train> names;			//���������������ƴ洢����
	bool lane_occupied[4];					//����ռ�ñ�ʶ
	int index_occupied[4];					//�г����ռ�ñ�ʶ
	float north_thresh;						//�г���ʾ����
	float south_thresh;						//�г���ʾ����
	float train_x_bias;						//�г�װ��¥ˮƽ����ƫ��
	float length_wagon;						//����Ƥ����
	BuildingAttrib building_A;				//17 18��BC33װ��A¥
	BuildingAttrib building_B;				//15 16��BC15װ��B¥
	float building_coords_A[6];				//װ��¥A���꣺x,y,z,state,focus,mode��ȡֵ0-3��
	float building_coords_B[6];				//װ��¥B���꣺x,y,z,state,focus,mode��ȡֵ0-3��
	unsigned int buildingVBO, buildingVAO;
	One_wagon wagon;
	TrainAttrib train;
	std::vector<TrainAttrib> trains;			//�г��洢����
	unsigned int trainVBO, trainVAO;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	void pre_button();
	void post_button();
	void set_button(int index, bool building);

};
