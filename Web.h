#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <imgui.h>
#include "res/GL_heads/Camera.hpp"
#include "res/GL_heads/Shader.h"
#include "SimuCore.h"

/*
ж����->Ƥ��->���ֻ�
���ֻ�->Ƥ��->װ��¥
���ֻ�->Ƥ��->Ͳ�֣�ֱ����
�ѳ��ʹ����ڴ�ϵͳ�б�ʡȥ

��ú̿Ƭ�ε��������㵽Ƥ��������ʱ����Ϊú̿Ƭ���Ǿ��ȵģ���ÿ��Ƥ�������ϵ�ú̿����ͬ��

*/

struct Type_web
{
	int type_index;			//����С���ţ����嵽��������ƣ���1��ʼ����
	int type_type;			//��������ţ�1Ϊ����������2Ϊ��Դ������3Ϊ��ҵ����
	std::string type_name;	//�ѻ�����
};

//ú̿Ƭ�Σ�����ʱ��ΪԴͷ��������л�Ϊ�������˲��
struct Fragment
{
	std::vector<std::string> froms;		//��Դ�豸���飬��������ж������Ƥ������Դ�ڵ���4�����̶��������Ƿ��������
	//std::string from;					//��Դ�豸��
	std::string to;						//��ֹ�豸��
	float total_length;					//Ƥ���ڵ�ǰ������Դ����ֹ�豸���ܳ��ȣ��ڵ�ǰƬ�δ���ʱ����
	float begin;							//��ͷλ�ã����������̽�βʱ��������Ƭ�ε�ú������ͷ������ǰ�ƽ�
	float end;							//ĩβλ�ã�����������Ϊδ�ѹ�������ǰԴͷλ�ò��жϹ����������ĩβλ�ù̶���Դͷλ���غ�
	bool disconnected;					//�ѹ���־���տ�����״̬false��һ��ú̿Ƭ��ΪԴͷ��������л�Ϊ�������˲�䣻�ѹ�˲�䣨״̬��Ϊtrue��ΪԴͷ��������л�Ϊ�������˲��
	float amount;						//�ػ�������ÿ��ʱ��Ƭ�������������������������ǰƬ�ε�begin�뱾����ĩβλ���غϣ�����Ƥ�����ټ��ٱ�����������ǰƬ��endλ���뿪ͷλ���غϣ������ϼ����������Ч�����������������������Ŷ���
	int cargo_type;						//������࣬0��ʾδ����
	int cargo_index;					//����С�࣬0��ʾδ����
};

//��¼�ڵ�ǰ������������Ϣ�Ľṹ��
struct PrePos
{
	std::string other_name;				//�ڵ�Ĵ���
	bool act;							//�˽ڵ��Ƿ��뵱ǰ�ڵ�����������
};

//Ƥ���ڵ��е�ú̿Ƭ��
struct FragChild
{
	float fragCoords[7];				//Ƭ������VBO��x1,y1,x2,y2,z,type,focus
};

//�������ڵ�
struct Node
{
	std::string name;					//���ڵ�����
	std::vector<PrePos> pre_codes;		//���ڵ�ǰ�����ӽڵ�Ĵ���
	std::vector<PrePos> pos_codes;		//���ڵ�������ӽڵ�Ĵ���
	int this_code;						//��ǰ�ڵ���루��0��ʼ��
	int type;							//�ڵ����࣬0��ʾ������1��ʾж������2��ʾƤ����3��ʾ���ֻ���4��ʾװ������5��ʾ�ѳ���6��ʾͲ�֣�7��ʾװ��¥��8��ʾ����ֱ����9��ʾ����3��λ�����룬10��ʾ�ؿ�
	int convLength;						//���ڵ㣨����Ƥ�����ĳ���
	float convSpeed;					//���ڵ㣨����Ƥ�����Ĵ���
	float convPos;						//���ڵ㣨����Ƥ�����ڵ�ǰ�����е��ۼƳ���λ�ã��ڵ�ǰ���̱��趨ʱ���㣬���ȱ��λ��Ϊ��Ƥ����ĩβ��
	float convCoords[6];					//���ڵ㣨����Ƥ�������꣺x1,y1,x2,y2,z,focus
	int convCode;						//Ƥ����ţ���0��ʼ
	std::vector<FragChild> frags;		//���ڵ㣨����Ƥ������Ƭ�δ洢����
	bool wheelMode;						//���ڵ㣨���Ƕ��ֻ���������ģʽ��0Ϊ���ϣ�1Ϊȡ��
	bool out;							//���ڵ㣨��������Դ�ڵ㣩�����״̬��0Ϊû���������1Ϊ�������
	float out_flux;						//���ڵ㣨��������Դ�ڵ��ȡ��ģʽ���ֻ������������
	int out_type;						//���ڵ㣨��������Դ�ڵ��ȡ��ģʽ���ֻ���������������
	int out_index;						//���ڵ㣨��������Դ�ڵ��ȡ��ģʽ���ֻ������������С��
};

class FindNode
{
public:
	std::string target;
	bool operator() (Node node)
	{
		return node.name == target;
	}
};

class Web
{
	friend class Record;
public:
	Web();
	void reset();
	void initGuiStyle();
	void drawFrags(Camera& camera, Shader& fragsShader);
	void update(SimuCore& simucore, float gapTime, float simurate);
	void add_type(std::string str_name, int type_type);
	void web_dispatch(bool unreal, bool authorised);																//��һ������Ϊtrue��ʾ����ǿ��ָ���������������ࣻ�ڶ�������Ϊtrue��ʾ��Ȩ�޿���Դ�ڵ�����뿪�����
	void start_input(std::string from_name, int type, int index, float flux);										//����Դ�ڵ����루�ѻ����ƣ�������ı������߼������Խ���˹���������������ȡ����ʹ�ã�
	void end_input(std::string from_name);																			//����Դ�ڵ����루�ѻ����ƣ�
	void begin_flow(std::vector<std::string>& equipments, int type, int index, float flux, bool unloader[4]);		//���̿�ʼ¼�룬���һ�������ʾж��������4̨ж�����Ŀ��������˳���1��4����0�رգ�1����
	void end_flow(std::vector<std::string>& equipments);															//���̽�����¼
	void emergencyShutDown(std::vector<std::string>& equipments);													//���̽���ͣ��
	bool cancelCheck(std::vector<std::string>& equipments);															//����ȡ����飬����true��ʾ����ȡ��������
	void set_focus(std::vector<std::string>& equipments);
	void lose_focus();
	float convAmount[51];				//Ƥ����ǰ�ػ���
	std::string finishEndName;			//���̽������ս����̵�ĩβȥ���豸��

protected:
	int find_to(std::string toName);	//����ȥ���豸�Ľڵ���ţ������Record�࿪�ţ�
	std::string find_toName(int idx);	//����ȥ���豸���豸���������Record�࿪�ţ�

private:
	ImGuiStyle* style;
	Type_web type_unit;
	std::vector<Type_web> names;
	Fragment fragment;						//ú̿Ƭ��
	std::vector<Fragment> fragments;		//ú̿Ƭ��Ⱥ
	PrePos prepos;							//ǰ��ڵ�ṹ��
	FragChild frag_child;					//��ú̿Ƭ��
	Node node;								//�ڵ�
	std::vector<Node> nodes;				//������
	unsigned int fragVBO, fragVAO;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	void pre_button(bool choosed);
	void post_button();

};
