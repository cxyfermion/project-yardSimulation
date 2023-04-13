#pragma once
#include <string>
#include <vector>
#include <imgui.h>

/*
ж����->Ƥ��->���ֻ�
���ֻ�->Ƥ��->װ��¥
���ֻ�->Ƥ��->Ͳ�֣�ֱ����
�ѳ��ʹ����ڴ�ϵͳ�б�ʡȥ

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
	float begin;				//��ͷλ�ã����������̽�βʱ��������Ƭ�ε�ú������ͷ������ǰ�ƽ�
	float end;				//ĩβλ�ã�����������Ϊ�ѹ�������ǰԴͷλ�ò��жϹ����������ĩβλ�ù̶���Դͷλ���غ�
	bool disconnected;		//�ѹ���־���տ�����״̬false��һ��ú̿Ƭ��ΪԴͷ��������л�Ϊ�������˲�䣻�ѹ�˲�䣨״̬��Ϊtrue��ΪԴͷ��������л�Ϊ�������˲��
	float amount;			//�ػ�������ÿ��ʱ��Ƭ�������������������������ǰƬ�ε�begin�뱾����ĩβλ���غϣ�����Ƥ�����ټ��ٱ�����������ǰƬ��endλ���뿪ͷλ���غϣ������ϼ����������Ч�����������������������Ŷ���
	int cargo_type;			//������࣬0��ʾδ����
	int cargo_index;		//����С�࣬0��ʾδ����

};

//��¼�ڵ�ǰ������������Ϣ�Ľṹ��
struct PrePos
{
	std::string other_name;				//�ڵ�Ĵ���
	bool act;							//�˽ڵ��Ƿ��뵱ǰ�ڵ�����������
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
	bool wheelMode;						//���ڵ㣨���Ƕ��ֻ���������ģʽ��0Ϊ���ϣ�1Ϊȡ��

};

class Web
{
public:
	Web();
	void reset();
	void initGuiStyle();
	void update();
	void add_type(std::string str_name, int type_type);
	void web_dispatch(bool unreal);

private:
	ImGuiStyle* style;
	Type_web type_unit;
	std::vector<Type_web> names;
	Fragment fragment;						//ú̿Ƭ��
	std::vector<Fragment> fragments;		//ú̿Ƭ��Ⱥ
	PrePos prepos;							//ǰ��ڵ�ṹ��
	Node node;								//�ڵ�
	std::vector<Node> nodes;				//������
	void pre_button(bool choosed);
	void post_button();

};
