#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include <glm/glm.hpp>
#include <imgui.h>
#include "res/GL_heads/Camera.hpp"
#include "res/GL_heads/Shader.h"
#include "Web.h"

/*
* ע�⣺sort�㷨�еıȽϽ�֧������<����֧��<=
�����豸״̬��ת��ɫ�󣬸�Ƥ�����Կ������У�Ҳ����ǳ��ɫ����״̬�С����̲����豸�����ο������к���ĩλ�豸����жú��������˸������У�ѡ�е������豸��ɫ��Ϊ����ɫ�����븺������״̬

�п��ҵ�����ָ���һ��ĳĳ������ѡ��ָ���ĳĳ�����̵������豸�������󣬲������̵������豸����ʾ���϶���ʾ��ɫ����˸�źţ���ʾ�п��Ұ��´������а�ť��Ȼ����豸���մ���������У�Ҳ���ǿ������У���ɫ���α�Ϊǳ��ɫ
ĳĳ�����̲����豸ȫ������������ϡ��п��Ұ��¸������У����豸��ɫ��Ϊ����ɫ��������������豸һ����������ú̿���źš�

BC7A��BC7Bȫ���ϲ�ΪBC7
XBC1��XBC2��ʾΪXC1��XC2
*/

struct Type_conv
{
	int type_index;			//����С���ţ����嵽��������ƣ���1��ʼ����
	int type_type;			//��������ţ�1Ϊ����������2Ϊ��Դ������3Ϊ��ҵ����
	std::string type_name;	//�ѻ�����
};

struct ConvAttrib
{
	std::string conv_name;		//Ƥ������
	int conv_state;				//Ƥ������״̬��-1��ɫϵͳ���ߣ�ͣ��״̬����0��ɫδ�ϵ����ϣ�1��ɫƤ�����ֳ��Ѿ�ͨ�磬�������ʩ�Ѿ������������п��Ҹ�Ƥ�����Խ����²����̻��ڣ������������Ƥ��������ɣ�2ǳ��ɫ���У����أ���3�������У����أ�
	int conv_idx;				//Ƥ�����
	int conv_pow;				//Ƥ�����й���
	float conv_speed;			//����
	int conv_length;			//Ƥ�����ȣ��ף�
	float conv_max_storage;	//Ƥ����߻�����������Ƥ�����Ⱦ���
	float conv_cur_storage;		//Ƥ����ǰ����
	int flow_name;				//ռ��������ţ���ԭ��������stoiת��Ϊint����-1Ϊδռ��
	int flow_idx;				//��ռ�������еļ��Σ��ڼ�λ������0��ʼ��
	int conv_type;				//�����ࣨ0��ʾδ���壩
	int conv_index;				//��С�ࣨ0��ʾδ���壩
};

struct FlowState
{
	int flow_num;		//���̴���
	bool begin;			//������ʼ�Ƿ��ڹ������true��
};

class ConvSort
{
public:
	bool operator()(ConvAttrib conv_1, ConvAttrib conv_2)
	{
		return conv_1.flow_idx < conv_2.flow_idx;
	}
};

class ConvResort
{
public:
	bool operator()(ConvAttrib conv_1, ConvAttrib conv_2)
	{
		return conv_1.conv_idx < conv_2.conv_idx;
	}
};

class Conveyor
{
public:
	Conveyor();
	void reset();																					//����
	void draw(Camera& camera, Shader& convShader, float value);									//����
	void initGuiStyle();																				//��ʽ��ʼ��
	void conv_dispatch(bool unreal);																//����״̬��ʾ
	void updateConvs(float gapTime, float simurate, float convAmount[51]);							//����Ƥ��
	void add_type(std::string str_name, int type_type);											//��ӻ�������
	bool startCheck(std::vector<std::string>& equipments);											//��鴫����豸���������Ƿ񶼿�����������������һ�£�����falseΪ��ֹ����������trueΪ��������
	bool sceneCheck(std::vector<std::string>& equipments);											//�ֳ����
	void run_unloaded(std::vector<std::string>& equipments);										//��������
	bool runCheck(int type, int index, std::vector<std::string>& equipments);						//����Ӧ��Ƥ���豸���Ƿ���ڳ�ͻ�Ļ����࣬����false��ʾ����������
	void run_loaded(int index_flow, int type, int index, std::vector<std::string>& equipments);	//��������
	void shutDown(int index_flow);																	//����ֹͣ
	void cancel(std::vector<std::string>& equipments);												//����ȡ��
	void cancel_web(int index_flow);																//���̱�Ϊ��ɫ��ȡ��֮��һ���¼���������web�ļ���
	//bool cancelCheck(std::vector<std::string>& equipments);											//����ȡ����飬����true��ʾ����ȡ��������
	void emergency_shutDown(int index_flow, std::vector<std::string>& equipments);				//���̼�ͣ
	void set_focus(std::vector<std::string>& equipments);											//ѡ�����̵���ʾ
	void lose_focus();																				//ʧ�����������ʾ

private:
	ImGuiStyle* style;					//ImGui��ʽ
	Type_conv type_unit;				//���ﵥԪ����������������ż�������
	std::vector<Type_conv> names;		//���������������ƴ洢����
	ConvAttrib conv;					//Ƥ��״̬�ṹ��
	float conv_coords[459];				//Ƥ������VBO������x1,y1,x2,y2,z,r,g,b,focus
	std::vector<ConvAttrib> belts;		//Ƥ�����ݴ洢����
	unsigned int convVBO, convVAO;		//VAO & VBO
	FlowState flow_state;				//����״̬�洢�ṹ��
	std::vector<FlowState> act_flows;	//���������״̬
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	void pre_button();
	void post_button();

};
