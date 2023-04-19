#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <imgui.h>
#include "Message.h"

/*
ע�⣺���ع�Ӧ������������PT���ף����ù񣬷ֱ��������޹����������ñ䣬�ֶ����磬���ߣ����룬�����񣬳��߹�ȣ��ڴ�ϵͳ��ͳͳ����
�����Ŀ��ع������豸ǰ����@ǰ׺��ʾ��ǰ���ع����������������������������ͷ�豸Ⱥ��

�ַ���u8�����ʽ��ascii�뷶Χ���ַ�ֵ��ͬ�������ĵ�u8�ַ��ͷ�u8�ַ�ֵ��ͬ

*/

//���ع�����ϵͳ�е����п��ع��Ǹ�ѹ���ع������̶��ǻ�ͨ
struct switchGear
{
	int gear_idx;						//���ع����
	int pow;								//���ع��Ӧ�豸�Ķ���й���
	int state;							//����״̬��0��ʾ�䱸״̬��1��ʾ�ȱ�״̬��2��ʾ����״̬
	float time;							//������ʱ��
	float pow_rate;						//��ǰ���ʱ���
	std::string gear_transformer;		//�����ı�ѹ��
	std::string gear_type;				//���ع��ͺ�
	int transformation_ratio;			//��ȣ�Ϊ������ʾ������ȣ�Ϊ0��ʾPT���ף�Ϊ����ʾ���ñ�KV
	std::string gear_provider;			//���ع�������
	std::string equipment;				//���ع���Ƶ��豸������ĸΪu8'@'��ʾ�ù���񲻷�������ͷ�豸Ⱥ��
};

//��ѹ���������̶�������Ƽ�
struct Transformer
{
	std::string transformer_name;			//��ѹ������
	std::vector<switchGear> gears;			//��ǰ��ѹ�����������п��ع�
	int transformer_idx;					//��ѹ����ţ���0��ʼ��0Ϊ���ı�
	std::string transformer_type;			//��ѹ���ͺ�
	int max_pow;							//��߹�����������λKVa��
	float cur_pow;							//��ǰ����ֵ
	float cur_energy;						//��ѹ���ĵ�ǰ���ܺ�
	std::string transformer_provider;		//��ѹ��������
};

class Energy
{
public:
	Energy();
	void reset();															//����
	void update(Message& message, float simurate, float gapTime);			//����״̬
	void coldStart(std::vector<std::string>& equipments);					//���䱸״̬��Ϊ�ȱ�״̬
	void hotStart(std::vector<std::string>& equipments);					//���ȱ�״̬��Ϊ����״̬
	void hotEnd(std::vector<std::string>& equipments);						//������״̬��Ϊ�ȱ�״̬
	void coldEnd(std::vector<std::string>& equipments);						//���ȱ�״̬��Ϊ�䱸״̬
	void emergencyEnd(std::vector<std::string>& equipments);				//����ͣ��
	std::vector<std::string> getEquipments();
	void initGuiStyle();														//��ʽ��ʼ��
	void transformerDispatch();												//��ѹ����ʾ����
	int trip;																//��բ�ı�ѹ����ţ�-1��ʾû��բ

private:
	float total_energy;							//���ܺ�
	ImGuiStyle* style;							//ImGui��ʽ�洢
	switchGear gear;							//���ع�ṹ��
	Transformer transformer;					//��ѹ���ṹ��
	std::vector<Transformer> transformers;		//��ѹ������
	void pre_button(bool choosed);				//��ť��ʽ����
	void post_button();

};
