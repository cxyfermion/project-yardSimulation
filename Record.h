#pragma once
#include <fstream>
#include <glm/glm.hpp>
#include <imgui.h>
#include "Environment.h"
#include "Flow.h"

//�п�
struct ConsoleFile
{
	bool random_initiating;			//�����ʼ��ѡ��λ
	int compitence;					//Ȩ�ޣ�0�οͣ�1ж������ʻԱ��2��������Ա��3�𳵵���Ա��4�𳵴����ܵ���Ա��5���̹���Ա��6������Ա��7�ѳ�����Ա��8Ͳ�ֹ���Ա��9���������Ա��10��������Ա��11����Ա��12�ϵ�
};

//������ļ�
struct CoreFile
{
	double time;					//����������ʱ�䣨�����Ƿ���ͣ��
	double simuTime;				//��������ʱ��
	bool simu_pause;				//����״̬��trueΪ��ͣ��falseΪ����
	float run_rate;					//���������ٶ�
	bool stepping;					//����״̬
	bool reset_zero;				//���á�����״̬
	bool reset_rand;				//���á���������������������������Ƚ����������
	int runtime_minutes;			//��������ʱ��-��
	int runtime_hours;				//��������ʱ��-ʱ
	double simu_deltaTime;
	bool freshRequire;				//ˢ��ʱ�̱�־λ��true��ʾ��Ҫˢ��
	double freshGapTime;			//ˢ�¼��ʱ�䣬ˢ�¼��ʱ���ǹ̶��ģ������ܷ�������Ӱ�죬������ʱ����ܷ�������Ӱ��
	int freshGapSwitch;				//ˢ�¼��ʱ��ѡ�񣬳�ʼֵΪ50ms��20֡����1��ʾ20ms50FPS��2��ʾ50ms20FPS��3��ʾ100ms10FPS��4��ʾ200ms5FPS��5��ʾ500ms2FPS��6��ʾ1000ms1FPS
	double curGapTime;				//��ǰ�ۼ�ˢ��ʱ��
	double simu_lastFrame;			//��һˢ��ʱ���¼
	double terminate_time;			//�����ս�ʱ��
	double terminate_countdown;	//�����սᵹ��ʱ
	int runtime_seconds;			//��������ʱ��-��
	int runtime_days;				//��������ʱ��-��
};

//��Դ��洢
struct EnergyFile
{
	int trip;										//��բ�ı�ѹ����ţ�-1��ʾû��բ
	float total_energy;								//���ܺ�
	float cur_pow[8];								//8̨����ĵ�ǰ�ܺ�
	float cur_energy[8];							//8̨����ĵ�ǰ���ܺ�
	int state[153];									//���ع�״̬
	float time[153];								//���ع�������ʱ��
	float pow_rate[153];							//���ع�������ʱ��
};

//������洢
struct EnvironmentFile
{
	int weather;							//������0Ϊ���죬1Ϊ���죬2Ϊ���꣬3Ϊ���꣬4Ϊ��������5Ϊ��磬6Ϊ��ѩ��7Ϊɳ������8�ױ�
	float lightPos[3];						//��Դλ�ã�ע���ȡת��Ϊvec3
	float deltaTime;
	float lastFrame;
	float circle_radius;						//ģ��̫�����ƶ��뾶
	bool skybox;							//��պп���
	bool background;						//��������
	int texture_act;						//����Ĳ���
	int time_mode;							//ʱ��ӳ��ģʽ��0Ϊû��ӳ�䣬1Ϊʹ�÷���ʱ��ӳ�䣬2Ϊ����ָ��ӳ��
	int minute_local;						//���ط���
	int hour_local;							//����Сʱ
	bool light;								//���տ���
	bool spotlight;							//�۹�ģʽ
};

