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
#include "Message.h"

/*
* ʵ��ץ������λ���ڴ��϶��Ϸ�����crab_bucket_hor = 1������-1
* ʵ��ץ����crab_bucket_hor = 1λ�þ����ڴ��϶��Ϸ����ͼ����Ƥ�������޹أ���ʾ������ץ����A��B��������Ϊ�˿��ӻ�
* ʵ��ץ��������·�߲���ֱ�ǵģ���ץ���ƶ������п���һ��ˮƽ�ƶ���һ�ߴ�ֱ�ƶ���������ģ�ͼ�Ϊץ��������λ�õ����ֱ�������ϣ����ץ��bucket_speed_hor���Ա�ʾ��ץ��ˮƽ�ƶ�����ֱ��λ֮ǰ��ȫ���̣�bucket_speed_ver����ץ��ץú��жú���ٶȣ�������ʾץ���ڴ�ֱ�ƶ����ٶȡ�
���������ÿ�����ҽ��ṩһ������ú̿����
ץ����һ��ץȡ��������ץȡѭ���������ٱ�ץ�ϵĲ���ú��
ץ����жú�����н�úת����ж�������϶��������У�������ӱ仯
ж�������϶�����ץ������ú�ٶȣ�Ҳ�����ɵ�ǰ���϶���ú�������Ծ�����ú�ٶȣ���������Ƥ�����ú
����ϵͳ����ӣ�
	ж��������+����ͣ��λ���Ŷ�
	��жú��ǰ�ڸ�����ʱ����ж��ж�ٿ죬�������
	ץ����6���׶��ƶ��ٶȵ�����

��ֻ��λ�����߼���
4��λ����С�ڵ���2���
21#22#��λ��
	����һ��λͣ��6.5��ּ���Ĵ�����һ��λ���ܿ���
	��һ��λͣ��5��ֵĴ�����һ��λֻ��ͣ2��ּ����µĴ�
	��һ��λͣ��3.5��ֵĴ�����һ��λֻ��ͣ3.5��ּ����µĴ�

1��ʯ		2ˮ��		3��ҵ��		4ֽ��		5��ɰ		6ͭ��		7ʯ�ͽ�			8��ú		9����ú		10��ú		12�Ż�		13����ú	14����
1���		2�ɻ�		3��ͬ		4��		5��ú		6����˹ú	7����˹����	8��ú
1���и�	2�񻪾�		3��̩		4����		5�ص׻�		6���		7����			8��һ		9��		10����

*/

struct Type_ship
{
	int type_index;			//����С���ţ����嵽��������ƣ���1��ʼ����
	int type_type;			//��������ţ�1Ϊ����������2Ϊ��Դ������3Ϊ��ҵ����
	std::string type_name;	//�ѻ�����
};

struct ShipUnloader
{
	std::string unloader_name;		//ж�������
	int unloader_index;				//ж������ţ�0-7
	int unloader_state;				//ж����״̬��0��ɫδ���У�1��ɫ�豸������2��ɫ��������
	float crab_bucket_hor;			//ץ��ˮƽλ�ã�-1.0f���ã�0.0f�ڴ��ϣ�1.0f��Ƥ����
	float crab_bucket_ver;			//ץ����ֱλ�ã�0.0f��ʾ����λ��1.0f��ʾ����λ
	bool crab_bucket_switch;		//�Ż�ץ��λ������Ƥ����0AƤ����1BƤ��
	bool crab_bucket_loaded;		//ץ����ȡ���ź�
};

struct Pos_bucket
{
	int ship;		//����λ��
	int conv_A;		//AƤ��λ��
	int conv_B;		//BƤ��λ��
	int idle;		//����λ��
};

struct ShipLoader
{
	std::string loader_name;		//װ�������
	int loader_state;				//װ����״̬��0��ɫδ���У�1��ɫ�豸������2��ɫ��������
	int loader_pow;					//װ�������й���
};

struct Ship
{
	std::string ship_name;					//�����
	int ship_state;							//��ֻ״̬��0δ���ۣ�����ʾ����1��ê�صȴ�����ê����ʾΪ��ɫ����2���ڿ�������ʾΪ��ɫ����3��������ʾΪ��ǰú���ٷֱȣ���4������ۣ���ʾΪ��ɫ��
	float wait_pos[2];						//ê��λ�ã����пؾ������ȴ�����ۺ�ͬʱ����ship_state��wait_pos��berth_idx����������״̬�л�ʱ�Զ����㴬����ǰλ�á��Ƕȡ�ע�⣺��ֵΪԭͼ�еĴ�ֵ
	double angle;							//�ƶ�����
	int berth_idx;							//����Ĳ�λ��0Ϊ21�Ų�λ��1Ϊ22�Ų�λ��2Ϊ4�Ų�λ�ң�3Ϊ4�Ų�λ��4Ϊ23��λ
	float ship_total_storage;				//����������λ����level 0.7Ϊ5ǧ�֣�0.9Ϊ2��֣�1.1Ϊ3.5��֣�1.3Ϊ5��֣�1.5Ϊ6.5��֣�
	float ship_current_storage;			//��ǰ����
	int ship_index;							//��С�ࣨ0��ʾδ���壩
	int ship_type;							//�����ࣨ0��ʾδ���壩
	float ship_coords[8];					//�������꣺x,y,z,state��ǰ״̬,percent�洢���ٷֱȣ�focus��ý��㣨δ����ʱx,y��Ϊ0����level��λ����type��������
	long double ship_dist;					//�����벴λ�ľ��룬������뿪ʼ�������Ϊ������ʼ�뿪��λ�����趨Ϊ����״̬
};