//�������洢
struct FragmentFile
{
	int froms_code;						//��Դ�豸������������ж������Ƥ������Դ�ڵ���4�����̶��������Ƿ��������
	int to_code;						//��ֹ�豸���
	float total_length;					//Ƥ���ڵ�ǰ������Դ����ֹ�豸���ܳ��ȣ��ڵ�ǰƬ�δ���ʱ����
	float begin;							//��ͷλ�ã����������̽�βʱ��������Ƭ�ε�ú������ͷ������ǰ�ƽ�
	float end;							//ĩβλ�ã�����������Ϊδ�ѹ�������ǰԴͷλ�ò��жϹ����������ĩβλ�ù̶���Դͷλ���غ�
	bool disconnected;					//�ѹ���־���տ�����״̬false��һ��ú̿Ƭ��ΪԴͷ��������л�Ϊ�������˲�䣻�ѹ�˲�䣨״̬��Ϊtrue��ΪԴͷ��������л�Ϊ�������˲��
	float amount;						//�ػ�������ÿ��ʱ��Ƭ�������������������������ǰƬ�ε�begin�뱾����ĩβλ���غϣ�����Ƥ�����ټ��ٱ�����������ǰƬ��endλ���뿪ͷλ���غϣ������ϼ����������Ч�����������������������Ŷ���
	int cargo_type;						//������࣬0��ʾδ����
	int cargo_index;					//����С�࣬0��ʾδ����
};

struct WebFile
{
	float convAmount[51];				//Ƥ����ǰ�ػ���
	float convPos[74];					//�ڵ��е�Ƥ���ۼƳ���
	bool wheelMode[74];					//���ڵ㣨���Ƕ��ֻ���������ģʽ��0Ϊ���ϣ�1Ϊȡ��
	bool out[74];						//���ڵ㣨��������Դ�ڵ㣩�����״̬��0Ϊû���������1Ϊ�������
	float out_flux[74];					//���ڵ㣨��������Դ�ڵ��ȡ��ģʽ���ֻ������������
	int out_type[74];					//���ڵ㣨��������Դ�ڵ��ȡ��ģʽ���ֻ���������������
	int out_index[74];					//���ڵ㣨��������Դ�ڵ��ȡ��ģʽ���ֻ������������С��
};

//������洢
struct FlowFile
{
	bool window_confirm;					//���̸�������ȷ�ϴ���
	bool windowGroundChoose;				//�ؿ�װ������ѡ�񴰿�
	bool groundSelected;					//�ؿ�װ����ѡ��
	int chooseType;							//�ؿ�ѡ������
	int chooseIndex;						//�ؿ�ѡ��С��
	int onfocus;								//��ǰ��������
	bool emergency_stop;					//���̼�ͣ����
	int flow_state[113];
	unsigned int flow_time[113];
	bool scene_ready[113];
};

//Ƥ����洢
struct ConvFile
{
	int conv_state[51];
	float conv_cur_storage[51];
	int flow_name[51];
	int flow_idx[51];
	int conv_type[51];
	int conv_index[51];
	float conv_coords[459];					//Ƥ������VBO������x1,y1,x2,y2,z,r,g,b,focus
};

//���ֻ��洢
struct WheelFile
{
	int wheel_state[8];
	bool wheel_mode[8];
	float wheel_coords[56];					//���ֻ����꣺x,y,z,r,g,b,focus
};

//��λ�洢
struct BerthFile
{
	int loader_state;						//װ����״̬
	int unloader_state[8];
	float crab_bucket_hor[8];
	float crab_bucket_ver[8];
	bool crab_bucket_switch[8];
	bool crab_bucket_loaded[8];
	int berth_finished;						//ж�������λ��ţ�-1Ϊ��������0��3Ϊ��λ��AƤ����4��7Ϊ��λBƤ��
	bool webUnloaders[4];					//ж�����Ƿ���ã������������
	bool info_ships_occupied[34];			//������Ӧ����Ƿ�ע�ᣬע����Ĵ�ֻ�����ظ�ע��
	bool berth_occupied[5];					//��λռ��״̬
	bool unload_mode_gen;					//ͨ�ò�λж��ģʽ��falseΪFIFO�Ƚ��ȳ���trueΪLIFO����ȳ�
	float unloader_coords[40];				//ж�������꣺x,y,z,state�豸״̬,focus��ý���
	float bucket_coords[40];				//ץ�����꣺x,y,z,ver�϶���ɫ��־0��ɫ1��ɫ,focus��ý���0ʧȥ����1
	float loader_coords[5];					//װ�������꣺x,y,z,state,focus
};

struct ShipFile
{
	char ship_name[50];
	int ship_state;
	float wait_pos[2];
	double angle;
	int berth_idx;
	float ship_total_storage;
	float ship_current_storage;
	int ship_index;
	int ship_type;
	float ship_coords[8];
	long double ship_dist;
};

//װ��¥�洢
struct TrainFile
{
	int building_state[2];					//װ��¥����״̬��-1��ɫϵͳ���ߣ�ͣ��״̬����0��ɫδ�ϵ����ϣ�1��ɫ����
	int building_mode[2];					//װ��ģʽ�л���0�Ƚ��ȳ�FIFO��1����ȳ�LIFO��2��16��18������3�ң�15��17����
	float building_coords_A[6];				//װ��¥A���꣺x,y,z,state,focus,mode��ȡֵ0-3��
	float building_coords_B[6];				//װ��¥B���꣺x,y,z,state,focus,mode��ȡֵ0-3��
};

//�ѳ��洢
struct YardFile
{
	int yard_choosed;						//ѡ�еĶѳ�
	int child_choosed;						//ѡ�е��Ӷѳ�
	int type_choosed;						//ѡ���Ӷѳ��Ļ������
	int index_choosed;						//ѡ���Ӷѳ��Ļ���С��
	float amount_choosed;					//ѡ���Ӷѳ��Ļ������
	bool data_send;							//�����Ƿ���
	bool yard_choosing;						//�Ƿ����ѳ�ѡ�񴰿�
	int flow_yard_choosed_1;				//������ѡ�еĶѳ�1��
	int flow_yard_choosed_2;				//������ѡ�еĶѳ�2��
	int flow_num_choose;					//������ѡ�еĶѳ�������
	int num_children[10];
	bool yard_focus[10];
};

//Ͳ�ִ洢
struct SiloFile
{
	bool data_send;							//�����Ƿ���
	bool select_window;						//�Ƿ���ʾͲ��ѡ�����
	float amount_apply;					//����ѷ���
	int selected_index;						//ѡ�е�Ͳ��
	bool silo_occupied[14];
	float storage_targets;					//Ŀ�����
	float storage_process;					//��ǰ�����仯
	int straight_state;						//ֱ��״̬��0Ϊ��ֱ����1Ϊֱ����죬2Ϊֱ������ú
	float straight_flux;					//ֱ������
	unsigned int out_total_0;				//�ܳ����
	unsigned int out_total_1;				//�ܳ�����ú
	float cur_storage[14];
	bool silo_type[14];
	float silo_flux[14];
	bool silo_focus[14];
	bool silo_state_up[14];
	bool silo_state_down[14];
	float silo_coords[8][14];
};

//�浵�ṹ��
struct Save
{
	ConsoleFile consoleFile;
	CoreFile coreFile;
	EnergyFile energyFile;
	EnvironmentFile environmentFile;
	int sizeofFragments;
	std::vector<FragmentFile> fragmentFiles;
	WebFile webFile;
	FlowFile flowFile;
	ConvFile convFile;
	int sizeofAct_flows;
	std::vector<FlowState> act_flows;
	WheelFile wheelFile;
	BerthFile berthFile;
	int sizeofShips;
	std::vector<ShipFile> ships;
	TrainFile trainFile;
	int sizeofTrains;
	std::vector<TrainAttrib> trains;
	YardFile yardFile;
	std::vector<Yard_child> yards;
	SiloFile siloFile;
};

class Record
{
public:
	Record();
	void initGuiStyle();
	void save_level(int mark, Message& message, bool& random_initiating, int& compitence, SimuCore& core, Energy& energy, Environment& environment, Web& web, Flow& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo);		//����浵
	void load_level(int mark, Message& message, bool& random_initiating, int& compitence, SimuCore& core, Energy& energy, Environment& environment, Web& web, Flow& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo);		//�浵��ȡ
	int showGui();			//��ʾ���棬������0��ʾ���·���������������ʾ�浵����λ�����ظ�����ʾ��������λ
	//bool new_saved;			//�ڵ�ǰ����ѭ���Ѿ��洢��һ�ε�����£���¼Ϊtrue����ʾ�ٴδ浵�Ḳ��ԭ�������һ���浵����¼Ϊfalse����ʾ��ǰ����ѭ����δ����������¼�µĴ浵

private:
	Save save_file;
	FragmentFile fragmentFile;
	FlowState flowState;
	ShipFile shipFile;
	TrainAttrib train;
	One_wagon wagon;
	Yard_child child;
	ImGuiStyle* style;
	void pre_button();
	void post_button();

};