struct Pos_Berth
{
	float x_4_1;		//4����λ����x���꣬��Ӧ�Ż�11#��12#��berth_idx = 3
	float x_4_2;		//4���Ҳ�λ����x���꣬��Ӧ�Ż�13#��14#��berth_idx = 2
	float x_21;			//21�Ų�λ����x���꣬��Ӧ�Ż�1#��2#��berth_idx = 0
	float x_22;			//22�Ų�λ����x���꣬��Ӧ�Ż�3#��4#��berth_idx = 1
	float x_23;			//23�Ų�λ����x���꣬��Ӧ�Ż�SL4��berth_idx = 4
};

//��λ
class Berth
{
public:
	Berth();
	void reset(SimuCore& core, bool rand_init);												//����
	void ship_random_initiator(SimuCore& core);											//���������ʼ��
	int berth_finished;																		//ж�������λ��ţ�-1Ϊ��������0��3Ϊ��λ��AƤ����4��7Ϊ��λBƤ��
	bool webUnloaders[4];																	//ж�����Ƿ���ã������������
	ShipLoader loader;																		//װ�������Խṹ�壬���ý�Ȩ������Ϊprivate�������ڹ��캯�����������ʳ�ͻ
	void drawUnloader(Camera& camera, Shader& unloaderShader, Shader& bucketShader);
	void drawLoader(Camera& camera, Shader& loaderShader);
	void drawShip(Camera& camera, Shader& shipShader);
	void initGuiStyle();																		//��ʽ��ʼ��
	int ship_dispatch(Message& message);													//�������ȿ���̨��������Ҫ����ֹͣ�Ĳ�λ���
	void unloader_dispatch();																//ж��������̨
	void updateBuckets(float gapTime, float simurate);										//����ץ��״̬
	int updateShips(float gapTime, float simurate);											//���´�ֻ״̬������1Ϊװ��װ������
	void add_type(std::string str_name, int type_type);									//��ӻ�������
	bool set_unloading_ship(Message& message, std::vector<std::string>& equipments);		//ж����ж��׼����飬����falseΪ��ʼʧ��
	bool set_loading_ship(Message& message, int load_type, int load_index);				//װ����װ��׼����飬����falseΪװ������ʧ��
	void run_unloader_unloaded(Message& message, std::vector<std::string>& equipments);	//��������
	void run_unloader_loaded(std::vector<std::string>& equipments);						//��������
	void unloader_shutDown(std::vector<std::string>& equipments);							//����ȡ��
	int get_up_type();																		//��ȡͨ�ò�λ�������
	int get_up_index();																		//��ȡͨ�ò�λ����С��
	float get_up_amount();																	//��ȡͨ�ò�λ��ҵ��
	int get_down_type();																	//��ȡ4��λ����С��
	int get_down_index();																	//��ȡ4��λ����С��
	float get_down_amount();																//��ȡ4��λ��ҵ��
	void set_focus(std::vector<std::string>& equipments);
	void lose_focus();

private:
	ImGuiStyle* style;								//ImGui��ʽ�洢
	std::string info_ships_name[34];				//��������
	float info_ships_storage[34];					//�����ػ���
	bool info_ships_occupied[34];					//������Ӧ����Ƿ�ע�ᣬע����Ĵ�ֻ�����ظ�ע��
	bool berth_occupied[5];							//��λռ��״̬
	bool unload_mode_gen;							//ͨ�ò�λж��ģʽ��falseΪFIFO�Ƚ��ȳ���trueΪLIFO����ȳ�
	Type_ship type_unit;							//���ﵥԪ����������������ż�������
	std::vector<Type_ship> names;					//���������������ƴ洢����
	ShipUnloader unloader;							//ж�������Խṹ��
	std::vector<ShipUnloader> unloaders;			//ж����״̬�洢����
	float unloader_coords[40];						//ж�������꣺x,y,z,state�豸״̬,focus��ý���
	Pos_bucket bucket_up;							//1-4#�Ż��϶�����(int)
	Pos_bucket bucket_down;						//11-14#�Ż��϶�����(int)
	float bucket_coords[40];						//ץ�����꣺x,y,z,ver�϶���ɫ��־0��ɫ1��ɫ,focus��ý���0ʧȥ����1
	unsigned int unloaderVBO, unloaderVAO;			//VAO & VBO
	unsigned int bucketVBO, bucketVAO;				//VAO & VBO
	float loader_coords[5];							//װ�������꣺x,y,z,state,focus
	unsigned int loaderVBO, loaderVAO;				//VAO & VBO
	Ship ship;										//�������Խṹ��
	std::vector<Ship> ships;							//��������
	Pos_Berth berth_pos;							//��λx����洢����(float)
	unsigned int shipVBO, shipVAO;					//VAO & VBO
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	int get_ship_type(float total_storage);						//�ж�������λ��0Ϊ5ǧ�֣�1Ϊ2��֣�2Ϊ3.5��֣�3Ϊ5��֣�4Ϊ6.5���
	float get_ship_level(float total_storage);						//�ж�������λ��0.7Ϊ5ǧ�֣�0.9Ϊ2��֣�1.1Ϊ3.5��֣�1.3Ϊ5��֣�1.5Ϊ6.5���
	bool type_check(int type, int index);							//����Ƿ������Ӧ������Ķ��壬����true��ʾ���ڶ���
	void bucket_unload(float gapTime, float simurate, ShipUnloader& unloader);	//ץ�����й����м��ٶ�Ӧ�Ż���λ�����ĵ�ǰ�ػ���
	void pre_button();
	void post_button();

};
